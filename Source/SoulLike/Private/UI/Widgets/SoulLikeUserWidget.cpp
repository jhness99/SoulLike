// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SoulLikeUserWidget.h"

void USoulLikeUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	PostSetWidgetController();
}
