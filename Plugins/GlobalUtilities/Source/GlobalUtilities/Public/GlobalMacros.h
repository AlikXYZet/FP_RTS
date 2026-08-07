#pragma once

// Core:
#include "CoreMinimal.h"

/* Пример:

// Global:
#include "GlobalMacros.h"

*/
//--------------------------------------------------------------------------------------



/* ---   Init LogGUP   --- */

GLOBALUTILITIES_API DECLARE_LOG_CATEGORY_EXTERN(LogGUP, All, All);
//--------------------------------------------------------------------------------------



/* ---   Values   --- */

#define ECC_Pickables       ECC_GameTraceChannel1   // Канал трассировки "Pickables" ("Подбираемые предметы")
#define ECC_MouseSelection  ECC_GameTraceChannel2   // Канал трассировки "Mouse Selection" ("Выбор Мыши")
#define ECC_WorldZone       ECC_GameTraceChannel3   // Канал трассировки "World Zone" ("Мировая Зона", она же Граница карты)

#define ProfileName_Destructible            TEXT("Destructible")
#define ProfileName_InvisibleWallDynamic    TEXT("InvisibleWallDynamic")
#define ProfileName_Trigger                 TEXT("Trigger")
#define ProfileName_InvisibleWall           TEXT("InvisibleWall")
#define ProfileName_Pickables               TEXT("Pickables")
#define ProfileName_Spectator               TEXT("Spectator")
#define ProfileName_PhysicsActor            TEXT("PhysicsActor")

// Соответствие проверено опытным путём:
#define EOT_Pawn            EObjectTypeQuery::ObjectTypeQuery3 // [2]: for ECollisionChannel::ECC_Pawn, [2]
#define EOT_Vehicle         EObjectTypeQuery::ObjectTypeQuery5 // [4]: for ECollisionChannel::ECC_Vehicle, [6]
#define EOT_Destructible    EObjectTypeQuery::ObjectTypeQuery6 // [5]: for ECollisionChannel::ECC_Destructible, [7]
/* PS: Следующий метод инициализации данных вызывает блокировку запуска любого Билда без какого-либо уведомления:
// Массив Типов Объектов, что отслеживаются Hitscan-методом
const TArray<TEnumAsByte<EObjectTypeQuery>> UWeaponNetworkController::ObjectTypesForHitscan
{
    UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn),
    UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle),
    UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Destructible)
} */

#define STRING_None FString("NONE")
//--------------------------------------------------------------------------------------



/* ---   ENums   --- */

/** Операторы сравнения для Enum */
#define ENUM_COMPARISON_OPERATORS(Enum) \
    inline constexpr bool operator  >   (Enum Lhs, Enum Rhs)    { return ((__underlying_type(Enum))Lhs  >   (__underlying_type(Enum))Rhs); } \
    inline constexpr bool operator  >=  (Enum Lhs, Enum Rhs)    { return ((__underlying_type(Enum))Lhs  >=  (__underlying_type(Enum))Rhs); } \
    inline constexpr bool operator  <   (Enum Lhs, Enum Rhs)    { return ((__underlying_type(Enum))Lhs  <   (__underlying_type(Enum))Rhs); } \
    inline constexpr bool operator  <=  (Enum Lhs, Enum Rhs)    { return ((__underlying_type(Enum))Lhs  <=  (__underlying_type(Enum))Rhs); } \
\
    inline constexpr bool operator  >   (Enum Lhs, __underlying_type(Enum) Rhs) { return ((__underlying_type(Enum))Lhs  >   Rhs); } \
    inline constexpr bool operator  >=  (Enum Lhs, __underlying_type(Enum) Rhs) { return ((__underlying_type(Enum))Lhs  >=  Rhs); } \
    inline constexpr bool operator  <   (Enum Lhs, __underlying_type(Enum) Rhs) { return ((__underlying_type(Enum))Lhs  <   Rhs); } \
    inline constexpr bool operator  <=  (Enum Lhs, __underlying_type(Enum) Rhs) { return ((__underlying_type(Enum))Lhs  <=  Rhs); } \
    inline constexpr bool operator  ==  (Enum Lhs, __underlying_type(Enum) Rhs) { return ((__underlying_type(Enum))Lhs  ==  Rhs); } \
    inline constexpr bool operator  !=  (Enum Lhs, __underlying_type(Enum) Rhs) { return ((__underlying_type(Enum))Lhs  !=  Rhs); } \
\
    inline constexpr bool operator  >   (uint8 Lhs, Enum Rhs) { return (Lhs  >   (uint8)Rhs); } \
    inline constexpr bool operator  >=  (uint8 Lhs, Enum Rhs) { return (Lhs  >=  (uint8)Rhs); } \
    inline constexpr bool operator  <   (uint8 Lhs, Enum Rhs) { return (Lhs  <   (uint8)Rhs); } \
    inline constexpr bool operator  <=  (uint8 Lhs, Enum Rhs) { return (Lhs  <=  (uint8)Rhs); } \
    inline constexpr bool operator  ==  (uint8 Lhs, Enum Rhs) { return (Lhs  ==  (uint8)Rhs); } \
    inline constexpr bool operator  !=  (uint8 Lhs, Enum Rhs) { return (Lhs  !=  (uint8)Rhs); }
//--------------------------------------------------------------------------------------



/* ---   Main LOG   --- */

/** Макрос, который выводит отформатированное сообщение в журнал под категорией 'LogGUP'

@param  Verbosity - Уровень информации (Error, Warning и др.)
@param  Format - Формат текста */
#define M_LOG_Empty(Verbosity, Format, ...) UE_LOG(LogGUP, Verbosity, TEXT("" Format), ##__VA_ARGS__)

/** Макрос, который выводит отформатированное сообщение в журнал под категорией 'LogGUP'
    и дополнительную информацию о экземпляре класса и методе класса, где было вызвано сообщение

@param  Verbosity - Уровень информации (Error, Warning и др.)
@param  Format - Формат текста

@note   Не используем '*FString(__FUNCTION__)', так как требуется знать экземпляр класса */
#define M_LOG(Verbosity, Format, ...) \
{ \
	M_LOG_Empty(Verbosity, \
        TEXT("'%s'::'%s': " Format), \
        *GetNameSafe(this), *FString(__func__),\
        ##__VA_ARGS__); \
}

/** Макрос, который выводит отформатированное сообщение в журнал под категорией 'LogGUP'
    и дополнительную информацию о структуре и его методе, где было вызвано сообщение

@param  Verbosity - Уровень информации (Error, Warning и др.)
@param  Format - Формат текста

@note   Используем '*FString(__FUNCTION__)', так как экземпляр класса или структуры может не существовать */
#define M_LOG_Static(Verbosity, Format, ...) \
{ \
	M_LOG_Empty(Verbosity, \
        TEXT("'%s': " Format), \
        *FString(__FUNCTION__),\
        ##__VA_ARGS__); \
}

/** Макрос, который выводит отформатированное сообщение в журнал под категорией 'LogGUP'
    и дополнительную информацию о экземплярах класса-владельца и текущего класса-компонента,
    а также о методе класса, где было вызвано сообщение

@param  Verbosity - Уровень информации (Error, Warning и др.)
@param  Format - Формат текста

@note   Не используем '*FString(__FUNCTION__)', так как требуется знать экземпляр класса */
#define M_LOG_Component(Verbosity, Format, ...) \
{ \
	M_LOG_Empty(Verbosity, \
        TEXT("'%s'::'%s'::'%s': " Format), \
        *GetNameSafe(GetOwner()), *GetNameSafe(this), *FString(__func__), \
        ##__VA_ARGS__); \
}

/** Примеры использования:

M_LOG_Empty(Warning, "CurrentWeaponData is NOT");

M_LOG(Error, "CurrentWeaponData is NOT");

M_LOG(Error, "lNewSpeed == %s",
    *lNewSpeed.ToString());

M_LOG(Warning, "'%s' is NOT 'ARTS_GameState'",
    GetWorld()->GetGameState<AGameStateBase>()
    ? *GetWorld()->GetGameState<AGameStateBase>()->GetName()
    : *STRING_None);

M_LOG_Static(Error, "lInputComponent is NOT");

*/

//--------------------------------------------------------------------------------------



/* ---   Main GEngine Message   --- */

/** Макрос, который выводит на экран сообщение с указанным цветом

@param  Color - Цвет текста
@param  Format - Формат текста */
#define M_ColorMessage_Empty(Color, Format, ...) \
{ \
    GEngine->AddOnScreenDebugMessage(-1, 600.f, Color, FString::Printf(TEXT("" Format), ##__VA_ARGS__)); \
}

/** Макрос, который выводит на экран сообщение (красным цветом)

@param  Format - Формат текста */
#define M_Message_Empty(Format, ...) M_ColorMessage_Empty(FColor::Red, Format, ##__VA_ARGS__)


/** Макрос, который выводит на экран сообщение с указанным цветом
    и дополнительную информацию о экземпляре класса и методе класса, где было вызвано сообщение

@param  Color - Цвет текста
@param  Format - Формат текста */
#define M_ColorMessage(Color, Format, ...) \
    M_ColorMessage_Empty(\
        Color,\
        TEXT("'%s'::'%s': " Format), \
        *GetNameSafe(this), *FString(__func__), \
        ##__VA_ARGS__)

/** Макрос, который выводит на экран сообщение (красным цветом)
    и дополнительную информацию о экземпляре класса и методе класса, где было вызвано сообщение
@param  Format - Формат текста */
#define M_Message(Format, ...) M_ColorMessage(FColor::Red, Format, ##__VA_ARGS__)

/** Макрос, который выводит на экран сообщение с указанным цветом
    и дополнительную информацию о классе и методе класса, где было вызвано сообщение

@param  Color - Цвет текста
@param  Format - Формат текста

@note   Используем '*FString(__FUNCTION__)', так как экземпляр класса или структуры может не существовать */
#define M_ColorMessage_Static(Color, Format, ...) \
    M_ColorMessage_Empty(\
        Color,\
        TEXT("'%s': " Format), \
        *FString(__FUNCTION__),\
        ##__VA_ARGS__)

/** Макрос, который выводит на экран сообщение (красным цветом)
    и дополнительную информацию о классе и методе класса, где было вызвано сообщение

@param  Format - Формат текста */
#define M_Message_Static(Format, ...) M_ColorMessage_Static(FColor::Red, Format, ##__VA_ARGS__)

/** Макрос, который выводит на экран сообщение с указанным цветом
    и дополнительную информацию о экземплярах класса-владельца и текущего класса-компонента,
    а также о методе класса, где было вызвано сообщение

@param  Color - Цвет текста
@param  Format - Формат текста

@note   Не используем '*FString(__FUNCTION__)', так как требуется знать экземпляр класса */
#define M_ColorMessage_Component(Color, Format, ...) \
{ \
    M_ColorMessage_Empty(\
        Color,\
        TEXT("'%s'::'%s'::'%s': " Format), \
        *GetNameSafe(GetOwner()), *GetNameSafe(this), *FString(__func__), \
        ##__VA_ARGS__); \
}

/** Макрос, который выводит на экран сообщение (красным цветом)
    и дополнительную информацию о экземплярах класса-владельца и текущего класса-компонента,
    а также о методе класса, где было вызвано сообщение

@param  Format - Формат текста */
#define M_Message_Component(Format, ...) M_ColorMessage_Component(FColor::Red, Format, ##__VA_ARGS__)
//--------------------------------------------------------------------------------------



/* ---   Main Errors Message   --- */

/** Макрос вывода сообщения в журнал под категорией 'LogGUP' и на экран (красным цветом)

@param  Format - Формат текста */
#define M_Error_Empty(Format, ...) \
    M_LOG_Empty(Error, Format, ##__VA_ARGS__); \
    M_Message_Empty(Format, ##__VA_ARGS__);

/** Макрос вывода сообщения в журнал под категорией 'LogGUP' и на экран (красным цветом)
    с дополнительной информацией о экземпляре класса и методе класса, где было вызвано сообщение

@param  Format - Формат текста */
#define M_Error(Format, ...) \
{ \
	M_Error_Empty(\
        TEXT("'%s'::'%s': " Format), \
        *GetNameSafe(this), *FString(__func__),\
        ##__VA_ARGS__); \
}

/** Макрос вывода сообщения в журнал под категорией 'LogGUP' и на экран (красным цветом)
    и дополнительную информацию о структуре и его методе, где было вызвано сообщение

@param  Format - Формат текста

@note   Используется '*FString(__FUNCTION__)', так как экземпляр класса или структуры может не существовать */
#define M_Error_Static(Format, ...) \
{ \
	M_Error_Empty(\
        TEXT("'%s': " Format), \
        *FString(__FUNCTION__),\
        ##__VA_ARGS__); \
}

/** Макрос вывода сообщения в журнал под категорией 'LogGUP' и на экран (красным цветом)
    с дополнительной информацией о экземплярах класса-владельца и текущего класса-компонента,
    а также о методе класса, где было вызвано сообщение

@param  Format - Формат текста */
#define M_Error_Component(Format, ...) \
{ \
	M_Error_Empty(\
        TEXT("'%s'::'%s'::'%s': " Format), \
        *GetNameSafe(GetOwner()), *GetNameSafe(this), *FString(__func__), \
        ##__VA_ARGS__); \
}
//--------------------------------------------------------------------------------------



/* ---   Sorting Predicate   --- */

/** Макрос: Предикат сортировки
@param  Value - Переменная класса или структуры, значение которого используется для сортировки
@param  Operator - Оператор сортировки */
#define SORTING_PREDICATE(Value, Operator) \
    [](const auto& first, const auto& second) \
    { \
        return \
            first.##Value \
            ##Operator## \
            second.##Value; \
    };

/** Макрос: Предикат сортировки по Указателю
@param  Ptr - Переменная класса или структуры, что является Указателем
@param  Value - Переменная из Указателя, значение которого используется для сортировки
@param  Operator - Оператор сортировки */
#define SORTING_PREDICATE_ByPtr(Ptr, Value, NoneValue, Operator) \
    [](const auto& first, const auto& second) \
    { \
        return \
            (IsValid(first.##Ptr) ? first.##Ptr->##Value : ##NoneValue) \
            ##Operator## \
            (IsValid(second.##Ptr) ? second.##Ptr->##Value : ##NoneValue); \
    };
//--------------------------------------------------------------------------------------



/* ---   Booleanas   --- */

/** Макрос: Преобразование 'bool' в указатель 'FString' для использования в макросах 'LOG' и 'Message' */
#define BoolToString(BOOL) *FString(BOOL ? "'TRUE'" : "'FALSE'")
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

/** Установить статическое Значение Структуры

@param  FStruct -- Структура, что имеет своё значение в виде переменной, отмеченной как 'static'
@param  Variable -- Переменная, требующая инициализации в файле '*.h'
@param  Value -- Требуемое значение переменной

@note   Используется, если требуется задать значение в пределах Заголовочного Файла '*.h'.
        Например: Если НЕ создан Файл реализации '.cpp'
*/
#define SET_STATIC_VALUE(FStruct, Variable, Value) __declspec(selectany) const FStruct FStruct::Variable = Value;

/** Установить пустое Значение Структуры для переменной 'Empty' этой же Структуры

@param  FStruct -- Структура, что имеет своё значение в виде статической переменной 'Empty'

@note   Используется, если требуется задать значение в пределах Заголовочного Файла '*.h'.
        Например: Если НЕ создан Файл реализации '.cpp'
*/
#define SET_STATIC_EMPTY(FStruct) SET_STATIC_VALUE(FStruct, Empty, FStruct())
//--------------------------------------------------------------------------------------



/* ===   Does NOT work via Macros   === */

/* ---   'Get Options' | Inputs   --- */

/** Доступ для 'GetOptions':

    - Получить Наименования групп ввода: Действия ('Action')
    "GlobalUtilities.BlueprintGlobalFunctions.GetActionGroupsNames"

    - Получить Наименования групп ввода: Оси ('Axis')
    "GlobalUtilities.BlueprintGlobalFunctions.GetAxisGroupsNames"

*/
//--------------------------------------------------------------------------------------
//======================================================================================



/* ===   Примечание   === */

/** Используемые форматы:
@see    Engine/Source/Runtime/Core/Public/Templates/UnrealTypeTraits.h
or      #include "Templates/UnrealTypeTraits.h"

// Базовые:
Expose_TFormatSpecifier(bool, "%i")     // Не работает для `bool`
Expose_TFormatSpecifier(uint8, "%u")
Expose_TFormatSpecifier(uint16, "%u")
Expose_TFormatSpecifier(uint32, "%u")
Expose_TFormatSpecifier(uint64, "%llu") // Подходит и для указателей
Expose_TFormatSpecifier(int8, "%d")
Expose_TFormatSpecifier(int16, "%d")
Expose_TFormatSpecifier(int32, "%d")
Expose_TFormatSpecifier(int64, "%lld")
Expose_TFormatSpecifier(float, "%f")
Expose_TFormatSpecifier(double, "%f")
Expose_TFormatSpecifier(long double, "%f")
Expose_TFormatSpecifier(long, "%ld")
Expose_TFormatSpecifier(unsigned long, "%lu")
*/

/** Конвертация единиц измерения:
@see    Engine/Source/Runtime/Core/Public/Math/UnitConversion.h
or      #include "Math/UnitConversion.h"
*/
//======================================================================================
