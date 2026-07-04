//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/Character.h"

// Interfaces:
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "RTS/Tools/Interfaces/Properties/InteractiveInterface.h"
#include "RTS/Tools/Interfaces/Properties/SelectableActorInterface.h"

// Structs:
#include "RTS/Tools/Structs/Properties/UnitCharacterData.h"

// GAS:
#include "RTS/GAS/RTS_AbilitySystemComponent.h"

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
    /* UE4: */  public IAbilitySystemInterface, public IGenericTeamAgentInterface,
    /* RTS: */  public IInteractiveInterface, public ISelectableActorInterface
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



    /* ---   GAS Events   --- */

    /** Событие BP: Изменение Здоровья */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Gameplay Ability System|Events",
        meta = (DisplayName = "Changing Health"))
    void Event_ChangingHealth(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(Health);

    /** Событие BP: Изменение максимального Здоровья */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Gameplay Ability System|Events",
        meta = (DisplayName = "Changing Max Health"))
    void Event_ChangingMaxHealth(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(MaxHealth);

    /** Событие BP: Изменение Брони */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Gameplay Ability System|Events",
        meta = (DisplayName = "Changing Armor"))
    void Event_ChangingArmor(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(Armor);

    /** Событие BP: Изменение максимальной Брони */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Gameplay Ability System|Events",
        meta = (DisplayName = "Changing Max Armor"))
    void Event_ChangingMaxArmor(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(MaxArmor);

    /** Событие BP: При Нулевом Здоровье */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Gameplay Ability System|Events",
        meta = (DisplayName = "On Zero Health"))
    void Event_OnZeroHealth();

    /** Событие BP: При Нулевой Броне */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "Gameplay Ability System|Events",
        meta = (DisplayName = "On Zero Armor"))
    void Event_OnZeroArmor();
    //-------------------------------------------



    /* ---   Interface: Interactive   --- */

    /** Получить компоненты, которые требуется подсветить */
    virtual TArray<FComponentRendering> GetUsedComponents_Implementation() override;
    //-------------------------------------------



    /* ---   Interactive   --- */
    //-------------------------------------------



    /* ---   Interface: Selectable Actor   --- */

    /** Установить состояние "Выбранный" */
    virtual void SetSelectedByPlayer_Implementation(bool bIsSelected) override;

    /** Является ли "Выбранным"? */
    virtual bool IsSelectedByPlayer_Implementation() const override;
    //-------------------------------------------



    /* ---   Selectable Actor   --- */
    //-------------------------------------------



    /* ---   Interface: Generic Team Agent   --- */

    /** Присваивает агенту команды заданный TeamID */
    UFUNCTION(BlueprintCallable,
        Category = "Generic Team Agent")
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override
    {
        TeamID = NewTeamID;
    };

    /** Извлекает идентификатор команды в виде FGenericTeamId */
    UFUNCTION(BlueprintCallable,
        Category = "Generic Team Agent")
    virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
    //-------------------------------------------



    /* ---   Unit Character Data   --- */

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Data")
    FUnitCharacterData UnitCharacterData;
    //-------------------------------------------



private:

    /* ---   GAS   --- */

    /** Инициализация данных AbilitySystemComp */
    void InitAbilitySystemComp();
    //-------------------------------------------



    /* ---   GAS Events   --- */

    /** Событие: При Нулевом Здоровье */
    UFUNCTION()
    void OnZeroHealth();
    //-------------------------------------------



    /* ---   Generic Team Agent   --- */

    UPROPERTY(EditAnywhere,
        Category = "Generic Team Agent")
    FGenericTeamId TeamID;
    //-------------------------------------------
};
