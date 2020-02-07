#include "UI/MyNavigationConfig.h"


FUINavigationKey::FUINavigationKey(EUIKeyControlType inControlType, EUINavigation inUINavigation)
	: ControlType(inControlType)
	, UINavigation(inUINavigation)
{

}

bool FNavigationConfigBase::IsProcessRepeatKeyEvent()
{
	return bProcessRepeatKeyEvent;
}

FNavigationDirectionConfig::FNavigationDirectionConfig()
{
	bProcessRepeatKeyEvent = true;
	bTabNavigation = false;
	bKeyNavigation = true;
	bAnalogNavigation = true;
	AnalogNavigationHorizontalThreshold = 0.40f;
	AnalogNavigationVerticalThreshold = 0.50f;

	KeyEventRules.Emplace(EKeys::A, EUINavigation::Left);
	KeyEventRules.Emplace(EKeys::D, EUINavigation::Right);
	KeyEventRules.Emplace(EKeys::W, EUINavigation::Up);
	KeyEventRules.Emplace(EKeys::S, EUINavigation::Down);
}

FNavigationPageConfig::FNavigationPageConfig()
{
	bProcessRepeatKeyEvent = false;
	bTabNavigation = false;
	bKeyNavigation = true;
	bAnalogNavigation = false;

	KeyEventRules.Empty();

	KeyEventRules.Emplace(EKeys::PageUp, EUINavigation::Left);
	KeyEventRules.Emplace(EKeys::Gamepad_LeftShoulder, EUINavigation::Left);

	KeyEventRules.Emplace(EKeys::PageDown, EUINavigation::Right);
	KeyEventRules.Emplace(EKeys::Gamepad_RightShoulder, EUINavigation::Right);
}

FNavigationViewActorConfig::FNavigationViewActorConfig()
{
	bProcessRepeatKeyEvent = true;
	bTabNavigation = false;
	bKeyNavigation = false;
	bAnalogNavigation = true;

	AnalogHorizontalKey = EKeys::Gamepad_RightX;
	AnalogVerticalKey = EKeys::Gamepad_RightY;
}

FNavigationConformConfig::FNavigationConformConfig()
{
	bProcessRepeatKeyEvent = false;
	bTabNavigation = false;
	bKeyNavigation = true;
	bAnalogNavigation = false;

	KeyEventRules.Empty();

	KeyEventRules.Emplace(EKeys::E, EUINavigation::Next);
	KeyEventRules.Emplace(EKeys::Enter, EUINavigation::Next);
	KeyEventRules.Emplace(EKeys::SpaceBar, EUINavigation::Next);
	KeyEventRules.Emplace(EKeys::Virtual_Accept, EUINavigation::Next);

	KeyEventRules.Emplace(EKeys::Escape, EUINavigation::Previous);
	KeyEventRules.Emplace(EKeys::Virtual_Back, EUINavigation::Previous);
	KeyEventRules.Emplace(EKeys::RightMouseButton, EUINavigation::Previous);

	KeyEventRules.Emplace(EKeys::Z, EUINavigation::Left);
	KeyEventRules.Emplace(EKeys::Gamepad_FaceButton_Left, EUINavigation::Left);

	KeyEventRules.Emplace(EKeys::Gamepad_FaceButton_Top, EUINavigation::Up);
}

FNavigationPage2Config::FNavigationPage2Config()
{
	bProcessRepeatKeyEvent = false;
	bTabNavigation = false;
	bKeyNavigation = true;
	bAnalogNavigation = false;

	KeyEventRules.Empty();

	KeyEventRules.Emplace(EKeys::F1, EUINavigation::Left);
	KeyEventRules.Emplace(EKeys::Gamepad_LeftTrigger, EUINavigation::Left);

	KeyEventRules.Emplace(EKeys::F2, EUINavigation::Right);
	KeyEventRules.Emplace(EKeys::Gamepad_RightTrigger, EUINavigation::Right);
}
