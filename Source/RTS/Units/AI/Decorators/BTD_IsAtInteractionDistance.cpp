//

// Base:
#include "BTD_IsAtInteractionDistance.h"

// UE:
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "Navigation/PathFollowingComponent.h"

// Interaction:
#include "RTS/Units/UnitCharacter.h"
//--------------------------------------------------------------------------------------



/* ---   Custom   --- */

// Класс-Обёртка для расчёта дистанции средствами ИИ Владельца
class UPathFollowingComponentTEST : public UPathFollowingComponent
{

public:

    /** Определить, достиг ли Объект другого Объекта по их местоположению
    @note   Код подобен 'UPathFollowingComponent:: HasReached (*)' */
    bool HasReachedDistance(const FVector& StartPoint, const FVector& EndPoint, EPathFollowingReachMode ReachMode, float InAcceptanceRadius) const
    {
        if (InAcceptanceRadius == UPathFollowingComponent::DefaultAcceptanceRadius)
        {
            InAcceptanceRadius = MyDefaultAcceptanceRadius;
        }

        const float AgentRadiusMod = (ReachMode == EPathFollowingReachMode::ExactLocation)
            || (ReachMode == EPathFollowingReachMode::OverlapGoal) ? 0.0f : MinAgentRadiusPct;
        return HasReachedInternal(EndPoint, 0.f, 0.f, StartPoint, InAcceptanceRadius, AgentRadiusMod);
    };
};
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

UBTD_IsAtInteractionDistance::UBTD_IsAtInteractionDistance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    NodeName = "Is At Interaction Distance";

    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = true;

    AcceptableRadius = 50.0f;
    bUseNavAgentGoalLocation = true;
    bPathFindingBasedTest = true;
    GeometricDistanceType = FAIDistanceType::Distance3D;

    // Принимаются только Объекты и Вектора
    StartPoint.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsAtInteractionDistance, StartPoint), AActor::StaticClass());
    StartPoint.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsAtInteractionDistance, StartPoint));
    EndPoint.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsAtInteractionDistance, EndPoint), AActor::StaticClass());
    EndPoint.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsAtInteractionDistance, EndPoint));

    // Невозможно прервать, ничего не наблюдается
    bAllowAbortLowerPri = false;
    bAllowAbortNone = false;
    bAllowAbortChildNodes = false;
    FlowAbortMode = EBTFlowAbortMode::None;
}
//--------------------------------------------------------------------------------------



/* ---   Functions : Base   --- */

void UBTD_IsAtInteractionDistance::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    UBlackboardData* BBAsset = GetBlackboardAsset();
    if (BBAsset)
    {
        StartPoint.ResolveSelectedKey(*BBAsset);
        EndPoint.ResolveSelectedKey(*BBAsset);
    }
    else
    {
        UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize %s due to missing blackboard data."), *GetName());
        StartPoint.InvalidateResolvedKey();
        EndPoint.ResolveSelectedKey(*BBAsset);
    }
}

void UBTD_IsAtInteractionDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        auto KeyID = StartPoint.GetSelectedKeyID();
        BlackboardComp->RegisterObserver(KeyID, this,
            FOnBlackboardChangeNotification::CreateUObject(this, &UBTD_IsAtInteractionDistance::OnBlackboardKeyValueChange_StartPoint));

        KeyID = EndPoint.GetSelectedKeyID();
        BlackboardComp->RegisterObserver(KeyID, this,
            FOnBlackboardChangeNotification::CreateUObject(this, &UBTD_IsAtInteractionDistance::OnBlackboardKeyValueChange_EndPoint));
    }
}

void UBTD_IsAtInteractionDistance::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->UnregisterObserversFrom(this);
    }
}

FString UBTD_IsAtInteractionDistance::GetStaticDescription() const
{
    FString KeyDescStart("'invalid'");

    if (StartPoint.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() ||
        StartPoint.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
    {
        KeyDescStart = StartPoint.SelectedKeyName.ToString();
    }

    FString KeyDescEnd("'invalid'");

    if (EndPoint.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() ||
        EndPoint.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
    {
        KeyDescEnd = EndPoint.SelectedKeyName.ToString();
    }

    return FString::Printf(TEXT("%s: \nfrom %s to %s"),
        *Super::GetStaticDescription(), *KeyDescStart, *KeyDescEnd);
}

EBlackboardNotificationResult UBTD_IsAtInteractionDistance::OnBlackboardKeyValueChange_StartPoint(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
    UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();
    if (BehaviorComp == nullptr)
    {
        return EBlackboardNotificationResult::RemoveObserver;
    }

    if (StartPoint.GetSelectedKeyID() == ChangedKeyID)
    {
        BehaviorComp->RequestExecution(this);
    }
    return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTD_IsAtInteractionDistance::OnBlackboardKeyValueChange_EndPoint(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
    UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();
    if (BehaviorComp == nullptr)
    {
        return EBlackboardNotificationResult::RemoveObserver;
    }

    if (EndPoint.GetSelectedKeyID() == ChangedKeyID)
    {
        BehaviorComp->RequestExecution(this);
    }
    return EBlackboardNotificationResult::ContinueObserving;
}
//--------------------------------------------------------------------------------------



/* ---   Functions : Calculate   --- */

bool UBTD_IsAtInteractionDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bHasReached = false;

    AAIController* AIOwner = OwnerComp.GetAIOwner();
    UPathFollowingComponent* PathFollowingComponent = AIOwner ? AIOwner->GetPathFollowingComponent() : nullptr;
    if (PathFollowingComponent)
    {
        const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
        float Radius = AcceptableRadius;

        // Получение параметра Юнита
        if (const AUnitCharacter* Unit = Cast<AUnitCharacter>(AIOwner->GetPawn()))
        {
            Radius = Unit->UnitCharacterData.AttackDistance;

            switch (FloatUnitData)
            {
            case EFloatUnitCharacterData::NONE:
                break;

            case EFloatUnitCharacterData::ViewingRadius:
                Radius = Unit->UnitCharacterData.ViewingRadius;
                break;

            case EFloatUnitCharacterData::AttackDistance:
                Radius = Unit->UnitCharacterData.AttackDistance;
                break;

            default:
                break;
            }
        }


        // Определение Стартовой точки
        FVector StartLocation = FAISystem::InvalidLocation;
        if (StartPoint.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
        {
            UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(StartPoint.GetSelectedKeyID());
            if (AActor* lActor = Cast<AActor>(KeyValue))
            {
                StartLocation = lActor->GetActorLocation();
            }
        }
        else if (StartPoint.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
        {
            StartLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(StartPoint.GetSelectedKeyID());
        }

        if (FAISystem::IsValidLocation(StartLocation))
        {
            // Определение Конечной точки
            FVector EndLocation = FAISystem::InvalidLocation;
            if (EndPoint.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
            {
                UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(EndPoint.GetSelectedKeyID());
                if (AActor* lActor = Cast<AActor>(KeyValue))
                {
                    EndLocation = lActor->GetActorLocation();
                }
            }
            else if (EndPoint.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
            {
                EndLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(EndPoint.GetSelectedKeyID());
            }

            if (FAISystem::IsValidLocation(EndLocation))
            {
                // Определение в пределах ли Радиуса
                bHasReached = bPathFindingBasedTest

                    ? ((UPathFollowingComponentTEST*)PathFollowingComponent)->HasReachedDistance(
                        StartLocation, EndLocation, EPathFollowingReachMode::OverlapAgent, Radius)

                    : GetGeometricDistanceSquared(StartLocation, EndLocation) < FMath::Square(Radius);
            }
        }
    }

    return bHasReached;
}

float UBTD_IsAtInteractionDistance::GetGeometricDistanceSquared(const FVector& A, const FVector& B) const
{
    float Result = MAX_flt;
    switch (GeometricDistanceType)
    {
    case FAIDistanceType::Distance3D:
        Result = FVector::DistSquared(A, B);
        break;
    case FAIDistanceType::Distance2D:
        Result = FVector::DistSquaredXY(A, B);
        break;
    case FAIDistanceType::DistanceZ:
        Result = FMath::Square(A.Z - B.Z);
        break;
    default:
        checkNoEntry();
        break;
    }
    return Result;
}
//--------------------------------------------------------------------------------------



/* ===   For EDITOR only   === */

#if WITH_EDITOR

FName UBTD_IsAtInteractionDistance::GetNodeIconName() const
{
    return FName("BTEditor.Graph.BTNode.Decorator.ReachedMoveGoal.Icon");
}

#endif	// WITH_EDITOR
//--------------------------------------------------------------------------------------
