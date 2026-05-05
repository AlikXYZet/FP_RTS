//

// Base:
#include "RTS_PlayerController.h"

// UE:
#include "GameFramework/HUD.h"
#include "GameFramework/InputSettings.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ARTS_PlayerController::ARTS_PlayerController()
{
    // Установка вызова функции Tick() в каждом кадре.
    PrimaryActorTick.bCanEverTick = true; // Принудительно
    //-------------------------------------------


    /* ---   Role Selection   --- */

    // Дистанция подбора предмета (трассировки наведения Мыши)
    HitResultTraceDistance = 280000.f;
    EnableMouseEvents(true);

    /** "Наблюдатель" по умолчанию
    @note   Заменяем `PC->StartSpectatingOnly();`, так как в данный момент:
    * `StateName == NAME_Spectating`;
    * `PlayerState == nullptr`. */
    bPlayerIsWaiting = false;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

//void ARTS_PlayerController::BeginPlay()
//{
//    Super::BeginPlay();
//
//}

void ARTS_PlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SetMouseToCenter();
}
//--------------------------------------------------------------------------------------



/* ---   Mouse   --- */

FORCEINLINE void ARTS_PlayerController::SetMouseToCenter()
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'
    if (bMouseToCenter)
    {
        if (GetPawn())
        {
            if (!IsPaused()
                && GetMousePosition(MousePositionX, MousePositionY))
            {
                GetViewportSize(SizeCenterX, SizeCenterY);

                if (MousePositionX != int32(SizeCenterX / 2) || MousePositionY != int32(SizeCenterY / 2))
                {
                    SetMouseLocation(SizeCenterX / 2, SizeCenterY / 2);
                }
            }
        }
        else
        {
            // Флаг: Прекратить контроль Мыши
            bMouseToCenter = false;
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
