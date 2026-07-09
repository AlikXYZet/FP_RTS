// 

// Base:
#include "RTS_AbilitySystemComponent.h"

// Global:
#include "RTS/Tools/Global/GlobalMacros.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

URTS_AbilitySystemComponent::URTS_AbilitySystemComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.
    // You can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false; // Предварительно
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void URTS_AbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(
        Timer_InitStartingData,
        this,
        &URTS_AbilitySystemComponent::InitStartingData,
        0.5f,
        false);
}

void URTS_AbilitySystemComponent::OnComponentCreated()
{
    Super::OnComponentCreated();
}

void URTS_AbilitySystemComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);
}
//--------------------------------------------------------------------------------------



/* ---   GAS   --- */

void URTS_AbilitySystemComponent::InitStartingData()
{
    /* ---   GAS : Checks   --- */

    CheckTagsInAbilitiesEffectsParams();
    CheckTagsInStartingEffectsWithParameters();
    //-------------------------------------------

    if (GetOwnerActor() && GetOwnerActor()->HasAuthority())
    {
        InitStartingAttributes();
        InitAvailableAbilities();
        InitStartingEffectsWithParameters();
    }
}

void URTS_AbilitySystemComponent::SetParamsInGameplayEffectSpec(FGameplayEffectSpec& Spec, const TMap<FGameplayTag, float>& Params)
{
    Spec.SetByCallerTagMagnitudes.Append(Params);
}

void URTS_AbilitySystemComponent::CheckTagsInParams(TMap<FGameplayTag, float>& Params)
{
#if WITH_EDITOR

    for (TPair<FGameplayTag, float>& Param : Params)
    {
        if (!Param.Key.IsValid())
        {
            M_LOG_Component(Warning, "Invalid param: '%s' with a value of '%f' ",
                *Param.Key.GetTagName().ToString(), Param.Value);
        }
    }
#endif // WITH_EDITOR

    Params.Remove(FGameplayTag::EmptyTag);
}
//--------------------------------------------------------------------------------------



/* ---   GAS | Attributes   --- */

FGameplayEffectSpecHandle URTS_AbilitySystemComponent::MakeEffectSpecWithParams(
    const TSubclassOf<UGameplayEffect> EffectClass,
    const float Level)
{
    if (!EffectClass) return FGameplayEffectSpecHandle();

    FGameplayEffectSpecHandle lResult = MakeOutgoingSpec(EffectClass, Level, MakeEffectContext());
    SetParamsInGameplayEffectSpec(*lResult.Data, AbilitiesEffectsParams);

    return lResult;
}

FActiveGameplayEffectHandle URTS_AbilitySystemComponent::ApplyEffectSpecToTargetWithParams(
    UAbilitySystemComponent* TargetASC,
    const TSubclassOf<UGameplayEffect> EffectClass,
    const float Level)
{
    if (!EffectClass || !TargetASC) return FActiveGameplayEffectHandle();

    return ApplyGameplayEffectSpecToTarget(
        *MakeEffectSpecWithParams(EffectClass, Level).Data,
        TargetASC);
}

void URTS_AbilitySystemComponent::InitStartingAttributes()
{
    for (const TPair<FGameplayAttribute, float>& Pair : StartingAttributes)
    {
        // Фильтрация от незаполненного значения с проверкой свойства на тип "FGameplayAttributeData"
        // PS: Является защитой, от ошибки при попытке изменить данные-"не атрибуты" в "AttributeSet",
        // например при ключе == "URTS_AttributeSet.OnZeroHealth"
        if (Pair.Key.IsGameplayAttributeDataProperty(Pair.Key.GetUProperty()))
        {
            if (GetAttributeSubobject(Pair.Key.GetAttributeSetClass()))
            {
                SetNumericAttributeBase(Pair.Key, Pair.Value);
            }
            else
            {
                M_Error_Component("'%s' is NOT used for this Actor. See Attribute '%s' ",
                    *Pair.Key.GetAttributeSetClass()->GetName(), *Pair.Key.GetName());
            }
        }
        else
        {
            M_LOG_Component(Warning, "Incorrect Attribute Name: '%s'",
                *Pair.Key.GetName());
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   GAS | Abilities   --- */

void URTS_AbilitySystemComponent::InitAvailableAbilities()
{
    for (TSubclassOf<UGameplayAbility>& GAClass : AvailableAbilities)
    {
        // Фильтрация от незаполненного значения
        if (GAClass.Get())
        {
            GiveAbility(
                FGameplayAbilitySpec(
                    GAClass,
                    1,
                    INDEX_NONE,
                    GetOwnerActor()));
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   GAS | Effects   --- */

void URTS_AbilitySystemComponent::RemoveAllGameplayEffects()
{
    for (const FActiveGameplayEffect& AGE : &ActiveGameplayEffects)
    {
        RemoveActiveGameplayEffect(AGE.Handle);
    }
}

void URTS_AbilitySystemComponent::InitStartingEffectsWithParameters()
{
    for (TPair<TSubclassOf<UGameplayEffect>, FEffectsParamData>& Pair : StartingEffectsWithParameters)
    {
        // Фильтрация от незаполненного значения
        if (Pair.Key.Get())
        {
            FGameplayEffectSpec lSpec = FGameplayEffectSpec(
                Pair.Key->GetDefaultObject<UGameplayEffect>(), MakeEffectContext());

            SetParamsInGameplayEffectSpec(lSpec, Pair.Value.SetByCallerValues);

            ApplyGameplayEffectSpecToSelf(lSpec);
        }
    }
}

void URTS_AbilitySystemComponent::CheckTagsInStartingEffectsWithParameters()
{
    for (TPair<TSubclassOf<UGameplayEffect>, FEffectsParamData>& Pair : StartingEffectsWithParameters)
    {
        CheckTagsInParams(Pair.Value.SetByCallerValues);
    }
}
//--------------------------------------------------------------------------------------
