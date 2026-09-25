//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "BehaviorTree/BTDecorator.h"

// UE:
#include "DataProviders/AIDataProvider.h"
#include "UObject/ObjectMacros.h"

// Structs:
#include "RTS/Tools/Structs/Properties/UnitCharacterData.h"

// Generated:
#include "BTD_IsAtInteractionDistance.generated.h"
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API UBTD_IsAtInteractionDistance : public UBTDecorator
{
    GENERATED_UCLASS_BODY()

    /* ---   Condition   --- */

    /* Пороговое значение расстояния, которое принимается как находящееся в Радиусе Взаимодействия,
    если не удалось получить данные из Юнита */
    UPROPERTY(EditAnywhere, Category = "Condition",
        meta = (ClampMin = 0, UIMin = 0,
            EditCondition = "FloatUnitData == EFloatUnitCharacterData::NONE"))
    float AcceptableRadius;

    /*  */
    UPROPERTY(EditAnywhere, Category = "Condition")
    EFloatUnitCharacterData FloatUnitData;

    /* Тип Расчёта геометрического Расстояния */
    UPROPERTY(EditAnywhere, Category = "Condition",
        meta = (EditCondition = "!bPathFindingBasedTest"))
    FAIDistanceType GeometricDistanceType;
    //-------------------------------------------



    /* ---   One Byte   --- */

    /* Если мы переходим к Объекту и этот Объект является навигационным Агентом,
    то мы перейдем к его местоположению навигационного агента */
    UPROPERTY(EditAnywhere, Category = Condition,
        meta = (EditCondition = "bPathFindingBasedTest"))
    uint32 bUseNavAgentGoalLocation : 1;

    /* Если значение равно true, результат будет соответствовать тестам, выполненным при следовании по траекториям.
    Установите значение false, чтобы использовать геометрическое расстояние, заданное с помощью 'Geometric Distance Type' */
    UPROPERTY(EditAnywhere, Category = Condition)
    uint32 bPathFindingBasedTest : 1;
    //-------------------------------------------



    /* ---   Blackboard   --- */

    /** Стартовая точка расчёта Дистанции */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector StartPoint;

    /** Конечная точка расчёта Дистанции */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector EndPoint;
    //-------------------------------------------



    /* ---   Functions : Base   --- */

    /** Инициализируйте любые данные, связанные с активами */
    virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

    /** Вызывается, когда контроллер потока выполнения становится Активным */
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    /** Вызывается, когда контроллер потока выполнения становится Неактивным */
    virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    /** Получить статическое Описание
    @return Строка, содержащая описание этого узла со всеми установочными значениями */
    virtual FString GetStaticDescription() const override;

    /** Уведомление об изменении ключа Blackboard: 'Start Point' */
    virtual EBlackboardNotificationResult OnBlackboardKeyValueChange_StartPoint(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

    /** Уведомление об изменении ключа Blackboard: 'End Point' */
    virtual EBlackboardNotificationResult OnBlackboardKeyValueChange_EndPoint(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
    //-------------------------------------------



    /* ---   Functions : Calculate   --- */

    /** Вычисляет исходное, базовое значение условия декоратора.
    Не должно включать вызов 'Is Inversed' */
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITOR

    virtual FName GetNodeIconName() const override;

#endif // WITH_EDITOR
    //===========================================



protected:

    /* ---   Functions : Calculate   --- */

    /** Получить квадрат геометрического расстояния */
    float GetGeometricDistanceSquared(const FVector& A, const FVector& B) const;
    //-------------------------------------------
};
