// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/MyNavigationConfig.h"
#include "NavigationUserWidget.generated.h"

/*
会接收Navigation事件的UserWidget
通过设置Active来On/Off该功能
对父节点类型无要求
*/
UCLASS(Abstract)
class UIWITHNAVIGATION_API UNavigationUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	bool UIEvent(EUIKeyControlType UIKeyControlType, EUINavigation UINavigation);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void ButtonCallUIEvent(EUIKeyControlType UIKeyControlType = EUIKeyControlType::Confirm, EUINavigation UINavigation = EUINavigation::Next);

	virtual bool IsNavigationActive();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void SetNavigationActive(bool b);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void SetControlTypes(TArray<EUIKeyControlType> InControlTypes);

	const TArray<EUIKeyControlType>& GetControlTypes();

	virtual UPanelWidget* GetNavigationParentPanel();

protected:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Navigation")
	bool OnUIEvent(EUIKeyControlType UIKeyControlType, EUINavigation UINavigation);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigation")
	TMap<FUINavigationKey, class USoundCue*> CustomSoundConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigation")
	TArray<EUIKeyControlType> ControlTypes;

	bool bNavigationActive;
};

/*
作为导航容器中的子结点
父节点为INavigationWidgetPanelInterface时 导航功能才会起作用
否则与普通UserWidget没区别
*/
UCLASS(Abstract)
class UIWITHNAVIGATION_API UNavigationUserWidget : public UNavigationUserWidgetBase
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Navigation")
	void OnSelectedByParentPanel(bool b);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Navigation")
	void OnParentPanelActiveStateChanged(bool ParentPanelIsActive);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	bool IsSelected();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void RequestBeSelected();
	
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	int32 GetSelfIndex();

	UFUNCTION()
	void OnNativeParentSelectedChanged();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	bool K2_IsNavigationActive();

	virtual bool IsNavigationActive() override;

	virtual UPanelWidget* GetNavigationParentPanel() override;

public:

	UPROPERTY(EditAnywhere,  Category = "Navigation")
	TMap<EUINavigation, class UNavigationUserWidget*> CustomNavigationConfig;
};
