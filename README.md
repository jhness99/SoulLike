## SoulLike Action RPG
Unreal Engine 5 Portfolio
- Unreal Engine 5 버전 : 5.4.4
- 에디터 : VSCode, Rider
- 제작기간 : 2025.03~2025.06 (4개월)
- 개발 인원 : 1인개발
## 프로젝트 개요
매인사진        
소울라이크 액션을 레퍼런스로 액션 RPG     
GAS 기반의 전투 시스템, 커스터마이징 가능한 키 바인딩, FastArray 기반 인벤토리, MVC 구조의 UI 연동, ObjectPooling 적 리스폰 등을 구현
## CharacterBase 구조

## MVC 패턴
Widget과 클래스간의 의존성을 줄이기 위해 MVC패턴으로 UI를 구현
1. 로컬 클라이언트에 하나만 존재하는 Controller의 HUD에 WidgetController를 생성
2. 현재 클라이언트가 조종하는 Controller는 싱글톤처럼 한개의 객체만 존재하고 HUD또한 한개만 존재
3. UBlueprintFunctionLibrary를 재정의 한 Static Helper Function을 사용해서 HUD의 WidgetController를 사용

## KeyBind
![KeyBindChangeScreenShot](Images/KeyBindChangeScreenShot.png)
![KeyBindChangeScreenShot](Images/KeyBindChangeScreenShotAfter.png)     
![KeyBind](Images/KeyBindFlow.png)
EnhancedInput의 InputAction 기반 구조를 확장하여, GameplayTag 기반으로 Ability를 동적으로 연결할 수 있도록 설계        
EnhancedInput의 InputAction을 GameplayTag(InputTag)와 매핑       
InputTag를 캐릭터의 어빌리티에 매핑해서 Ability의 TriggerInput을 동적으로 전환할 수 있도록 구현      
![InputWorkFlow](Images/InputWorkFlow.png)      
InputTag로 입력을 구분하고 Ability를 활성화   
![KeybindChangeFlow](Images/KeybindChangeFlow.png)      
InputAction과 Ability는 InputTag로 매핑되어 있으므로, Ability의 InputTag를 변경한다면, 매칭된 InputAction 변경 가능
```c++
//SoulLikePlayerController.cpp
void ASoulLikePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
   ...

	if(InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_KeyBind))
	{
		ChangeAbilityInputTag(InputTag);
		return;
	}
	
	if(GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void ASoulLikePlayerController::ChangeAbilityInputTag(const FGameplayTag& InputTag)
{
	if(UKeybindMenuWidgetController* KeybindMenuWidgetController = USoulLikeFunctionLibrary::GetKeybindMenuWidgetController(this))
	{
		if(GetASC())
		{
			GetASC()->ChangeAbilityInputTag(KeybindMenuWidgetController, InputTag);
		}
	}
}
```
```c++
//SoulLikeAbilitySystemComponent.cpp
void USoulLikeAbilitySystemComponent::ChangeAbilityInputTag(UKeybindMenuWidgetController* KeybindMenuWidgetController, const FGameplayTag& InputTag)
{
	UAbilityInfo* AbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfo(this);
	if(AbilityInfo == nullptr) return;

	if(KeybindMenuWidgetController == nullptr) return;

	const FGameplayTag SelectedAbilityTag = KeybindMenuWidgetController->SelectedAbilityTag;
  //변경할 InputTag를 가지고 있는 어빌리티는 초기화
	if(FGameplayAbilitySpec* PrevAbilitySpec = GetSpecFromInputTag(InputTag))
	{
		const FGameplayTag& PrevAbilityTag = GetAbilityTagFromSpec(*PrevAbilitySpec);
		
		PrevAbilitySpec->DynamicAbilityTags.RemoveTag(InputTag);
		
		AbilityInfo->ChangeAbilityInputTag(PrevAbilityTag, FGameplayTag());

		MarkAbilitySpecDirty(*PrevAbilitySpec);
		
		KeybindMenuWidgetController->OnReceiveInputTagDelegate.Broadcast(PrevAbilityTag);
	}
  //변경할 어빌리티 태그에 InputTag추가
	if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(SelectedAbilityTag))
	{
		AbilitySpec->DynamicAbilityTags.RemoveTag(AbilityInfo->GetAbilityInputTag(SelectedAbilityTag));
		
		AbilityInfo->ChangeAbilityInputTag(SelectedAbilityTag, InputTag);

		AbilitySpec->DynamicAbilityTags.AddTag(InputTag);

		MarkAbilitySpecDirty(*AbilitySpec);
		
    //Widget을 Delegate를 통해 갱신
		KeybindMenuWidgetController->SelectedAbilityTag = FGameplayTag();
		KeybindMenuWidgetController->OnReceiveInputTagDelegate.Broadcast(SelectedAbilityTag);
	}
}
```
## InventorySystem
멀티플레이를 고려하여 Inventory를 FastArraySerializer를 사용<br>
Item의 내부 데이터는 ItemInstance에 UItemData와 FInventoryData에서 보관<br>
1. UItemData : 변하지 않는 아이템의 정보
2. FInventoryData : 변하는 아이템의 정보

ItemInstance를 생성할 때 FindItemDataFromIndexAndItemType() 팩토리 함수 사용<br>
ItemID와 GameplayTag로 FSL_ItemData를 탐색<br>
탐색한 FSL_ItemData로 UItemData를 생성, 생성한 ItemData를 ItemInstance에 저장
```c++
UItemData* UItemDataAsset::FindItemDataFromIndexAndItemType(UObject* Outer, FGameplayTag ItemType, FName ItemID) const
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	
	for(const FItemDataTable& ItemDataTableStruct : ItemDataTables)
	{
		if(ItemType.MatchesTagExact(ItemDataTableStruct.ItemTypeTag))
		{
			FSL_ItemData* ItemData = ItemDataTableStruct.DataTable->FindRow<FSL_ItemData>(ItemID, FString("Not Found"));
			if(ItemData == nullptr) return nullptr;

			if(Outer)
			{
				UItemData* ItemDataObject = NewObject<UItemData>(Outer, ItemDataTableStruct.ItemDataClass.Get());
				ItemDataObject->Init(ItemData);

				return ItemDataObject;
			}
		
		}
	}
	
	return nullptr;
}
```
### ObjectPoolingSubsystem
### MeleeTrace
### ComboAbility의 State로 인한 FSM
### GAS



