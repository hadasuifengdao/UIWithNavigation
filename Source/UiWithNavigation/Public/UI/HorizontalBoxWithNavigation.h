// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/NavigationWidgetPanelInterface.h"
#include "Components/HorizontalBox.h"
#include "HorizontalBoxWithNavigation.generated.h"

UCLASS()
class UIWITHNAVIGATION_API UHorizontalBoxWithNavigation : public UHorizontalBox, public INavigationWidgetPanelInterface
{
	GENERATED_BODY()

public:

	UHorizontalBoxWithNavigation();

public://Interface

	virtual const EUIKeyControlType& GetControlType() override;

protected:

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigation")
		EUIKeyControlType ControlType;	

public:
	UFUNCTION(BlueprintCallable, Category = "Navigation")
		void SetControlType(EUIKeyControlType NewControlType);
};
