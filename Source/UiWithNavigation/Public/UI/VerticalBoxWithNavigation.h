// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationWidgetPanelInterface.h"
#include "Components/VerticalBox.h"
#include "VerticalBoxWithNavigation.generated.h"

UCLASS()
class UIWITHNAVIGATION_API UVerticalBoxWithNavigation : public UVerticalBox, public INavigationWidgetPanelInterface
{
	GENERATED_BODY()

public:

	UVerticalBoxWithNavigation();

public://Interface

	virtual const EUIKeyControlType& GetControlType() override;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PaladinNavigation")
		EUIKeyControlType ControlType;	
};
