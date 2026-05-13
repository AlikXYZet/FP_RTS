//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "Components/ActorComponent.h"

// Structs:
#include "RTS/Tools/Structs/Properties/ComponentRendering.h"

// Generated:
#include "InteractiveComponent.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
class APlayerCharacter;
class IInteractiveInterface;
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат: При Локальном срабатывании действия на Актор
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerWasClicked, const FKey&, ButtonReleased);
//--------------------------------------------------------------------------------------



// Компонент интерактивности для подсвечивания Актора через изменение трафаретного буфера (Stencil Buffer) рендеринга
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RTS_API UInteractiveComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    /* Делегат: При срабатывании действия на Актор */
    UPROPERTY(BlueprintAssignable)
    FOnOwnerWasClicked OnOwnerWasClicked;
    //-------------------------------------------



    /* ---   Constructors   --- */

    // Sets default values for this component's properties
    UInteractiveComponent();
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Инициализация компонента до вызова "BeginPlay()" */
    virtual void InitializeComponent() override;

    /** Called when the game starts */
    virtual void BeginPlay() override;
    //-------------------------------------------



    /* ---   Highlighting   --- */

    /** Список используемых Компонентов с указанием глубины Выделения
    @warning    Присваивание значений следует проводить в BeginPlay() до вызова Super::BeginPlay()
                Иначе: Повторно вызывать 'InitHighlightedComponents()' для подготовки Компонентов */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Interactive|Highlighting",
        meta = (TitleProperty = "Component"))
    TArray<FComponentRendering> HighlightedComponents;
    //-------------------------------------------



    /* ---   Actions   --- */

    /** Контейнер с быстрым поиском Клавиши
    @note   Автоматически заполняется в Редакторе при изменении переменной 'Selected Action Groups' */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Interactive|Actions",
        meta = (DisplayAfter = "SelectedActionGroups"))
    TSet<FKey> ActionKeys;

    //

    /** Событие нажатия на Актора */
    UFUNCTION(BlueprintCallable,
        Category = "Interactive|Actions")
    void OwnerWasClicked(AActor* TouchedActor, FKey ButtonReleased);
    //-------------------------------------------



private:

    /* ---   Highlighting   --- */

    /** Событие выделения Актора при наведении */
    UFUNCTION()
    void CursorWasBeginOverOwner(AActor* TouchedActor);

    /** Событие прекращения выделения Актора при прекращении наведения */
    UFUNCTION()
    void CursorWasEndFromOwner(AActor* TouchedActor);

    /** Предварительная инициализация используемых Компонентов */
    UFUNCTION()
    void InitHighlightedComponents();
    //-------------------------------------------



    /* ---   Actions   --- */

    /** Предварительная инициализация используемых Клавиш */
    void InitActionGroup();
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITORONLY_DATA

private:

    /* ---   Actions   --- */

    /** Выбранные группы Действий */
    UPROPERTY(EditDefaultsOnly,
        Category = "Interactive|Actions",
        meta = (GetOptions = "GetActionGroupsNames"))
    FName SelectedActionGroups = NAME_None;
    //-------------------------------------------

#endif // WITH_EDITORONLY_DATA


#if WITH_EDITOR

public:

    /* ---   Base: Debugs   --- */

    /** Вызывается, когда какое-либо свойство этого объекта было изменено
    @note   Используется для проверки изменённых переменных */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    /** Альтернативная версия PostEditChange, вызываемая при изменении свойств внутри структур */
    //virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
    //-------------------------------------------



private:

    /* ---   Actions   --- */

    /* Получить имена всех Функций-Предикатов Актора-Владельца */
    UFUNCTION()
    TArray<FName> GetActionGroupsNames() const;

    /* Повторная инициализация используемых Клавиш */
    void ReInitActionGroup();
    //-------------------------------------------

#endif // WITH_EDITOR
    //===========================================
};
