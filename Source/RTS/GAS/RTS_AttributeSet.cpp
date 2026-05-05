// 

// Base:
#include "RTS_AttributeSet.h"

// Macros:
#include "RTS/Tools/GlobalMacros.h"

// Net:
#include "Net/UnrealNetwork.h"

// GAS:
#include "RTS/Tools/GAS/RTS_GameplayTags.h"
#include "GameplayEffectExtension.h"

// Interaction:
//#include "RTS/Core/RTS_GameMode.h"
//--------------------------------------------------------------------------------------



/* ---   Macros   --- */

// Смещение значений Атрибутов, ниже которого Атрибут воспринимается как с Нулевым значением
#define ZERO_VALUE_OFFSET 0.5f
//--------------------------------------------------------------------------------------



/* ---   Net   --- */

void URTS_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);


    /* ---   Attributes   --- */

    DOREPLIFETIME_CONDITION_NOTIFY(URTS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTS_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTS_AttributeSet, Armor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTS_AttributeSet, MaxArmor, COND_None, REPNOTIFY_Always);
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   AttributeSet   --- */

bool URTS_AttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    return Super::PreGameplayEffectExecute(Data);
}

void URTS_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (bIsZeroHealth)
    {
        //if (GetRTSGameMode())
        //{
        //    GetRTSGameMode()->***;
        //}

        bIsZeroHealth = false;
    }
}

void URTS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    /** Отслеживание изменений Атрибутов со стороны сервера */

    // Проверяем в порядке от более часто изменяемого к редкому
    if (Attribute == GetArmorAttribute())
    {
        if (GetArmor() >= ZERO_VALUE_OFFSET
            && ZERO_VALUE_OFFSET > NewValue)
        {
            GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                RTS_GameplayTags::GameplayState_WithoutArmor);

            OnZeroArmor.Broadcast();
        }
        else if (GetArmor() < ZERO_VALUE_OFFSET
            && ZERO_VALUE_OFFSET <= NewValue)
        {
            GetOwningAbilitySystemComponent()->RemoveLooseGameplayTag(
                RTS_GameplayTags::GameplayState_WithoutArmor,
                3 /* Защита от многократного тега */);
        }
    }
    else if (Attribute == GetHealthAttribute())
    {
        if (GetHealth() >= ZERO_VALUE_OFFSET
            && ZERO_VALUE_OFFSET > NewValue)
        {
            GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                RTS_GameplayTags::GameplayState_OnDestroyed);

            OnZeroHealth.Broadcast();

            bIsZeroHealth = true;
        }
    }

    Super::PreAttributeChange(Attribute, NewValue);
}

void URTS_AttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    /** Ограничение Атрибутов в соответствующем диапазоне */

    if (NewValue < ZERO_VALUE_OFFSET)
    {
        // Всеобщее ограничение в нижнем диапазоне
        NewValue = 0.f;

        // Отслеживание ключевых тегов на старте
        if (Attribute == GetArmorAttribute())
        {
            GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                RTS_GameplayTags::GameplayState_WithoutArmor);
        }
        else if (Attribute == GetHealthAttribute())
        {
            GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                RTS_GameplayTags::GameplayState_OnDestroyed);
        }
    }
    else
    {
        // Ограничение по верхнему диапазону с проверкой в порядке от более часто изменяемого к редкому
        if (Attribute == GetArmorAttribute())
        {
            NewValue = FMath::Min(NewValue, GetMaxArmor());
        }
        else if (Attribute == GetHealthAttribute())
        {
            NewValue = FMath::Min(NewValue, GetMaxHealth());
        }
    }

    Super::PreAttributeBaseChange(Attribute, NewValue);
}
//--------------------------------------------------------------------------------------



/* ---   Net | OnRep   --- */

void URTS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    /** Отслеживание со стороны клиента */
    if (Health.GetBaseValue() < ZERO_VALUE_OFFSET
        && ZERO_VALUE_OFFSET <= OldHealth.GetBaseValue())
    {
        OnZeroHealth.Broadcast();
    }

    GAMEPLAYATTRIBUTE_REPNOTIFY(URTS_AttributeSet, Health, OldHealth);
}

void URTS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URTS_AttributeSet, MaxHealth, OldMaxHealth);
}

void URTS_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
    /** Отслеживание со стороны клиента */
    if (Armor.GetBaseValue() < ZERO_VALUE_OFFSET
        && ZERO_VALUE_OFFSET <= OldArmor.GetBaseValue())
    {
        OnZeroArmor.Broadcast();
    }

    GAMEPLAYATTRIBUTE_REPNOTIFY(URTS_AttributeSet, Armor, OldArmor);
}

void URTS_AttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URTS_AttributeSet, MaxArmor, OldMaxArmor);
}
//--------------------------------------------------------------------------------------
