//

// Base:
#include "UnitCharacter.h"

// Global:
#include "RTS/Tools/Global/GlobalFunctions.h"
#include "RTS/Tools/Global/GlobalMacros.h"

// GAS:
#include "RTS/GAS/RTS_AbilitySystemComponent.h"
#include "RTS/GAS/RTS_AttributeSet.h"

// UE:
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Plugins:
#include "SkeletalMeshComponentBudgeted.h"

// Interaction:
#include "RTS/ActorComponents/Properties/InteractiveComponent.h"
#include "RTS/Core/RTS_GameMode.h"
#include "RTS/Core/RTS_PlayerController.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

AUnitCharacter::AUnitCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USkeletalMeshComponentBudgeted>(ACharacter::MeshComponentName))
{
    // Set this pawn to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно
    SetActorTickInterval(1.f); // 1 раз/сек.

    bUseControllerRotationYaw = false;
    //-------------------------------------------


    /* ---   Components   --- */

    // Главный Меш образа
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    GetMesh()->bReceivesDecals = false;

    // Декаль выделения данного Персонажа
    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->SetupAttachment(GetCapsuleComponent());
    Decal->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
    Decal->SetUsingAbsoluteRotation(true);
    Decal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    Decal->SetRelativeScale3D(FVector(0.3f));
    Decal->SetHiddenInGame(true);

    /* Виджет отображения Атрибутов данного Юнита */
    AttributesWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Attributes Widget"));
    AttributesWidget->SetupAttachment(RootComponent);
    AttributesWidget->SetHiddenInGame(true);
    //-------------------------------------------


    /* ---   Non-scene Components   --- */

    // Компонент Интерактивности с данным Юнитом
    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("Interactive Component"));

    // Компонент Системы Способностей (GAS)
    AbilitySystemComp = CreateDefaultSubobject<URTS_AbilitySystemComponent>(TEXT("Ability System Comp"));
    AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Скрытый Набор Атрибутов (для GAS)
    //AttributeSet = CreateDefaultSubobject<UFPS_AttributeSet>(TEXT("Attributes"));
    // PS: Создание перенесено в `PreInitializeComponents()`, см. в обсуждении ошибки UE-81109
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void AUnitCharacter::BeginPlay()
{
    Super::BeginPlay();

    InitAbilitySystemComp();

    if (GetRTSGameMode())
    {
        GetRTSGameMode()->UnitRegistration(this);
    }
}

//void AUnitCharacter::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//}

void AUnitCharacter::Destroyed()
{
    if (AttributeSet)
    {
        // Вызываем уничтожение компонента, созданного через 'NewObject<*>(*)'
        AttributeSet->ConditionalBeginDestroy();
    }

    Super::Destroyed();
}

void AUnitCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
    AttributeSet->SetHealth(0);
}

void AUnitCharacter::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    // Скрытый Набор Атрибутов (для GAS)
    if (HasAuthority() && !AttributeSet)
    {
        AttributeSet = NewObject<URTS_AttributeSet>(this, TEXT("Attributes"));
        AbilitySystemComp->AddAttributeSetSubobject(AttributeSet);
        // PS: Warning! Данный метод автоматически Реплицирует "AttributeSet" на стороне Клиентов
    }
    // PS: Создание здесь экземпляра AttributeSet через NewObject<T>(*),
    // а не в конструкторе через CreateDefaultSubobject<T>(*),
    // является решением ошибки, описанной в UE-81109:
    // "уничтожение сборщиком AttributeSet у дубликатов актора-владельца"
}

void AUnitCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (AAIController* AIC = Cast<AAIController>(NewController))
    {
        AIC->SetGenericTeamId(TeamID);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Interface: Interactive   --- */

TArray<FComponentRendering> AUnitCharacter::GetUsedComponents_Implementation()
{
    return TArray<FComponentRendering>{ FComponentRendering(GetMesh(), TeamID ? 1 : 0) };
}
//--------------------------------------------------------------------------------------



/* ---   Interactive   --- */
//--------------------------------------------------------------------------------------



/* ---   GAS   --- */

void AUnitCharacter::InitAbilitySystemComp()
{
    if (GetAbilitySystemComponent())
    {
        if (GetRTSAttributeSet())
        {
            GAMEPLAYATTRIBUTE_VALUE_Delegating(Health);
            GAMEPLAYATTRIBUTE_VALUE_Delegating(MaxHealth);
            GAMEPLAYATTRIBUTE_VALUE_Delegating(Armor);
            GAMEPLAYATTRIBUTE_VALUE_Delegating(MaxArmor);

            GetRTSAttributeSet()->OnZeroHealth.AddUObject(this, &AUnitCharacter::OnZeroHealth);
            GAMEPLAYATTRIBUTE_ZERO_Delegating(OnZeroArmor);
        }
    }
    else
    {
        M_Error("AbilitySystemComp is NOT");
    }
}
//--------------------------------------------------------------------------------------



/* ---   GAS Events   --- */

void AUnitCharacter::OnZeroHealth()
{
    /* ---   Убираем всё лишнее   --- */

    if (GetCharacterMovement())
        GetCharacterMovement()->DestroyComponent();

    if (Decal)
        Decal->DestroyComponent();

    if (AttributesWidget)
        AttributesWidget->DestroyComponent();

    if (InteractiveComponent)
        InteractiveComponent->DestroyComponent();

    // @note    'AbilitySystemComp' необходим для дальнейшего взаимодействия оставшихся Снарядов
    //if (AbilitySystemComp)
    //    AbilitySystemComp->DestroyComponent();

    // @note    'AttributeSet' не удаляем, так как его делегат 'OnZeroHealth' вызывает данный метод класса
    //-------------------------------------------


    /* ---   Изменяем параметры   --- */

    if (GetCapsuleComponent())
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (GetMesh())
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetSimulatePhysics(true);
    }
    //-------------------------------------------


    /* ---   Регистрируем и Информируем об уничтожении   --- */

    if (GetRTSGameMode())
    {
        GetRTSGameMode()->RegisteringUnitDestruction(this);
    }

    SetLifeSpan(10.f);

    Execute_Event_OnZeroHealth(this);
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Interface: Selectable Actor   --- */

void AUnitCharacter::SetSelectionMode_Implementation(EActorSelectionMode Mode)
{
    if (Decal && GetSelectionMode() != Mode)
    {
        CurrentSelectionMode = Mode;
        Decal->SetHiddenInGame(Mode == EActorSelectionMode::NotSelected);
    }
}

EActorSelectionMode AUnitCharacter::GetSelectionMode() const
{
    return CurrentSelectionMode;
}
//--------------------------------------------------------------------------------------



/* ---   Statistics   --- */

const FFactionData& AUnitCharacter::GetFactionData() const
{
    if (GetRTSGameMode())
    {
        return GetRTSGameMode()->GetFactionData(this);
    }

#if WITH_EDITOR

    else if (ReserveFactionsData)
    {
        TArray<FFactionData*> lAllRows;
        ReserveFactionsData->GetAllRows<FFactionData>(__FUNCTION__, lAllRows);

        if (lAllRows.IsValidIndex(TeamID))
        {
            return *lAllRows[TeamID];
        }
    }

#endif // WITH_EDITOR

    return FFactionData::Empty;
}
//--------------------------------------------------------------------------------------
