// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoulLikeUserWidget.generated.h"

/**
 * WidgetController(MVC의 C)와 View인 Widget을 연결하기 위한 클래스
 * Model은 여러 내부 로직(AttributeSet, AbilitySystem)
 */
UCLASS()
class SOULLIKE_API USoulLikeUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void PostSetWidgetController();
};
