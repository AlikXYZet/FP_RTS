// 

// Base:
#include "RTS_AttributeSet.h"

// Global:
#include "RTS/Tools/Global/GlobalMacros.h"

// Net:
#include "Net/UnrealNetwork.h"

// UE:
#include "Perception/AISense_Damage.h"

// GAS:
#include "GameplayEffectExtension.h"
#include "RTS/Tools/GAS/RTS_GameplayTags.h"

// Interaction:
//#include "RTS/Core/RTS_GameMode.h"
//--------------------------------------------------------------------------------------



/* ---   Macros   --- */

// Смещение значений Атрибутов, ниже которого Атрибут воспринимается как с Нулевым значением
#define ZERO_VALUE_OFFSET 0.5f

#define GAMEPLAYSTATE_CHECKVALUE_MAX(PropertyName) \
    if (Get##PropertyName##() < GetMax##PropertyName##() \
        && GetMax##PropertyName##() == NewValue) \
    { \
        GetOwningAbilitySystemComponent()->AddLooseGameplayTag( \
            RTS_GameplayTags::GameplayState_##PropertyName##_Max); \
    } \
    else if (Get##PropertyName##() == GetMax##PropertyName##() \
        && GetMax##PropertyName##() > NewValue) \
    { \
        GetOwningAbilitySystemComponent()->RemoveLooseGameplayTag( \
            RTS_GameplayTags::GameplayState_##PropertyName##_Max, 3); \
    };
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

    // Передача информации о уроне
    if (Data.EvaluatedData.Magnitude < 0)
    {
        if (Data.EvaluatedData.Attribute == GetArmorAttribute()
            || Data.EvaluatedData.Attribute == GetHealthAttribute())
        {
            AActor* lOwningActor = GetOwningActor();
            FVector lLocation = lOwningActor->GetActorLocation();
            AActor* lInstigator = Data.EffectSpec.GetEffectContext().Get()->GetInstigator();

            // Информирование Актора средствами UE
            // PS: Излишне, так как используются уже встроенные средства GAS
            //UGameplayStatics::ApplyDamage(
            //    lOwningActor,
            //    Data.EvaluatedData.Magnitude,
            //    lInstigator ? lInstigator->GetInstigatorController() : nullptr,
            //    lInstigator,
            //    NULL);

            // Информирование ИИ
            UAISense_Damage::ReportDamageEvent(
                GetWorld(),
                lOwningActor,   // Кому урон
                lInstigator,    // Кто нанёс урон
                -Data.EvaluatedData.Magnitude,  // Величина урона
                lInstigator ? lInstigator->GetActorLocation() : lLocation,  // Место события
                lLocation);     // Место попадания
            // PS: Параметр "Место события" ("Event Location") используется как
            // местоположение "Инициатора" ("Instigator") на момент совершения события
        }
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
                RTS_GameplayTags::GameplayState_Armor_Zero);

            OnZeroArmor.Broadcast();
        }
        else if (GetArmor() < ZERO_VALUE_OFFSET
            && ZERO_VALUE_OFFSET <= NewValue)
        {
            GetOwningAbilitySystemComponent()->RemoveLooseGameplayTag(
                RTS_GameplayTags::GameplayState_Armor_Zero,
                3 /* Защита от многократного тега */);
        }

        GAMEPLAYSTATE_CHECKVALUE_MAX(Armor);
    }
    else if (Attribute == GetHealthAttribute())
    {
        if (GetHealth() >= ZERO_VALUE_OFFSET
            && ZERO_VALUE_OFFSET > NewValue)
        {
            GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                RTS_GameplayTags::GameplayState_Health_Zero);

            OnZeroHealth.Broadcast();
        }

        GAMEPLAYSTATE_CHECKVALUE_MAX(Health);
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
                RTS_GameplayTags::GameplayState_Armor_Zero);
        }
        else if (Attribute == GetHealthAttribute())
        {
            GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                RTS_GameplayTags::GameplayState_Health_Zero);
        }
    }
    else
    {
        // Ограничение по верхнему диапазону с проверкой в порядке от более часто изменяемого к редкому
        if (Attribute == GetArmorAttribute())
        {
            if (NewValue >= GetMaxArmor())
            {
                NewValue = GetMaxArmor();
                GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                    RTS_GameplayTags::GameplayState_Armor_Max);
            }
        }
        else if (Attribute == GetHealthAttribute())
        {
            if (NewValue >= GetMaxHealth())
            {
                NewValue = GetMaxHealth();
                GetOwningAbilitySystemComponent()->AddLooseGameplayTag(
                    RTS_GameplayTags::GameplayState_Health_Max);
            }
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
