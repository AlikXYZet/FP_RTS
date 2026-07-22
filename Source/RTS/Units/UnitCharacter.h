//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/Character.h"

// Interfaces:
#include "GenericTeamAgentInterface.h"
#include "RTS/Tools/GAS/RTS_AbilitySystemInterface.h"
#include "RTS/Tools/Interfaces/Properties/InteractiveInterface.h"
#include "RTS/Tools/Interfaces/Properties/SelectableActorInterface.h"

// Structs:
#include "RTS/Tools/Structs/Properties/FactionData.h"
#include "RTS/Tools/Structs/Properties/UnitCharacterData.h"

// Generated:
#include "UnitCharacter.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction | GAS:
class URTS_AttributeSet;

// Interaction:
class UInteractiveComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API AUnitCharacter : public ACharacter,
    /* UE4: */  public IGenericTeamAgentInterface,
    /* RTS: */  public IRTS_AbilitySystemInterface, public IInteractiveInterface, public ISelectableActorInterface
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this character's properties
    AUnitCharacter();
    //-------------------------------------------



    /* ---   Components   --- */

    /* Декаль выделения данного Персонажа */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UDecalComponent* Decal = nullptr;
    //-------------------------------------------



    /* ---   Non-scene Components   --- */

    /* Компонент Интерактивности с данным Юнитом */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UInteractiveComponent* InteractiveComponent = nullptr;

    // Компонент Системы Способностей (GAS)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    URTS_AbilitySystemComponent* AbilitySystemComp = nullptr;

    // Скрытый Набор Атрибутов (для GAS)
    UPROPERTY(BlueprintReadOnly,
        Category = "Components")
    URTS_AttributeSet* AttributeSet = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Вызывается при Запуске игры или при Спавне в уже запущенной игре
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Функция, вызываемая каждый кадр в этом Акторе, если не назначена другая частота */
    //virtual void Tick(float DeltaSeconds) override;

    /** Вызывается, когда этот субъект явно уничтожается во время игрового процесса или в редакторе,
    * но не вызывается во время трансляции уровней или завершения игрового процесса */
    virtual void Destroyed() override;

    /** Реакция на Выпадение из Мира (ниже KillZ и тому подобного) */
    virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

    /** Вызывается перед инициализацией всех компонентов */
    virtual void PreInitializeComponents() override;

    /** Вызывается при подключения Контроллера
    @note   Вызывается только на сервере (или в автономном режиме)
    @param  NewController - Контроллер, захвативший владение данным Игроком (Пешкой) */
    virtual void PossessedBy(AController* NewController) override;
    //-------------------------------------------



    /* ---   Interface: GAS   --- */

    /** Возвращает Компонент Системы Способностей данного Игрока */
    FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
    {
        return AbilitySystemComp;
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



    /* ---   Selectable Actor   --- */
    //-------------------------------------------



    /* ---   Interface: Generic Team Agent   --- */

    /** Присваивает Идентификатор Команды (Номер Фракции) */
    UFUNCTION(BlueprintCallable,
        Category = "Unit Character|Generic Team Agent")
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override
    {
        TeamID = NewTeamID;
    };

    /** Извлекает Идентификатор Команды в виде FGenericTeamId */
    UFUNCTION(BlueprintCallable,
        Category = "Unit Character|Generic Team Agent")
    virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
    //-------------------------------------------



    /* ---   Unit Character Data   --- */

    /* Индивидуальные Данные данного Юнита */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Character|Unit Data")
    FUnitCharacterData UnitCharacterData;
    //-------------------------------------------



    /* ---   Statistics   --- */

    /** Получить данные о Фракци */
    UFUNCTION(BlueprintPure,
        Category = "Unit Character|Statistics")
    const FFactionData& GetFactionData() const;
    //-------------------------------------------



private:

    /* ---   Interface: GAS   --- */

    /** Возвращает Компонент Атрибутов данного Игрока */
    FORCEINLINE URTS_AttributeSet* GetRTSAttributeSet() const override
    {
        return AttributeSet;
    };

    /** Инициализация данных GAS */
    void InitAbilitySystemComp() override;
    //-------------------------------------------



    /* ---   GAS Events   --- */

    /** Событие: При Нулевом Здоровье */
    UFUNCTION()
    void OnZeroHealth();
    //-------------------------------------------



    /* ---   Selectable Actor   --- */

    /* Текущее значение выбора игрока */
    EActorSelectionMode CurrentSelectionMode = EActorSelectionMode::NotSelected;
    //-------------------------------------------



    /* ---   Generic Team Agent   --- */

    /* Номер Фракции данного Югита */
    UPROPERTY(EditAnywhere,
        Category = "Unit Character|Generic Team Agent")
    FGenericTeamId TeamID;
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITORONLY_DATA

private:

    /* ---   Statistics   --- */

    /* Таблица Данных: Данные Фракций
    @note   Используется как заглушка, при отсутствии "GameMode" в режиме редактора */
    UPROPERTY(EditDefaultsOnly,
        Category = "Unit Character|Statistics",
        meta = (NoResetToDefault, RequiredAssetDataTags = "RowStructure=FactionData"))
    UDataTable* ReserveFactionsData = nullptr;
    //-------------------------------------------

#endif // WITH_EDITORONLY_DATA
};
