//

// Base:
#include "BlueprintGlobalFunctions.h"

// Global:
#include "RTS/Tools/Global/GlobalMacros.h"
//--------------------------------------------------------------------------------------



/* ---   Blueprint Global Functions   --- */

EIsValid UBlueprintGlobalFunctions::ConvertObjectToClass(UObject* InObject, TSubclassOf<UObject> ObjectClass, UObject*& OutObject)
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
}

EIsValid UBlueprintGlobalFunctions::ConvertActorToClass(AActor* InActor, TSubclassOf<AActor> ActorClass, AActor*& OutActor)
{

#if WITH_EDITOR

    // Отслеживание ошибок в режиме Редактора:
    if (!ActorClass)
    {
        M_Error_Static("'Actor Class' is 'NONE'");
    }
    else if (InActor && !InActor->IsA(ActorClass))
    {
        M_Error_Static("'%s' is NOT class '%s'. Replace this function with 'Cast'",
            *InActor->GetName(), *ActorClass->GetName());
    }
    else

#endif // WITH_EDITOR

    {
        OutActor = InActor;
    }

    return EIsValid(!IsValid(OutActor));
}
//--------------------------------------------------------------------------------------
