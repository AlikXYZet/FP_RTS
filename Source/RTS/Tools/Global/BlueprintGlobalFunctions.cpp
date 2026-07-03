//

// Base:
#include "BlueprintGlobalFunctions.h"

// Global:
#include "RTS/Tools/Global/GlobalMacros.h"
//--------------------------------------------------------------------------------------



/* ---   UObject   --- */

UObject* UBlueprintGlobalFunctions::ConvertObjectToClass(UObject* InObject, TSubclassOf<UObject> ObjectClass)
{

#if WITH_EDITOR

    // Отслеживание ошибок в режиме Редактора:
    if (!ObjectClass)
    {
        M_Error_Static("'Object Class' is 'NONE'");
    }
    else if (InObject && !InObject->IsA(ObjectClass))
    {
        M_Error_Static("'%s' is NOT class '%s'. Replace this function with 'Cast'",
            *InObject->GetName(), *ObjectClass->GetName());
    }

#endif // WITH_EDITOR

    return InObject;
}

UObject* UBlueprintGlobalFunctions::ConvertValidObjectToClass(UObject* InObject, TSubclassOf<UObject> ObjectClass, EIsValid& Validity)
{

#if WITH_EDITOR

    // Отслеживание ошибок в режиме Редактора:
    if (!ObjectClass)
    {
        M_Error_Static("'Object Class' is 'NONE'");
    }
    else if (InObject && !InObject->IsA(ObjectClass))
    {
        M_Error_Static("'%s' is NOT class '%s'. Replace this function with 'Cast'",
            *InObject->GetName(), *ObjectClass->GetName());
    }

#endif // WITH_EDITOR

    Validity = EIsValid(!IsValid(InObject));

    return InObject;
}
/*
EIsValid UBlueprintGlobalFunctions::ConvertValidObjectToClass(UObject* InObject, TSubclassOf<UObject> ObjectClass, UObject*& OutObject)
{

#if WITH_EDITOR

    // Отслеживание ошибок в режиме Редактора:
    if (!ObjectClass)
    {
        M_Error_Static("'Object Class' is 'NONE'");
    }
    else if (InObject && !InObject->IsA(ObjectClass))
    {
        M_Error_Static("'%s' is NOT class '%s'. Replace this function with 'Cast'",
            *InObject->GetName(), *ObjectClass->GetName());
    }
    else

#endif // WITH_EDITOR

    {
        OutObject = InObject;
    }

    return EIsValid(!IsValid(OutObject));
}*/
//--------------------------------------------------------------------------------------
