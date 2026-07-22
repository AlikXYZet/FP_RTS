//

#pragma once

// Core:
#include "Kismet/BlueprintFunctionLibrary.h"

// Global:
#include "GlobalMacros.h"

// Enums:
#include "RTS/Tools/Enums/IsValid.h"

// Generated:
#include "BlueprintGlobalFunctions.generated.h"
//--------------------------------------------------------------------------------------



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
    @param  In -- Value to set the FText to
    @return The literal FName */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Name",
        meta = (BlueprintThreadSafe, ReturnDisplayName = "Out"))
    static FName MakeLiteralNameFromText(FText In)
    {
        return FName(In.ToString());
    };
    //-------------------------------------------



    /* ---   UObject   --- */

    /** Конвертация (преобразование) Объекта в требуемый Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объекта и при уверенности его Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InObject -- Объект, требующий конвертации

    @return Сконвертируемый Объект */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Object",
        meta = (ReturnDisplayName = "Out Object", CustomStructureParam = "ReturnValue"))
    static UObject* ConvertObjectToClass(
        UPARAM(ref) UObject* InObject)
    {
        return InObject;
    }

    /** Конвертация (преобразование) Объекта в требуемый явно выбранный Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объекта и при уверенности его Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InObject -- Объект, требующий конвертации
    @param  ObjectClass -- Класс конвертации

    @return Сконвертируемый Объект */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Object",
        meta = (ObjectClass = "Actor", ReturnDisplayName = "Out Object", DeterminesOutputType = "ObjectClass"))
    static UObject* ConvertObjectToSelectedClass(
        UPARAM(ref) UObject* InObject,
        TSubclassOf<UObject> ObjectClass)
    {
#if WITH_EDITOR
        // Отслеживание ошибок в режиме Редактора:
        if (InObject && !InObject->IsA(ObjectClass))
        {
            M_Error_Static("'%s' is NOT class '%s'. Replace this function with 'Cast'",
                *InObject->GetName(), *ObjectClass->GetName());
        }
#endif // WITH_EDITOR
        return InObject;
    };

    /** Конвертация (преобразование) Объекта в требуемый явно выбранный Класс с проверкой на Валидность

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InObject -- Объект, требующий конвертации
    @param  ObjectClass -- Класс конвертации
    @param  Validity -- Валиден ли Сконвертируемый Объект

    @return Сконвертируемый Объект */
    UFUNCTION(BlueprintCallable,
        Category = "Utilities|Object",
        meta = (ObjectClass = "Actor", ExpandEnumAsExecs = "Validity",
            ReturnDisplayName = "Out Object", DeterminesOutputType = "ObjectClass"))
    static UObject* ConvertValidObjectToClass(
        UPARAM(ref) UObject* InObject,
        TSubclassOf<UObject> ObjectClass,
        EIsValid& Validity)
    {
#if WITH_EDITOR
        // Отслеживание ошибок в режиме Редактора:
        if (InObject && !InObject->IsA(ObjectClass))
        {
            M_Error_Static("'%s' is NOT class '%s'. Replace this function with 'Cast'",
                *InObject->GetName(), *ObjectClass->GetName());
        }
#endif // WITH_EDITOR
        Validity = EIsValid(IsValid(InObject));
        return InObject;
    };
    //-------------------------------------------



    /* ---   TArray < UObject* >   --- */

    /** Конвертация (преобразование) Массива Объектов в требуемый Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объектов и при уверенности их Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InArray -- Массив, требующий конвертации

    @return Сконвертируемый Массив */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Array",
        meta = (ReturnDisplayName = "Out Array",
            ArrayParm = "ReturnValue"))
    static TArray<UObject*>& ConvertArrayToClass(
        UPARAM(ref) TArray<UObject*>& InArray)
    {
        return InArray;
    };

    /** Конвертация (преобразование) Массива Объектов в требуемый явно выбранный Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объектов и при уверенности их Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InArray -- Массив, требующий конвертации
    @param  ObjectsClass -- Класс конвертации

    @return Сконвертируемый Массив */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Array",
        meta = (ReturnDisplayName = "Out Array",
            ObjectsClass = "Actor", DeterminesOutputType = "ObjectsClass"))
    static TArray<UObject*>& ConvertArrayToSelectedClass(
        UPARAM(ref) TArray<UObject*>& InArray,
        TSubclassOf<UObject> ObjectsClass)
    {
        return InArray;
    };
    //-------------------------------------------



    /* ---   TSet < UObject* >   --- */

    /** Конвертация (преобразование) Набора Объектов в требуемый Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объектов и при уверенности их Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InSet -- Набор, требующий конвертации

    @return Сконвертируемый Набор */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Set",
        meta = (ReturnDisplayName = "Out Set",
            SetParam = "ReturnValue"))
    static TSet<UObject*>& ConvertSetToClass(
        UPARAM(ref) TSet<UObject*>& InSet)
    {
        return InSet;
    };

    /** Конвертация (преобразование) Набора Объектов в требуемый явно выбранный Класс

    @note   Используется как замена 'Cast' и Интерфейса при явно известном Классе Объектов и при уверенности их Валидности.
    В противном случае, может вызвать Ошибки использования памяти.

    @param  InSet -- Набор, требующий конвертации
    @param  ObjectsClass -- Класс конвертации

    @return Сконвертируемый Набор */
    UFUNCTION(BlueprintPure,
        Category = "Utilities|Set",
        meta = (ReturnDisplayName = "Out Set",
            ObjectsClass = "Actor", DeterminesOutputType = "ObjectsClass"))
    static TSet<UObject*>& ConvertSetToSelectedClass(
        UPARAM(ref) TSet<UObject*>& InSet,
        TSubclassOf<UObject> ObjectsClass)
    {
        return InSet;
    };
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
