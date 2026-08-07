//

#pragma once

// Core:
#include "Kismet/BlueprintFunctionLibrary.h"

// Global:
#include "Enums/IsValid.h"
#include "GlobalMacros.h"

// Interaction:
#include "RTS/GAS/RTS_AbilitySystemComponent.h"
#include "RTS/GAS/RTS_AttributeSet.h"
#include "RTS_AbilitySystemInterface.h"

// Generated:
#include "RTS_AbilitySystemBlueprintLibrary.generated.h"
//--------------------------------------------------------------------------------------



/* ---   RTS Ability System Blueprint Library   --- */

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "RTS_AbilitySystemBlueprintLibrary"))
class RTS_API URTS_AbilitySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    /* ---   RTS Attribute Set   --- */

    /** Получить "Ability System Component" с проверкой на Валидность, используя "Ability System Interface" */
    UFUNCTION(BlueprintCallable,
        Category = "Utilities|GAS",
        meta = (ReturnDisplayName = "AS Comp", ExpandEnumAsExecs = "Validity"))
    static UAbilitySystemComponent* GetValidAbilitySystemComponent(UPARAM(ref) AActor* Actor, EIsValid& Validity)
    {
        UAbilitySystemComponent* lASC = nullptr;

        if (const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Actor))
        {
            lASC = ASI->GetAbilitySystemComponent();
        }

#if WITH_EDITOR
        else
        {
            M_LOG_Static(Log, "'%s' does NOT Implement 'Ability System Interface'",
                Actor ? *Actor->GetName() : *STRING_None);

            lASC = Actor ? Actor->FindComponentByClass<UAbilitySystemComponent>() : nullptr;
        }
#endif // WITH_EDITOR

        Validity = EIsValid(IsValid(lASC));
        return lASC;
    };
    //-------------------------------------------


    /* ---   RTS Attribute Set   --- */

    /** Получить "RTS Attribute Set", используя "RTS Ability System Interface" */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|GAS",
        meta = (DisplayName = "Get RTS Attribute Set", ReturnDisplayName = "Attribute Set"))
    static URTS_AttributeSet* GetRTSAttributeSet(UPARAM(ref) AActor* Actor)
    {
        URTS_AttributeSet* lAttributeSet = nullptr;

        if (const IRTS_AbilitySystemInterface* ASI = Cast<IRTS_AbilitySystemInterface>(Actor))
        {
            lAttributeSet = ASI->GetRTSAttributeSet();
        }

#if WITH_EDITOR
        else
        {
            M_LOG_Static(Log, "'%s' does NOT Implement 'RTS Ability System Interface'",
                Actor ? *Actor->GetName() : *STRING_None);

            EIsValid lValidity = EIsValid::IsNOT_Valid;

            if (UAbilitySystemComponent* lASC = GetValidAbilitySystemComponent(Actor, lValidity))
            {
                for (UAttributeSet* Data : lASC->GetSpawnedAttributes())
                {
                    lAttributeSet = Cast<URTS_AttributeSet>(Data);
                    if (lAttributeSet)
                    {
                        break;
                    }
                }
            }
        }
#endif // WITH_EDITOR

        return lAttributeSet;
    };

    /** Получить "RTS Attribute Set" с проверкой на Валидность, используя "RTS Ability System Interface" */
    UFUNCTION(BlueprintCallable,
        Category = "Utilities|GAS",
        meta = (DisplayName = "Get Valid RTS Attribute Set", ReturnDisplayName = "Attribute Set", ExpandEnumAsExecs = "Validity"))
    static URTS_AttributeSet* GetValidRTSAttributeSet(UPARAM(ref) AActor* Actor, EIsValid& Validity)
    {
        URTS_AttributeSet* lAttributeSet = nullptr;

        if (const IRTS_AbilitySystemInterface* ASI = Cast<IRTS_AbilitySystemInterface>(Actor))
        {
            lAttributeSet = ASI->GetRTSAttributeSet();
        }

#if WITH_EDITOR
        else
        {
            M_LOG_Static(Log, "'%s' does NOT Implement 'RTS Ability System Interface'",
                Actor ? *Actor->GetName() : *STRING_None);

            if (UAbilitySystemComponent* lASC = GetValidAbilitySystemComponent(Actor, Validity))
            {
                for (UAttributeSet* Data : lASC->GetSpawnedAttributes())
                {
                    lAttributeSet = Cast<URTS_AttributeSet>(Data);
                    if (lAttributeSet)
                    {
                        break;
                    }
                }
            }
        }
#endif // WITH_EDITOR

        Validity = EIsValid(IsValid(lAttributeSet));
        return lAttributeSet;
    };
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
