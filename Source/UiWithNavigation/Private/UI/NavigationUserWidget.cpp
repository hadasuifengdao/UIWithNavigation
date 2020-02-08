#include "UI/NavigationUserWidget.h"
#include "UI/UIManager.h"
#include "Application/SlateApplicationBase.h"
#include "Blueprint/WidgetTree.h"
#include "UI/NavigationWidgetPanelInterface.h"
#include "Components/Button.h"
#include "Mode/UiWithNavigationGameMode.h"

void UNavigationUserWidgetBase::NativeConstruct()
{
	bNavigationActive = true;

	Super::NativeConstruct();
}

UPanelWidget* UNavigationUserWidgetBase::GetNavigationParentPanel()
{
	return nullptr;
}

bool UNavigationUserWidgetBase::UIEvent(EUIKeyControlType UIKeyControlType, EUINavigation UINavigation)
{
	if (!OnUIEvent(UIKeyControlType, UINavigation))
	{
		return false;
	}

	UPanelWidget* NavigationParentPanel = GetNavigationParentPanel();
	if (NavigationParentPanel)
	{
		//广播这次导航成功的消息
		//NavigationParentPanel->OnSelectedChildProcessedUIEvent.Broadcast((uint8)UIKeyControlType, UINavigation);
	}
	return true;
}

void UNavigationUserWidgetBase::ButtonCallUIEvent(EUIKeyControlType UIKeyControlType, EUINavigation UINavigation)
{
	UIEvent(UIKeyControlType, UINavigation);
}

bool UNavigationUserWidgetBase::IsNavigationActive()
{
	return bNavigationActive;
}

void UNavigationUserWidgetBase::SetNavigationActive(bool b)
{
	bNavigationActive = b;
}

void UNavigationUserWidgetBase::SetControlTypes(TArray<EUIKeyControlType> InControlTypes)
{
	ControlTypes = InControlTypes;
}

const TArray<EUIKeyControlType>& UNavigationUserWidgetBase::GetControlTypes()
{
	return ControlTypes;
}

UPanelWidget* UNavigationUserWidget::GetNavigationParentPanel()
{
	UPanelWidget* ParentPanel = GetParent();
	if (INavigationWidgetPanelInterface* NavigationParentPanel = Cast<INavigationWidgetPanelInterface>(ParentPanel))
	{
		return ParentPanel;
	}
	return nullptr;
}

bool UNavigationUserWidget::IsNavigationActive()
{
	UPanelWidget* NavigationParentPanel = GetNavigationParentPanel();
	if (NavigationParentPanel)
	{
		INavigationWidgetPanelInterface* NavigationParentPanelInterface = CastChecked<INavigationWidgetPanelInterface>(NavigationParentPanel);
		return bNavigationActive && NavigationParentPanelInterface->IsNavigationActive();
	}
	return true;
}

bool UNavigationUserWidget::IsSelected()
{
	if (INavigationWidgetPanelInterface* NavigationParentPanel = Cast<INavigationWidgetPanelInterface>(GetNavigationParentPanel()))
	{
		return (this == NavigationParentPanel->GetSelectedChild());
	}
	return true;
}

void UNavigationUserWidget::RequestBeSelected()
{
	if (INavigationWidgetPanelInterface* NavigationParentPanel = Cast<INavigationWidgetPanelInterface>(GetNavigationParentPanel()))
	{
		int32 PreSelectChildIdx = NavigationParentPanel->GetSelectedChildIndex();
		NavigationParentPanel->ChildRequestBeSelected(this);
		int32 CurSelectChildIdx = NavigationParentPanel->GetSelectedChildIndex();
		if (PreSelectChildIdx != CurSelectChildIdx)
		{
			UUIManager* UIManager = AUiWithNavigationGameMode::GetUIManager();
			UIManager->PlayPanelNavigationSound(GetNavigationParentPanel());
		}
	}
}

int32 UNavigationUserWidget::GetSelfIndex()
{
	if (UPanelWidget* NavigationParentPanel = GetNavigationParentPanel())
	{
		return NavigationParentPanel->GetChildIndex(this);
	}
	return -1;
}

void UNavigationUserWidget::NativeConstruct()
{
	if (GetNavigationParentPanel())
	{
		if (INavigationWidgetPanelInterface* NavigationParentPanel = Cast<INavigationWidgetPanelInterface>(GetNavigationParentPanel()))
		{
			//NavigationParentPanel->OnSelectedChildChangedDelegate.AddUniqueDynamic(this, &UNavigationUserWidget::OnNativeParentSelectedChanged);
			NavigationParentPanel->OnSelectedChildChangedDelegate.AddUObject(this, &UNavigationUserWidget::OnNativeParentSelectedChanged);
		}
	}
	
	Super::NativeConstruct();
}

void UNavigationUserWidget::OnNativeParentSelectedChanged()
{
	if (GetNavigationParentPanel())
	{
		if (INavigationWidgetPanelInterface* NavigationParentPanel = Cast<INavigationWidgetPanelInterface>(GetNavigationParentPanel()))
		{
			OnSelectedByParentPanel(NavigationParentPanel->GetSelectedChild() == this);
		}
	}
}

bool UNavigationUserWidget::K2_IsNavigationActive()
{
	return IsNavigationActive();
}
