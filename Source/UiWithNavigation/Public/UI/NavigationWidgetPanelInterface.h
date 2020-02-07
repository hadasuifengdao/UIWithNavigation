// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "Templates/Casts.h"
#include "UObject/Interface.h"
#include "Components/PanelWidget.h"
#include "UI/MyNavigationConfig.h"
#include "UI/NavigationUserWidget.h"
#include "NavigationWidgetPanelInterface.generated.h"

//声明无参代理

DECLARE_DELEGATE(FOnSelectedChildChangedDelegate)

/** Interface for assets/objects that can own UserData **/
UINTERFACE(MinimalApi, meta = (CannotImplementInterfaceInBlueprint))
class UNavigationWidgetPanelInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class INavigationWidgetPanelInterface
{
	GENERATED_IINTERFACE_BODY()
	
public:

	virtual const EUIKeyControlType& GetControlType() = 0;

	UFUNCTION(BlueprintCallable, Category = "PaladinNavigation")
	virtual void SetNavigationActive(bool b, ESlateVisibility ActiveVisibility = ESlateVisibility::SelfHitTestInvisible, ESlateVisibility UnActiveVisibility = ESlateVisibility::HitTestInvisible);

	UFUNCTION(BlueprintCallable, Category = "PaladinNavigation")
	virtual bool IsNavigationActive();

	UFUNCTION(BlueprintCallable, Category = "PaladinNavigation")
	virtual void SetNavigationSound(class USoundCue* Sound);

	virtual int32 GetSelectedChildIndex();

	virtual void ChildRequestBeSelected(UNavigationUserWidget*NavWidget);

	virtual UNavigationUserWidget* GetSelectedChild();

	virtual void PanelNavigation(const FNavigationEvent& NavigationEvent);

public:

	INavigationWidgetPanelInterface();

private:
	
	bool bNavigationActive;

	int32 OnSelctedNumber=-1;

public:

	class USoundCue* CustomNavigationSound;

	FOnSelectedChildChangedDelegate  OnSelectedChildChangedDelegate;
};