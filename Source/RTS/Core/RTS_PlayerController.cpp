//

// Base:
#include "RTS_PlayerController.h"

// Global:
#include "GlobalFunctions.h"

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
//--------------------------------------------------------------------------------------



/* ---   Inputs | Actions   --- */

void ARTS_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    /* ===   Actions   === */

    /* ---   Actions | On-Screens   --- */

    if (ActionGroups_OnScreenSelection != NAME_None)
    {
        InputComponent->BindAction(ActionGroups_OnScreenSelection, IE_Pressed, this, &ARTS_PlayerController::OnScreenSelection);
        //InputComponent->BindAction(ActionGroups_OnScreenSelection, IE_Released, this, &ARTS_PlayerController::OnScreenSelection);
    }

    if (ActionGroups_OnScreenAction != NAME_None)
    {
        InputComponent->BindAction(ActionGroups_OnScreenAction, IE_Pressed, this, &ARTS_PlayerController::OnScreenAction);
        //InputComponent->BindAction(ActionGroups_OnScreenAction, IE_Released, this, &ARTS_PlayerController::OnScreenAction);
    }
    //-------------------------------------------
    //===========================================


    /* ===   Axis   === */

    /* ---   Axis | Test   --- */

    //if (AxisGroups_Test != NAME_None)
    //    InputComponent->BindAxis(AxisGroups_Test, this, &ARTS_PlayerController::Test);
    //-------------------------------------------
    //===========================================


    /* ===   For EDITOR only   === */

#if WITH_EDITOR

    /* ---   Inputs   --- */

    CheckActionGroups({
        ActionGroups_OnScreenSelection,
        ActionGroups_OnScreenAction });

    CheckActionGroups(ActionGroups_OtherScreenInteractions);
    //-------------------------------------------

#endif // WITH_EDITOR
    //===========================================
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
                    if (bHit)
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

void ARTS_PlayerController::OnScreenSelection()
{
    if (AUnitCharacter* lUnit = Cast<AUnitCharacter>(HitResultForActionGroups.Actor))
    {
        if (lUnit->GetGenericTeamId() == TeamID)
        {
            if (lUnit->GetSelectionMode() == EActorSelectionMode::ControlledFriend)
            {
                ISelectableActorInterface::Execute_SetSelectionMode(lUnit, EActorSelectionMode::NotSelected);
                SelectedAlliedUnits.Remove(lUnit);
            }
            else
            {
                ISelectableActorInterface::Execute_SetSelectionMode(lUnit, EActorSelectionMode::ControlledFriend);
                SelectedAlliedUnits.Add(lUnit);
            }

            return;
        }
    }

    // При нажатии на пустую область стираем Массив Выбранных Союзных Юнитов 
    ClearSelectedUnits();
}

void ARTS_PlayerController::OnScreenAction()
{
    if (ISelectableActorInterface::CheckImplementation(HitResultForActionGroups.Actor.Get()))
    {
        SetSelectedTargetActionActor(HitResultForActionGroups.Actor.Get());
    }
    else
    {
        SetSelectedTargetActionActor(nullptr);
    }
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



/* ---   Selectable Actor   --- */

void ARTS_PlayerController::ClearSelectedUnits()
{
    for (auto Unit : SelectedAlliedUnits)
    {
        if (Unit)
        {
            ISelectableActorInterface::Execute_SetSelectionMode(Unit, EActorSelectionMode::NotSelected);
        }
    }
    SelectedAlliedUnits.Empty();

    if (SelectedTargetActionActor)
    {
        ISelectableActorInterface::Execute_SetSelectionMode(SelectedTargetActionActor, EActorSelectionMode::NotSelected);
        SelectedTargetActionActor = nullptr;
    }
}

void ARTS_PlayerController::SetSelectedTargetActionActor(AActor* TargetActor)
{
    if (SelectedTargetActionActor)
    {
        ISelectableActorInterface::Execute_SetSelectionMode(SelectedTargetActionActor, EActorSelectionMode::NotSelected);
    }

    SelectedTargetActionActor = TargetActor;

    if (SelectedTargetActionActor)
    {
        if (IGenericTeamAgentInterface* lGTAI = Cast<IGenericTeamAgentInterface>(SelectedTargetActionActor))
        {
            if (lGTAI->GetGenericTeamId() == TeamID)
            {
                SelectedTargetActionMode = EActorSelectionMode::TargetFriend;
            }
            else if (lGTAI->GetGenericTeamId() != FGenericTeamId::NoTeam)
            {
                SelectedTargetActionMode = EActorSelectionMode::TargetEnemy;
            }
            else
            {
                SelectedTargetActionMode = EActorSelectionMode::TargetNeutral;
            }
        }
        else
        {
            SelectedTargetActionMode = EActorSelectionMode::TargetNeutral;
        }

        ISelectableActorInterface::Execute_SetSelectionMode(SelectedTargetActionActor, SelectedTargetActionMode);
    }
}
//--------------------------------------------------------------------------------------



/* ===   For EDITOR only   === */

#if WITH_EDITOR

/* ---   Debugs   --- */

#define CheckPropertyName(Param) \
{ \
    if(PropertyName == GET_MEMBER_NAME_CHECKED(ARTS_PlayerController, Param)) \
    { \
        CheckAxisGroups({ Param }); \
        UpdateClickEventKeys(); \
    } \
}

void ARTS_PlayerController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property)
    {
        // Здесь можно написать логику проверки изменённого свойства.
        FName PropertyName = PropertyChangedEvent.Property->GetFName();

        CheckPropertyName(ActionGroups_OtherScreenInteractions);
        CheckPropertyName(ActionGroups_OnScreenSelection);
        CheckPropertyName(ActionGroups_OnScreenAction);
    }
};
//--------------------------------------------------------------------------------------



/* ---   Inputs   --- */

void ARTS_PlayerController::UpdateClickEventKeys()
{
    if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
    {
        TArray<FInputActionKeyMapping> lArray;

        if (ActionGroups_OnScreenSelection != NAME_None)
        {
            InputSettings->GetActionMappingByName(ActionGroups_OnScreenSelection, lArray);
        }

        if (ActionGroups_OnScreenAction != NAME_None)
        {
            InputSettings->GetActionMappingByName(ActionGroups_OnScreenAction, lArray);
        }

        for (FName& NameGroup : ActionGroups_OtherScreenInteractions)
        {
            if (NameGroup != NAME_None)
            {
                InputSettings->GetActionMappingByName(NameGroup, lArray);
            }
        }

        ClickEventKeys.Empty();
        for (FInputActionKeyMapping& Data : lArray)
        {
            ClickEventKeys.AddUnique(Data.Key);
        }
    }
}
//--------------------------------------------------------------------------------------
#endif
//======================================================================================
