//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/Actor.h"

// Interfaces:
#include "GenericTeamAgentInterface.h"
#include "RTS/Tools/GAS/RTS_AbilitySystemInterface.h"
#include "RTS/Tools/Interfaces/Properties/InteractiveInterface.h"
#include "RTS/Tools/Interfaces/Properties/SelectableActorInterface.h"

// Structs:
#include "RTS/Tools/Structs/Properties/FactionData.h"

// Interaction:
#include "RTS/GAS/RTS_AbilitySystemComponent.h"

// Generated:
#include "AttributedActor.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class UAIPerceptionStimuliSourceComponent;
class UWidgetComponent;

// Interaction | GAS:
class URTS_AttributeSet;

// Interaction:
class UInteractiveComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API AAttributedActor : public AActor,
    /* UE4: */  public IGenericTeamAgentInterface,
    /* RTS: */  public IRTS_AbilitySystemInterface, public IInteractiveInterface, public ISelectableActorInterface
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    AAttributedActor();
    //-------------------------------------------



    /* ---   Components   --- */

    /** Меш визуализации */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = Components,
        meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StaticMesh = nullptr;

    /* Декаль выделения данного Персонажа */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UDecalComponent* Decal = nullptr;

    /* Виджет отображения Атрибутов данного Актора */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UWidgetComponent* AttributesWidget;
    //-------------------------------------------



    /* ---   Non-scene Components   --- */

    /* Компонент Интерактивности с данным Актором */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UInteractiveComponent* InteractiveComponent = nullptr;

    /* Компонент "стимуляции" Сенсорики */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource = nullptr;

    /* Компонент Системы Способностей (GAS) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    URTS_AbilitySystemComponent* AbilitySystemComp = nullptr;

    /* Скрытый Набор Атрибутов (для GAS) */
    UPROPERTY(BlueprintReadOnly,
        Category = "Components")
    URTS_AttributeSet* AttributeSet = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //-------------------------------------------



    /* ---   Interface: GAS   --- */

    /** Инициализация данных GAS */
    void InitAbilitySystemComp() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Вызывается перед инициализацией всех компонентов */
    virtual void PreInitializeComponents() override;
    //-------------------------------------------



    /* ---   Interface: GAS   --- */

    /** Возвращает Компонент Системы Способностей данного Игрока */
    FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
    {
        return AbilitySystemComp;
    };

    /** Возвращает Компонент Атрибутов данного Актора */
    FORCEINLINE URTS_AttributeSet* GetRTSAttributeSet() const override
    {
        return AttributeSet;
    };
    //-------------------------------------------



    /* ---   Interface: Interactive   --- */

    /** Получить компоненты, которые требуется подсветить */
    virtual TArray<FComponentRendering> GetUsedComponents_Implementation() override;
    //-------------------------------------------



    /* ---   Interactive   --- */
    //-------------------------------------------



    /* ---   Interface: Selectable Actor   --- */

    /** Установить состояние "Выбранный" */
    virtual void SetSelectionMode_Implementation(EActorSelectionMode Mode) override;

    /** Является ли "Выбранным"? */
    UFUNCTION(BlueprintCallable,
        Category = "Selectable Actor")
    virtual EActorSelectionMode GetSelectionMode() const override;
    //-------------------------------------------



    /* ---   Interface: Generic Team Agent   --- */

    /** Присваивает Идентификатор Команды (Номер Фракции) */
    UFUNCTION(BlueprintCallable,
        Category = "Attributed Actor|Generic Team Agent")
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override
    {
        TeamID = NewTeamID;
    };

    /** Извлекает Идентификатор Команды в виде FGenericTeamId */
    UFUNCTION(BlueprintCallable,
        Category = "Attributed Actor|Generic Team Agent")
    virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
    //-------------------------------------------



    /* ---   Statistics   --- */

    /** Получить данные о Фракци */
    UFUNCTION(BlueprintPure,
        Category = "Attributed Actor|Statistics")
    const FFactionData& GetFactionData() const;
    //-------------------------------------------



private:

    /* ---   Selectable Actor   --- */

    /* Текущее значение выбора игрока */
    EActorSelectionMode CurrentSelectionMode = EActorSelectionMode::NotSelected;
    //-------------------------------------------



    /* ---   Generic Team Agent   --- */

    /* Номер Фракции данного Объекта */
    UPROPERTY(EditAnywhere,
        Category = "Attributed Actor|Generic Team Agent")
    FGenericTeamId TeamID;
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITORONLY_DATA

private:

    /* ---   Statistics   --- */

    /* Таблица Данных: Данные Фракций
    @note   Используется как заглушка, при отсутствии "GameMode" в режиме редактора */
    UPROPERTY(EditDefaultsOnly,
        Category = "Attributed Actor|Statistics",
        meta = (NoResetToDefault, RequiredAssetDataTags = "RowStructure=FactionData"))
    UDataTable* ReserveFactionsData = nullptr;
    //-------------------------------------------

#endif // WITH_EDITORONLY_DATA
};
