// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HorizontalBoxWithNavigation.h"

UHorizontalBoxWithNavigation::UHorizontalBoxWithNavigation()
{
	//bUniformGridPanel = true;
	bCanHaveMultipleChildren = true;
	//SelectedChildIndex = 0;
	ControlType = EUIKeyControlType::DirectionNavigation;
}

const EUIKeyControlType& UHorizontalBoxWithNavigation::GetControlType()
{
	return ControlType;
}

TSharedRef<SWidget> UHorizontalBoxWithNavigation::RebuildWidget()
{
	//ResetSelectedChildIndexBySlotNum();

	return Super::RebuildWidget();
}

void UHorizontalBoxWithNavigation::SetControlType(EUIKeyControlType NewControlType)
{
	ControlType = NewControlType;
}
