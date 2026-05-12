//

// Base:
#include "RTS_PlayerController.h"

// UE:
#include "GameFramework/HUD.h"
#include "GameFramework/InputSettings.h"

// Interaction:
#include "RTS/Units/UnitCharacter.h"
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

// Общедоступный указатель на текущий Локальный Контроллер класса 'ARTS_PlayerController'
ARTS_PlayerController* ARTS_PlayerController::CurrentLocalController = nullptr;
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ARTS_PlayerController::ARTS_PlayerController()
{
    // Установка вызова функции Tick() в каждом кадре.
    PrimaryActorTick.bCanEverTick = true; // Принудительно
    //-------------------------------------------


    /* ---   Mouse   --- */

    // Дистанция подбора предмета (трассировки наведения Мыши)
    HitResultTraceDistance = 280000.f;
    // Включить отображение мыши
    bShowMouseCursor = true;
    // Включить события нажатия мыши и другой клавиши из списка 'Click Event Keys'
    EnableMouseEvents(true);
    // Изменить трассировку с 'ECC_Visibility' для отслеживания взора на объекты
    DefaultClickTraceChannel = ECC_MouseSelection;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void ARTS_PlayerController::BeginPlay()
{
    /* ---   Statics   --- */

    // Общедоступный указатель на текущий Локальный Контроллер класса 'ARTS_PlayerController'

    if (IsLocalController())
    {
        CurrentLocalController = this;
    }
    //-------------------------------------------

    Super::BeginPlay();

    InitMouseControl();
}

void ARTS_PlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    KeepMouseCentered();
}

void ARTS_PlayerController::Destroyed()
{
    /* ---   Statics   --- */

    if (CurrentLocalController == this)
    {
        CurrentLocalController = nullptr;
    }
    //-------------------------------------------

    Super::Destroyed();
}
//--------------------------------------------------------------------------------------



/* ---   Mouse   --- */

void ARTS_PlayerController::InitMouseControl()
{
    if (ULocalPlayer* lLP = Cast<ULocalPlayer>(Player))
    {
        if (lLP->ViewportClient
            && lLP->ViewportClient->Viewport)
        {
            CurrentViewport = lLP->ViewportClient->Viewport;
        }
    }
}

void ARTS_PlayerController::SetMouseToCenter()
{
    if (!IsPaused() && CurrentViewport)
    {
        FIntPoint lSize = CurrentViewport->GetSizeXY();             // Замена перегруженного `GetViewportSize(*,*)`
        FIntPoint lPos;
        CurrentViewport->GetMousePos(lPos);                         // Замена перегруженного `GetMousePosition(*,*)`

        if (lPos.X != int32(lSize.X / 2) || lPos.Y != int32(lSize.Y / 2))
        {
            CurrentViewport->SetMouse(lSize.X / 2, lSize.Y / 2);    // Замена перегруженного `SetMouseLocation(*,*)`
        }
    }
}

FORCEINLINE void ARTS_PlayerController::KeepMouseCentered()
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'
    if (bMouseControlToCenter)
    {
        if (GetPawn())
        {
            SetMouseToCenter();
        }
        else
        {
            // Флаг: Прекратить контроль Мыши
            bMouseControlToCenter = false;
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   Action   --- */

TArray<FName> ARTS_PlayerController::GetActionsGroup(FKey Key)
{
    TArray<FName> ActualGroups;

    if (SelectedActionGroups.Num())
    {
        UInputSettings* InputSettings = UInputSettings::GetInputSettings();
        TArray<FInputActionKeyMapping> lArray;
        lArray.Reserve(4);

        // Пройтись по всем зарегистрированным группам
        for (FName& NameGroup : SelectedActionGroups)
        {
            UInputSettings::GetInputSettings()->GetActionMappingByName(NameGroup, lArray);

            // Поиск соответствующей клавиши
            if (lArray.FindByPredicate([Key](const FInputActionKeyMapping& Item) { return Item.Key == Key; }))
            {
                ActualGroups.Add(NameGroup);
            }

            lArray.Empty(4);
        }
    }

    return ActualGroups;
}
//--------------------------------------------------------------------------------------



/* ---   Unit Selection   --- */

void ARTS_PlayerController::AddUnitToSelectedUnits(AUnitCharacter* Unit)
{
    if (Unit->FractionNumber == FractionNumber)
    {
        SelectedAlliedUnits.Add(Unit);
    }
    else
    {
        if (SelectedEnemyUnits)
            SelectedEnemyUnits->SetSelectedByPlayer(false);
        SelectedEnemyUnits = Unit;
    }
}

void ARTS_PlayerController::RemoveUnitFromSelectedUnits(AUnitCharacter* Unit)
{
    if (Unit->FractionNumber == FractionNumber)
    {
        SelectedAlliedUnits.Remove(Unit);
    }
    else
    {
        if (SelectedEnemyUnits)
            SelectedEnemyUnits->SetSelectedByPlayer(false);
        SelectedEnemyUnits = nullptr;
    }
}

void ARTS_PlayerController::ClearSelectedUnits()
{
    for (auto Unit : SelectedAlliedUnits)
    {
        if (Unit)
        {
            Unit->SetSelectedByPlayer(false);
        }
    }
    SelectedAlliedUnits.Empty();

    if (SelectedEnemyUnits)
        SelectedEnemyUnits->SetSelectedByPlayer(false);
    SelectedEnemyUnits = nullptr;
}
//--------------------------------------------------------------------------------------



/* ===   For EDITOR only   === */

#if WITH_EDITOR

/* ---   Debugs   --- */

void ARTS_PlayerController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Здесь можно написать логику проверки изменённого свойства.
    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(ARTS_PlayerController, SelectedActionGroups))
    {
        InitActionGroup();
    }
};
//--------------------------------------------------------------------------------------



/* ---   Actions   --- */

void ARTS_PlayerController::InitActionGroup()
{
    ClickEventKeys.Empty();

    if (SelectedActionGroups.Num())
    {
        UInputSettings* InputSettings = UInputSettings::GetInputSettings();
        TArray<FInputActionKeyMapping> lArray;

        for (FName& NameGroup : SelectedActionGroups)
        {
            InputSettings->GetActionMappingByName(NameGroup, lArray);

            for (FInputActionKeyMapping& Data : lArray)
            {
                ClickEventKeys.AddUnique(Data.Key);
            }
        }
    }
}

TArray<FName> ARTS_PlayerController::GetActionNames()
{
    UInputSettings* InputSettings = UInputSettings::GetInputSettings();

    TArray<FName> ActionNames;

    InputSettings->GetActionNames(ActionNames);

    return ActionNames;
}
//--------------------------------------------------------------------------------------
#endif
//======================================================================================
