//

// Base:
#include "RTS_PlayerController.h"

// UE:
#include "GameFramework/HUD.h"
#include "GameFramework/InputSettings.h"

// Interaction:
#include "RTS/Units/UnitCharacter.h"

// Interfaces:
#include "RTS/Tools/Interfaces/Properties/InteractiveInterface.h"
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

bool ARTS_PlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
    // @note    Дублируем, оптимизируем и расширяем код из 'Super::InputKey(*)'

    bool bResult = false;
    if (PlayerInput)
    {
        bResult = PlayerInput->InputKey(Key, EventType, AmountDepressed, bGamepad);
        if (bEnableClickEvents && (ClickEventKeys.Contains(Key) || ClickEventKeys.Contains(EKeys::AnyKey)))
        {
            FVector2D MousePosition;
            UGameViewportClient* ViewportClient = CastChecked<ULocalPlayer>(Player)->ViewportClient;
            if (ViewportClient && ViewportClient->GetMousePosition(MousePosition))
            {
                if (!(GetHUD() && GetHUD()->UpdateAndDispatchHitBoxClickEvents(MousePosition, EventType)))
                {
                    // Вызываем Трассировку при любом воздействии
                    const bool bHit = GetHitResultAtScreenPosition(MousePosition, CurrentClickTraceChannel, true, HitResultForActionGroups);

                    UPrimitiveComponent* ClickedPrimitive = nullptr;
                    if (bEnableMouseOverEvents)
                    {
                        ClickedPrimitive = CurrentClickablePrimitive.Get();
                    }
                    else if (bHit)
                    {
                        ClickedPrimitive = HitResultForActionGroups.Component.Get();
                    }

                    if (ClickedPrimitive)
                    {
                        switch (EventType)
                        {
                        case IE_Pressed:
                        case IE_DoubleClick:
                            ClickedPrimitive->DispatchOnClicked(Key);
                            break;

                        case IE_Released:
                            ClickedPrimitive->DispatchOnReleased(Key);
                            break;

                        case IE_Axis:
                        case IE_Repeat:
                            break;
                        }
                    }
                }

                bResult = true;
            }
        }
    }

    return bResult;
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



/* ---   Selectable Actor   --- */

void ARTS_PlayerController::AddUnitToSelectedUnits(AActor* Unit)
{
    if (Cast<AUnitCharacter>(Unit)
        && ((AUnitCharacter*)Unit)->FractionNumber == FractionNumber)
    {
        SelectedAlliedUnits.Add((AUnitCharacter*)Unit);
    }
    else
    {
        if (ISelectableActorInterface::CheckImplementation(SelectedActor))
            ISelectableActorInterface::Execute_SetSelectedByPlayer(SelectedActor, false);
        SelectedActor = Unit;
    }
}

void ARTS_PlayerController::RemoveUnitFromSelectedUnits(AActor* Unit)
{
    if (Cast<AUnitCharacter>(Unit)
        && ((AUnitCharacter*)Unit)->FractionNumber == FractionNumber)
    {
        SelectedAlliedUnits.Remove((AUnitCharacter*)Unit);
    }
    else
    {
        if (ISelectableActorInterface::CheckImplementation(SelectedActor))
            ISelectableActorInterface::Execute_SetSelectedByPlayer(SelectedActor, false);
        SelectedActor = nullptr;
    }
}

void ARTS_PlayerController::ClearSelectedUnits()
{
    for (auto Unit : SelectedAlliedUnits)
    {
        if (Unit)
        {
            ISelectableActorInterface::Execute_SetSelectedByPlayer(Unit, false);
        }
    }
    SelectedAlliedUnits.Empty();

    if (ISelectableActorInterface::CheckImplementation(SelectedActor))
        ISelectableActorInterface::Execute_SetSelectedByPlayer(SelectedActor, false);
    SelectedActor = nullptr;
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
