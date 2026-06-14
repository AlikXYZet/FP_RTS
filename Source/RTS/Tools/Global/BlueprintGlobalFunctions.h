//

#pragma once

// Core:
#include "Kismet/BlueprintFunctionLibrary.h"

// Generated:
#include "BlueprintGlobalFunctions.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Enum   --- */

UENUM()
enum struct EIsValid : uint8
{
    IsValid,
    IsNOT_Valid,
};
//-------------------------------------------



/* ---   Blueprint Global Functions   --- */

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "GlobalFunctions"))
class RTS_API UBlueprintGlobalFunctions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    /* ---   FText   --- */

    /** Converts localizable FText to a FName value */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Text",
        meta = (DisplayName = "ToName (text)", CompactNodeTitle = "->", BlueprintAutocast))
    static FName Conv_TextToName(const FText& InText)
    {
        return FName(InText.ToString());
    };

    /** Creates a literal FName from FText
    @param  Value -- Value to set the FText to
    @return The literal FName */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Name",
        meta = (BlueprintThreadSafe))
    static FName MakeLiteralNameFromText(FText Value)
    {
        return FName(Value.ToString());
    };
    //-------------------------------------------



    /* ---   UObject   --- */

    /** Конвертация (преобразование) Объекта в требуемый Класс

    @note   Используется как замена 'Cast' при явно известном Классе.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InObject -- Объект, требующий конвертации
    @param  ObjectClass -- Класс конвертации
    @param  OutObject -- Сконвертируемый Объект

    @return Валиден ли Сконвертируемый Объект 'Out Object' */
    UFUNCTION(BlueprintCallable,
        Category = "Utilities|Object",
        meta = (DisplayName = "Convert Object to Class", ExpandEnumAsExecs = "ReturnValue",
            ObjectClass = "Actor", DeterminesOutputType = "ObjectClass", DynamicOutputParam = "OutObject"))
    static EIsValid ConvertObjectToClass(
        UPARAM(ref) UObject* InObject,
        TSubclassOf<UObject> ObjectClass,
        UObject*& OutObject);
    //-------------------------------------------



    /* ---   AActor   --- */

    /** Конвертация (преобразование) Актора в требуемый Класс

    @note   Необходим как замена 'Cast' при частом использовании, но может вызвать Ошибки использования памяти.
    Следует использовать, если 'In Actor' явно имеет класс, указанный в 'Actor Class' или дочерний ему.

    @param  InActor -- Актор, требующий конвертации
    @param  ActorClass -- Класс конвертации
    @param  OutActor -- Сконвертируемый Актор

    @return Валиден ли Сконвертируемый Актор 'Out Actor' */
    UFUNCTION(BlueprintCallable,
        Category = "Utilities|Actor",
        meta = (DisplayName = "Convert Actor to Class", ExpandEnumAsExecs = "ReturnValue",
            ActorClass = "Actor", DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActor"))
    static EIsValid ConvertActorToClass(
        UPARAM(ref) AActor* InActor,
        TSubclassOf<AActor> ActorClass,
        AActor*& OutActor);
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
