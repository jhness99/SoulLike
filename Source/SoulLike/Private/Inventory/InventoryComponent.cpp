// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Inventory/EquipmentItemInstance.h"

#include "Player/SoulLikePlayerState.h"

#include "Interface/CombatInterface.h"

#include "AbilitySystem/SoulLikeAttributeSet.h"

#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

#include "SoulLikeFunctionLibrary.h"
#include "SoulLikeGameplayTags.h"

#include "Engine/ActorChannel.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BindToWidgetController()
{
	UInventoryWidgetController* InventoryWC = USoulLikeFunctionLibrary::GetInventoryWidgetController(this);
	if(InventoryWC)
	{
		InventoryWC->BindToInventoryComponent();
		InventoryWC->OnRegistedItem.AddDynamic(this, &UInventoryComponent::RegistItem);
		InventoryWC->OnUpgradedItem.AddDynamic(this, &UInventoryComponent::UpgradeItem);
	}

	UOverlayWidgetController* OverlayWC = USoulLikeFunctionLibrary::GetOverlayWidgetController(this);
	if(OverlayWC)
	{
		OverlayWC->BindToInventoryComponent(this);
	}
}

TArray<FSavedItem> UInventoryComponent::GetSavedItemFromInventoryList() 
{
	TArray<FSavedItem> SavedItems;

	for(const FInventoryListItem& InventoryListItem : InventoryList.GetItemsRef())
	{
		FSavedItem SavedItem;
		SavedItem.InventoryData = InventoryListItem.ItemInstance->GetInventoryData();
		SavedItem.RegistInfo = InventoryListItem.ItemInstance->GetRegistInfo();
		SavedItems.Add(SavedItem);
	}
	
	return SavedItems;
}

void UInventoryComponent::LoadInventoryListFromSavedItems(const TArray<FSavedItem>& SavedItems)
{
	DefaultInventoryItems.Empty();
	for(const FSavedItem& SavedItem : SavedItems)
	{
		FInventoryData Data = SavedItem.InventoryData;
		
		UInventoryItemInstance* ItemInstance;
		if(Data.ItemType.MatchesTagExact(FSoulLikeGameplayTags::Get().ItemType_Item))
		{
			ItemInstance = NewObject<UInventoryItemInstance>(GetOwner());
			ItemInstance->Init(Data);
		}
		else
		{
			ItemInstance = NewObject<UEquipmentItemInstance>(GetOwner());
			ItemInstance->Init(Data);
		}
		AddItem(ItemInstance);
		
		FRegistInfo RegistInfo = SavedItem.RegistInfo;
		
		if(RegistInfo.Index == -1) continue;
		if(URegisterableItemInstance* RegisterableItemInstance = Cast<URegisterableItemInstance>(ItemInstance))
		{
			RegistItem(RegisterableItemInstance, RegistInfo.SlotTag, RegistInfo.Index);

			if(UInventoryWidgetController* InventoryWidgetController = USoulLikeFunctionLibrary::GetInventoryWidgetController(this))
			{
				InventoryWidgetController->OnRegistedItemToWidget.Broadcast(RegisterableItemInstance, RegistInfo.SlotTag, RegistInfo.Index);
			}
		}
	}
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags){

	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;
		if(IsValid(ItemInstance)){
			WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}
	
	return WroteSomething;
}

FEquipmentInventoryList& UInventoryComponent::FindEquipmentSlotWithTag(const FGameplayTag& SlotTag)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	
	if(SlotTag.MatchesTagExact(GameplayTags.EquipSlot_RightWeapon))
	{
		return RightWeaponList;
	}
	if(SlotTag.MatchesTagExact(GameplayTags.EquipSlot_LeftWeapon))
	{
		return LeftWeaponList;
	}
	if(SlotTag.MatchesTagExact(GameplayTags.EquipSlot_Accessory))
	{
		return AccessoryList;
	}
	if(SlotTag.MatchesTagExact(GameplayTags.EquipSlot_Tool))
	{
		return ToolList;
	}

	return RightWeaponList;
}

void UInventoryComponent::RemoveConsumeItem(UInventoryItemInstance* ItemInstance)
{
	if(ItemInstance == nullptr) return;
	
	if(UToolData* ToolData = Cast<UToolData>(ItemInstance->GetItemData()))
	{
		if(!ToolData->bConsume)
		{
			return;
		}
	}
	
	if(UItemData* ItemData = ItemInstance->GetItemData())
	{
		if(ItemInstance->GetItemNum() == 0)
		{
			//없애기
			InventoryList.RemoveItem(ItemInstance);
			ToolList.RemoveItem(ItemInstance);
		}
	} 
}

void UInventoryComponent::ApplyPassiveEffectRegisted(URegisterableItemInstance* ItemInstance) const
{
	const FGameplayTag& SlotTag = ItemInstance->GetRegistInfo().SlotTag;
	if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_Accessory))
	{
		UAccessoryData* Data = ItemInstance->GetAccessoryData();

		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
		if(ASC && Data)
		{
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Data->BoostEffect, 1.f, ContextHandle);
			
			for(FAttributeBoostInfo BoostInfo : Data->AttributeBoostInfos)
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BoostInfo.AttributeBoostTag, BoostInfo.Value);
			}
			
			const FActiveGameplayEffectHandle& ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			
			ItemInstance->SetAppliedActiveGEHandle(ActiveHandle);
		}
	}
}

void UInventoryComponent::RemovePassiveEffectRegisted(URegisterableItemInstance* ItemInstance) const
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if(ASC)
	{
		ASC->RemoveActiveGameplayEffect(ItemInstance->GetAppliedActiveGEHandle());
	}
}

void UInventoryComponent::RefreshCurrentSlot(FEquipmentInventoryList EquipmentSlot, const FGameplayTag& SlotTag)
{
	UpdateInventoryListToWidgetController();
	if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_RightWeapon))
	{
		if(CurrentRightWeapon) CurrentRightWeapon->OnUnEquip();
		CurrentRightWeapon = EquipmentSlot.GetEquipmentItemInstance(RightWeaponSlotIndex);
		if(CurrentRightWeapon) CurrentRightWeapon->OnEquip(GetOwner(), SlotTag);
		
		RefreshRegistSlotAtOverlay.Broadcast(CurrentRightWeapon, SlotTag, GetSlotIndex(SlotTag));
	}
	else if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_LeftWeapon))
	{
		if(CurrentLeftWeapon) CurrentLeftWeapon->OnUnEquip();
		CurrentLeftWeapon = EquipmentSlot.GetEquipmentItemInstance(LeftWeaponSlotIndex);
		if(CurrentLeftWeapon) CurrentLeftWeapon->OnEquip(GetOwner(), SlotTag);

		RefreshRegistSlotAtOverlay.Broadcast(CurrentLeftWeapon, SlotTag, GetSlotIndex(SlotTag));
	}
	else if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_Tool))
	{
		while(EquipmentSlot.GetEquipmentItemInstance(ToolSlotIndex) == nullptr)
		{
			ToolSlotIndex++;
			if(ToolSlotIndex == EquipmentSlot.GetMaxIndex())
			{
				ToolSlotIndex = 0;
				break;
			}
		}
		
		if(CurrentTool) CurrentTool->OnUnEquip();
		CurrentTool = EquipmentSlot.GetEquipmentItemInstance(ToolSlotIndex);
		if(CurrentTool) CurrentTool->OnEquip(GetOwner(), SlotTag);
		
		RefreshRegistSlotAtOverlay.Broadcast(CurrentTool, SlotTag, GetSlotIndex(SlotTag));
	}
	
	if(ASoulLikePlayerState* SL_PS = CastChecked<ASoulLikePlayerState>(UGameplayStatics::GetPlayerState(this, 0)))
	{
		APawn* Pawn = SL_PS->GetPawn();	
		if(Pawn && Pawn->Implements<UCombatInterface>())
		{
			if(CurrentRightWeapon == nullptr)
			{
				ICombatInterface::Execute_SetupCurrentWeaponItemType(Pawn, FSoulLikeGameplayTags::Get().ItemType_Weapon_None);
			}
			else if(UItemData* ItemData = CurrentRightWeapon->GetItemData())
			{
				ICombatInterface::Execute_SetupCurrentWeaponItemType(Pawn, ItemData->ItemType);
			}
		}
	}
}

int32 UInventoryComponent::GetSlotIndex(const FGameplayTag& SlotTag) const
{
	if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_RightWeapon))
	{
		return RightWeaponSlotIndex;
	}
	if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_LeftWeapon))
	{
		return LeftWeaponSlotIndex;
	}
	if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_Tool))
	{
		return ToolSlotIndex;
	}
	return 0;
}

void UInventoryComponent::AddItem(UInventoryItemInstance* InItemInstance)
{
	if (!IsValid(InItemInstance)) return;
	
	InventoryList.AddItem(InItemInstance);
}

void UInventoryComponent::RemoveItem(UInventoryItemInstance* InItemInstance)
{
	if (!IsValid(InItemInstance)) return;
	
	InventoryList.RemoveItem(InItemInstance);
}

void UInventoryComponent::OnRep_CurrentRightWeapon(UEquipmentItemInstance* OldCurrentRightWeapon)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	const FGameplayTag& SlotTag = GameplayTags.EquipSlot_RightWeapon;
	
	RefreshRegistSlotAtOverlay.Broadcast(CurrentRightWeapon, SlotTag, GetSlotIndex(SlotTag));
}

void UInventoryComponent::OnRep_CurrentLeftWeapon(UEquipmentItemInstance* OldCurrentLeftWeapon)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	const FGameplayTag& SlotTag = GameplayTags.EquipSlot_LeftWeapon;
	
	RefreshRegistSlotAtOverlay.Broadcast(CurrentLeftWeapon, SlotTag, GetSlotIndex(SlotTag));
}

void UInventoryComponent::OnRep_CurrentTool(UEquipmentItemInstance* OldCurrentTool)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	const FGameplayTag& SlotTag = GameplayTags.EquipSlot_Tool;
	
	RefreshRegistSlotAtOverlay.Broadcast(CurrentTool, SlotTag, GetSlotIndex(SlotTag));
}

void UInventoryComponent::UsingTool(URegisterableItemInstance*& ItemInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("%hs"), __FUNCTION__);
	if(ItemInstance == nullptr) ItemInstance = CurrentTool;
	if(ItemInstance == nullptr) return;
	
	ItemInstance->SetItemNum(ItemInstance->GetItemNum() - 1);
	RemoveConsumeItem(ItemInstance);

	UpdateInventoryListToWidgetController();
}

void UInventoryComponent::RefillPotion()
{
	ASoulLikePlayerState* SL_PS = Cast<ASoulLikePlayerState>(GetOwner());
	if(SL_PS == nullptr) return;
	
	for(const FInventoryListItem& InventoryListItem : InventoryList.GetItemsRef())
	{
		if(UToolData* ToolData = InventoryListItem.ItemInstance->GetToolData())
		{
			if(ToolData->ItemID == FName("Tool_1") || ToolData->ItemID == FName("Tool_2"))
			{
				InventoryListItem.ItemInstance->SetItemNum(SL_PS->GetMaxPotion());
			}
		}
	}
	UpdateInventoryListToWidgetController();
}

bool UInventoryComponent::CheckRequirement(URegisterableItemInstance* ItemInstance) const
{
	if(UWeaponData* WeaponData = ItemInstance->GetWeaponData())
	{
		ASoulLikePlayerState* SL_PS = CastChecked<ASoulLikePlayerState>(UGameplayStatics::GetPlayerState(this, 0));
		USoulLikeAttributeSet* SL_AS = CastChecked<USoulLikeAttributeSet>(SL_PS->GetAttributeSet());
		
		for(const TTuple<FGameplayTag, int>& Pair : WeaponData->WeaponRequirements)
		{
			FGameplayAttribute Attribute = SL_AS->TagsToAttributes.FindChecked(Pair.Key)();
			int32 AttributeValue = Attribute.GetNumericValue(SL_AS);

			if(AttributeValue < Pair.Value)
			{
				return false;
			}
		}
	}
	return true;
}

void UInventoryComponent::NextSlot_Implementation(const FGameplayTag& SlotTag)
{
	FEquipmentInventoryList& EquipmentSlot = FindEquipmentSlotWithTag(SlotTag);

	int32 MaxIndex = EquipmentSlot.GetMaxIndex();
	if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_RightWeapon))
	{
		RightWeaponSlotIndex = RightWeaponSlotIndex == MaxIndex-1 ? 0 : RightWeaponSlotIndex + 1;
	}
	else if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_LeftWeapon))
	{
		LeftWeaponSlotIndex = LeftWeaponSlotIndex == MaxIndex-1 ? 0 : LeftWeaponSlotIndex + 1;
	}
	else if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_Tool))
	{
		ToolSlotIndex = ToolSlotIndex == MaxIndex-1 ? 0 : ToolSlotIndex + 1;
	}
	
	RefreshCurrentSlot(EquipmentSlot, SlotTag);
}

void UInventoryComponent::RegistItem_Implementation(URegisterableItemInstance* ItemInstance, const FGameplayTag& SlotTag, int32 Index)
{
	if(!CheckRequirement(ItemInstance)) return;
	if(!ItemInstance) return;
	FEquipmentInventoryList& EquipmentSlot = FindEquipmentSlotWithTag(SlotTag);
	if(EquipmentSlot.GetMaxIndex() <= Index) return;

	if(MarkAsDirtyDelegate.IsBound())
	{
		MarkAsDirtyDelegate.Execute();
	}
	
	URegisterableItemInstance* CurrentItem = EquipmentSlot.GetEquipmentItemInstance(Index);

	// 이미 등록된 아이템인 경우 해제 처리
	if(ItemInstance->GetRegisted())
	{
		// 현재 인덱스에 이미 등록되어 있는 경우, 등록 해제만 수행
		if(ItemInstance == CurrentItem)
		{
			UnRegistItem(ItemInstance);
			RefreshCurrentSlot(EquipmentSlot, SlotTag);
			return;
		}
		UnRegistItem(ItemInstance);
	}

	if(CurrentItem && CurrentItem->GetRegisted())
	{
		UnRegistItem(CurrentItem);
	}
	ItemInstance->SetRegistInfo(FRegistInfo(SlotTag, Index, true));
	EquipmentSlot.Register(ItemInstance, Index);
	
	RefreshCurrentSlot(EquipmentSlot, SlotTag);

	ApplyPassiveEffectRegisted(ItemInstance);
}

void UInventoryComponent::UnRegistItem_Implementation(URegisterableItemInstance* ItemInstance)
{
	if(!ItemInstance) return;
	const FRegistInfo& Reginfo = ItemInstance->GetRegistInfo();
 
	FEquipmentInventoryList& EquipmentSlot =  FindEquipmentSlotWithTag(Reginfo.SlotTag);
	
	ItemInstance->SetRegistInfo(FRegistInfo());
	EquipmentSlot.UnRegister(Reginfo.Index);

	RemovePassiveEffectRegisted(ItemInstance);
}

void UInventoryComponent::Init()
{
	RightWeaponList.Init(3);
	LeftWeaponList.Init(3);
	AccessoryList.Init(4);
	ToolList.Init(10);
}

void UInventoryComponent::SetupDefaultInventoryList()
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	
	/**
	 * 현재 테스트를 위해 하드코딩으로 State에 Inventory에 임의로 등록된 인벤토리 리스트를 사용해서 인벤토리 추가
	 */
	
	for(const FInventoryData& Data : DefaultInventoryItems)
	{
		UInventoryItemInstance* ItemInstance;
		if(Data.ItemType.MatchesTagExact(GameplayTags.ItemType_Item))
		{
			ItemInstance = NewObject<UInventoryItemInstance>(GetOwner());
			ItemInstance->Init(Data);
		}else
		{
			ItemInstance = NewObject<UEquipmentItemInstance>(GetOwner());
			ItemInstance->Init(Data);
		}
		AddItem(ItemInstance);
	}
	
	int32 Index = 0;
	
	for(const FInventoryData& Data : DefaultWeapons)
	{
		UEquipmentItemInstance* ItemInstance = NewObject<UEquipmentItemInstance>(GetOwner());
		ItemInstance->Init(Data);
		
		RightWeaponList.Register(ItemInstance, Index);
		AddItem(ItemInstance);
		Index++;
	}
	
	InventoryList.SortItems();

	if(GetOwner())
	{
		if(ASoulLikePlayerState* SL_PS = Cast<ASoulLikePlayerState>(GetOwner()))
		{
			UE_LOG(LogTemp, Warning, TEXT("SL_PS name is : %s"), *SL_PS->GetName());
		}
	}
}

void UInventoryComponent::Reset()
{
	CurrentLeftWeapon = nullptr;
	CurrentLeftWeapon = nullptr;
	CurrentTool = nullptr;

	InventoryList.RemoveAll();
	RightWeaponList.RemoveAll();
	LeftWeaponList.RemoveAll();
	AccessoryList.RemoveAll();
	ToolList.RemoveAll();
}

void UInventoryComponent::EquipItem()
{
	CurrentRightWeapon = RightWeaponList.GetEquipmentItemInstance(RightWeaponSlotIndex);
	if(CurrentRightWeapon != nullptr)
	{
		CurrentRightWeapon->OnEquip(GetOwner(), FGameplayTag());
	}
}

void UInventoryComponent::AddItemToInventoryList(const FInventoryData& InventoryData)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();

	if(MarkAsDirtyDelegate.IsBound())
	{
		MarkAsDirtyDelegate.Execute();
	}
	
	UInventoryItemInstance* ItemInstance;
	
	if(InventoryData.ItemType.MatchesTagExact(GameplayTags.ItemType_Item))
	{
		if(InventoryData.ItemID == "UpgradeStone")
		{
			if(UInventoryItemInstance* UpgradeMaterial = GetItemInstanceWithTag(FSoulLikeGameplayTags::Get().ItemType_Item_UpgradeMaterial))
			{
				const int32 CurrentUpgradeMaterial = GetItemNumWithTag(FSoulLikeGameplayTags::Get().ItemType_Item_UpgradeMaterial);

				UpgradeMaterial->SetItemNum(CurrentUpgradeMaterial + 1);
				PickedUpDelegate.Broadcast(UpgradeMaterial->GetItemData());
				return;
			}
		}
		
		ItemInstance = NewObject<UInventoryItemInstance>(GetOwner());
	}
	else if(InventoryData.ItemType.MatchesTagExact(GameplayTags.ItemType_Tool) || InventoryData.ItemType.MatchesTagExact(GameplayTags.ItemType_Accessory))
	{
		ItemInstance = NewObject<URegisterableItemInstance>(GetOwner());
	}
	else
	{
		ItemInstance = NewObject<UEquipmentItemInstance>(GetOwner());
	}
	
	ItemInstance->Init(InventoryData);
	AddItem(ItemInstance);
	InventoryList.SortItems();
	PickedUpDelegate.Broadcast(ItemInstance->GetItemData());
}

void UInventoryComponent::UpgradeItem(URegisterableItemInstance* ItemInstance)
{
	if(ItemInstance == nullptr) return;

	if(MarkAsDirtyDelegate.IsBound())
	{
		MarkAsDirtyDelegate.Execute();
	}
	
	const int32 CurrentUpgradeLevel = ItemInstance->GetUpgradeLevel();
	const int32 CurrentUpgradeMaterial = GetItemNumWithTag(FSoulLikeGameplayTags::Get().ItemType_Item_UpgradeMaterial);

	if(CurrentUpgradeLevel < CurrentUpgradeMaterial)
	{
		UInventoryItemInstance* MaterialInstance = GetItemInstanceWithTag(FSoulLikeGameplayTags::Get().ItemType_Item_UpgradeMaterial);
		MaterialInstance->SetItemNum(CurrentUpgradeMaterial - (CurrentUpgradeLevel+1));
		RemoveConsumeItem(MaterialInstance);
		ItemInstance->SetUpgradeLevel(CurrentUpgradeLevel+1);
		UpdateInventoryListToWidgetController();
	}
}

void UInventoryComponent::UpdateInventoryListToWidgetController() const
{
	if(UInventoryWidgetController* InventoryWC = USoulLikeFunctionLibrary::GetInventoryWidgetController(this))
	{
		InventoryWC->InventoryList = InventoryList;
		InventoryWC->OnChangedInventory.Broadcast();

		if(MarkAsDirtyDelegate.IsBound())
		{
			MarkAsDirtyDelegate.Execute();
		}
	}
}

void UInventoryComponent::UpdateRegistSlotToWidgetController() const
{
	UInventoryWidgetController* InventoryWidgetController = USoulLikeFunctionLibrary::GetInventoryWidgetController(this);
	if(InventoryWidgetController == nullptr) return;
	
	for(const FInventoryListItem& InventoryListItem : InventoryList.GetItems())
	{
		UEquipmentItemInstance* EquipmentItemInstance = Cast<UEquipmentItemInstance>(InventoryListItem.ItemInstance);
		if(EquipmentItemInstance == nullptr) continue;

		const FRegistInfo& RegistInfo = EquipmentItemInstance->GetRegistInfo();
		
		if(RegistInfo.Index == -1) continue;
		InventoryWidgetController->OnRegistedItemToWidget.Broadcast(EquipmentItemInstance, RegistInfo.SlotTag, RegistInfo.Index);
	}
}

int32 UInventoryComponent::GetItemNumWithTag(const FGameplayTag& Tag)
{
	if(UInventoryItemInstance* ItemInstance = GetItemInstanceWithTag(Tag))
	{
		return ItemInstance->GetItemNum();
	}
	
	return 0;
}

UInventoryItemInstance* UInventoryComponent::GetItemInstanceWithTag(const FGameplayTag& Tag)
{
	for(FInventoryListItem& Item: InventoryList.GetItemsRef())
	{
		if(UItemData* ItemData = Item.ItemInstance->GetItemData())
		{
			if(Tag.MatchesTagExact(ItemData->ItemType))
			{
				return Item.ItemInstance;
			}
		}
	}
	return nullptr;
}

void UInventoryComponent::SetDefaultWeapons(const TArray<FInventoryData>& InDefaultWeapons)
{
	DefaultWeapons = InDefaultWeapons;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, RightWeaponSlotIndex);
	DOREPLIFETIME(UInventoryComponent, LeftWeaponSlotIndex);
	DOREPLIFETIME(UInventoryComponent, ToolSlotIndex);
	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, RightWeaponList);
	DOREPLIFETIME(UInventoryComponent, LeftWeaponList);
	DOREPLIFETIME(UInventoryComponent, AccessoryList);
	DOREPLIFETIME(UInventoryComponent, ToolList);
	DOREPLIFETIME(UInventoryComponent, CurrentRightWeapon);
	DOREPLIFETIME(UInventoryComponent, CurrentLeftWeapon);
	DOREPLIFETIME(UInventoryComponent, CurrentTool);
}
