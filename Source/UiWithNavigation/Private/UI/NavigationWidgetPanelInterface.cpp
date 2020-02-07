#include "UI/NavigationWidgetPanelInterface.h"
#include "Blueprint/WidgetTree.h"

UNavigationWidgetPanelInterface::UNavigationWidgetPanelInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void INavigationWidgetPanelInterface::SetNavigationActive(bool b, ESlateVisibility ActiveVisibility /*= ESlateVisibility::SelfHitTestInvisible*/, ESlateVisibility UnActiveVisibility /*= ESlateVisibility::HitTestInvisible*/)
{
	bNavigationActive = b;

	UPanelWidget* PanelWidget = Cast<UPanelWidget>(this);

	PanelWidget->SetVisibility(b ? ActiveVisibility : UnActiveVisibility);

	for (UWidget* Child : PanelWidget->GetAllChildren())
	{
		if (UNavigationUserWidget * NavigationUserWidget = Cast<UNavigationUserWidget>(Child))
		{
			NavigationUserWidget->OnParentPanelActiveStateChanged(b);
		}
	}
}

bool INavigationWidgetPanelInterface::IsNavigationActive()
{
	return bNavigationActive;
}

void INavigationWidgetPanelInterface::SetNavigationSound(class USoundCue* Sound)
{
	CustomNavigationSound = Sound;
}

INavigationWidgetPanelInterface::INavigationWidgetPanelInterface()
	: bNavigationActive(true)
	, CustomNavigationSound(nullptr)
{

}

int32 INavigationWidgetPanelInterface::GetSelectedChildIndex()
{
	return OnSelctedNumber;
}

void INavigationWidgetPanelInterface::ChildRequestBeSelected(UNavigationUserWidget* NavWidget)
{
	UPanelWidget* PanelWidget = Cast<UPanelWidget>(this);
	int32  WidgetIndex=PanelWidget->GetChildIndex(NavWidget);
	if ((WidgetIndex != INDEX_NONE) && (OnSelctedNumber != WidgetIndex))
	{
		OnSelctedNumber = WidgetIndex;
		OnSelectedChildChangedDelegate.Execute();
	}
}
UNavigationUserWidget* INavigationWidgetPanelInterface::GetSelectedChild()
{
	UPanelWidget* PanelWidget = Cast<UPanelWidget>(this);
	if((PanelWidget->HasAnyChildren()))
	{
		UNavigationUserWidget* NavigationUserWidget = Cast<UNavigationUserWidget>(PanelWidget->GetChildAt(OnSelctedNumber));
		return NavigationUserWidget;
	}
	return nullptr;
}

void INavigationWidgetPanelInterface::PanelNavigation(const FNavigationEvent& NavigationEvent)
{
	UPanelWidget* PanelWidget = Cast<UPanelWidget>(this);
	EUINavigation NavigationType = NavigationEvent.GetNavigationType();
	if ((NavigationType == EUINavigation::Down) || (NavigationType == EUINavigation::Left))
	{
		if (PanelWidget->GetChildAt(OnSelctedNumber - 1))
		{
			OnSelctedNumber--;
			OnSelectedChildChangedDelegate.Execute();
		}
	}
	
	if ((NavigationType == EUINavigation::Up) || (NavigationType == EUINavigation::Right))
	{
		if (PanelWidget->GetChildAt(OnSelctedNumber + 1))
		{
			OnSelctedNumber++;
			OnSelectedChildChangedDelegate.Execute();
		}
	}
}