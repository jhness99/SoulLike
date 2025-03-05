// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/SL_InputConfig.h"
#include "SoulLikeInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * 입력받은 InputAction에 매칭된 GameplayTag를 매개변수로 가지는 함수를
	 * InputAction에 바인드하는 함수
	 * @tparam UserClass Object
	 * @tparam PressedFuncType 바인드 할 함수 포인터
	 * @tparam HeldFuncType 바인드 할 함수 포인터
	 * @tparam ReleasedFuncType 바인드 할 함수 포인터
	 * @param InputConfig InputAction, GameplayTag 매칭을 저장한 DataAsset
	 * @param Object 
	 * @param PressedFunc 
	 * @param HeldFunc 
	 * @param ReleasedFunc 
	 */
	template<class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const USL_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc);
	
};

template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
inline void USoulLikeInputComponent::BindAbilityActions(const USL_InputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for(const FSL_InputAction& InputAction : InputConfig->InputActions)
	{
		if(PressedFunc) BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputAction.InputTag);
		if(HeldFunc) BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.InputTag);
		if(ReleasedFunc) BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.InputTag);
	}
}
