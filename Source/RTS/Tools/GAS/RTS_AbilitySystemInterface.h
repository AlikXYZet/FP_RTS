// 

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "AbilitySystemInterface.h"

// Interaction:
#include "RTS/GAS/RTS_AttributeSet.h"

// Generated:
#include "RTS_AbilitySystemInterface.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Macros   --- */

/** Макрос: Создание функции делегата для передачи изменённого значения атрибутов GAS через Событие BP */
#define GAMEPLAYATTRIBUTE_VALUE_HandleChanged(PropertyName) \
	FORCEINLINE void Handle##PropertyName##Changed(const FOnAttributeChangeData& Data) \
	{ \
		Execute_Event_Changing##PropertyName(GetAbilitySystemComponent()->GetOwnerActor(), Data.NewValue); \
	}

/** Макрос: Подписка функции к делегату для передачи изменённого значения атрибутов GAS через Событие BP */
#define GAMEPLAYATTRIBUTE_VALUE_Delegating(OwnerClass, PropertyName) \
    GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetRTSAttributeSet()->Get##PropertyName##Attribute()) \
        .AddUObject(this, &OwnerClass::Handle##PropertyName##Changed);

/** Макрос: Создание функции делегата для передачи события нулевого значения атрибутов GAS через Событие BP */
#define GAMEPLAYATTRIBUTE_ZERO_HandleEvent(PropertyName) \
	FORCEINLINE void Handle##PropertyName() \
	{ \
		Execute_Event_##PropertyName(GetAbilitySystemComponent()->GetOwnerActor()); \
	}

/** Макрос: Подписка функции к делегату для передачи события нулевого значения атрибутов GAS через Событие BP */
#define GAMEPLAYATTRIBUTE_ZERO_Delegating(OwnerClass, PropertyName) \
     GetRTSAttributeSet()->PropertyName.AddUObject(this, &OwnerClass::Handle##PropertyName);
//--------------------------------------------------------------------------------------



// This class does not need to be modified.
UINTERFACE(Blueprintable)
class RTS_API URTS_AbilitySystemInterface : public UAbilitySystemInterface
{
    GENERATED_BODY()
};



/* Интерфейс Интерактивности Актора
* @note Используется как шаблон для взаимодействия с "UInteractiveComponent" */
class RTS_API IRTS_AbilitySystemInterface : public IAbilitySystemInterface
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Быстрая Проверка Объекта на реализацию данного Интерфейса
    @note   Вызов методов 'Execute_*' в Объекте без реализованного Интерфейса вызовет фатальную ошибку */
    FORCEINLINE static bool CheckImplementation(const UObject* O)
    {
        return O ? O->GetClass()->ImplementsInterface(URTS_AbilitySystemInterface::StaticClass()) : false;
    };
    //-------------------------------------------



    /* ---   GAS   --- */

    /** Возвращает Компонент Атрибутов данного Актора */
    FORCEINLINE virtual URTS_AttributeSet* GetRTSAttributeSet() const = 0;
    //-------------------------------------------

protected:

    /* ---   GAS   --- */

    /** Инициализация данных GAS */
    FORCEINLINE virtual void InitAbilitySystemComp() = 0;
    //-------------------------------------------



public:

    /* ---   GAS Events: New Value   --- */

    /** Событие BP: Изменение Здоровья */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "RTS Ability System|Events",
        meta = (DisplayName = "Changing Health"))
    void Event_ChangingHealth(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(Health);

    /** Событие BP: Изменение максимального Здоровья */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "RTS Ability System|Events",
        meta = (DisplayName = "Changing Max Health"))
    void Event_ChangingMaxHealth(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(MaxHealth);

    /** Событие BP: Изменение Брони */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "RTS Ability System|Events",
        meta = (DisplayName = "Changing Armor"))
    void Event_ChangingArmor(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(Armor);

    /** Событие BP: Изменение максимальной Брони */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "RTS Ability System|Events",
        meta = (DisplayName = "Changing Max Armor"))
    void Event_ChangingMaxArmor(float Data);
    GAMEPLAYATTRIBUTE_VALUE_HandleChanged(MaxArmor);
    //-------------------------------------------



    /* ---   GAS Events: On Zero Value   --- */

    /** Событие BP: При Нулевом Здоровье */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "RTS Ability System|Events",
        meta = (DisplayName = "On Zero Health"))
    void Event_OnZeroHealth();
    GAMEPLAYATTRIBUTE_ZERO_HandleEvent(OnZeroHealth);

    /** Событие BP: При Нулевой Броне */
    UFUNCTION(BlueprintImplementableEvent,
        Category = "RTS Ability System|Events",
        meta = (DisplayName = "On Zero Armor"))
    void Event_OnZeroArmor();
    GAMEPLAYATTRIBUTE_ZERO_HandleEvent(OnZeroArmor);
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   undef   --- */

// Используются вне данного файла
//#undef GAMEPLAYATTRIBUTE_VALUE_HandleChanged
//#undef GAMEPLAYATTRIBUTE_VALUE_Delegating
//#undef GAMEPLAYATTRIBUTE_ZERO_HandleEvent
//#undef GAMEPLAYATTRIBUTE_ZERO_Delegating
//--------------------------------------------------------------------------------------
