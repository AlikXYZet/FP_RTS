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

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объекта и при уверенности его Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InObject -- Объект, требующий конвертации
    @param  ObjectClass -- Класс конвертации

    @return Сконвертируемый Объект */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Object",
        meta = (DisplayName = "Convert Object to Class", ReturnDisplayName = "Out Object",
            ObjectClass = "Actor", DeterminesOutputType = "ObjectClass"))
    static UObject* ConvertObjectToClass(
        UPARAM(ref) UObject* InObject,
        TSubclassOf<UObject> ObjectClass);


    /** Конвертация (преобразование) Объекта в требуемый Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InObject -- Объект, требующий конвертации
    @param  ObjectClass -- Класс конвертации
    @param  OutObject -- Сконвертируемый Объект

    @return Валиден ли Сконвертируемый Объект 'Out Object' */
    UFUNCTION(BlueprintCallable,
        Category = "Utilities|Object",
        meta = (DisplayName = "Convert Valid Object to Class", ExpandEnumAsExecs = "ReturnValue",
            ObjectClass = "Actor", DeterminesOutputType = "ObjectClass", DynamicOutputParam = "OutObject"))
    static EIsValid ConvertValidObjectToClass(
        UPARAM(ref) UObject* InObject,
        TSubclassOf<UObject> ObjectClass,
        UObject*& OutObject);


    /** Конвертация (преобразование) Массива Объектов в требуемый Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объектов и при уверенности их Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InArray -- Массив, требующий конвертации
    @param  ObjectsClass -- Класс конвертации

    @return Сконвертируемый Массив */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Object",
        meta = (DisplayName = "Convert Array to Class", ReturnDisplayName = "Out Array",
            ObjectsClass = "Actor", DeterminesOutputType = "ObjectsClass"))
    static TArray<UObject*> ConvertArrayToClass(
        UPARAM(ref) TArray<UObject*> InArray,
        TSubclassOf<UObject> ObjectsClass)
    {
        return InArray;
    };
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
