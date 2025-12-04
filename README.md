# SoulLike Action RPG

![Main](Images/main.png)

# 목차
>1. [프로젝트 개요](#프로젝트-개요)
>2. [InventorySystem](#InventorySystem)
>3. [UI](#UI)
>4. [KeyBind](#keybind)
>5. [ObjectPoolingSubsystem](#ObjectPoolingSubsystem)
>6. [Save/Load System](#SaveLoad-System)
>7. [Multiplayer Session (Sign-Based Summoning)](#multiplayer-session-sign-based-summoning)
>8. [MeleeTrace](#MeleeTrace)
>9. [GameplayAbilitySystem](#gameplayabilitysystem)        
    9.1 [AbilityState](#AbilityState)        
    9.2 [TargetLock](#TargetLock)              
    9.3 [InteractionAbility](#InteractionAbility)
>10. [플레이 영상](#플레이-영상)

## 프로젝트 개요
**Unreal Engine 5 Portfolio**
- **Unreal Engine 5 버전 : 5.4.4**
- **에디터 : VSCode, Rider**
- **개발 인원 : 1인개발**

소울라이크 액션을 레퍼런스로 한 액션 RPG     
GAS 기반의 전투 시스템, 동적 키 바인딩, FastArray 기반 인벤토리, MVC 구조의 UI 연동, ObjectPooling 적 리스폰 등을 구현
## InventorySystem
![InventorySystem](Images/InventoryComponent1.png)     

각 캐릭터에 `UInventoryComponent`를 통해 **InventorySystem**을 구현한다.      
플레이어는 **PlayerState**, 적은 **Character** 클래스에 `UInventoryComponent`를 소유한다.     
캐릭터가 소지하고 장착하는 모든 로직을 `UInventoryComponent`에서 관리한다.        
`UInventoryWidgetController`를 통해 **Widget**과 상호작용 할 수 있다.

![ItemInstance](Images/itemInstance.png)        

**ItemInstance**는 아이템의 데이터를 모아서 개념을 담당하는 `UObject`입니다.        
3가지로 구분되어 관리됩니다.
1. **InventoryData**        
    * 인벤토리에서 아이템의 데이터를 저장하는 구조체
    * 아이템의 변화에 따라 동적으로 변화
    * 리플리케이션 됨
2. **ItemData**
    * 변하지 않는 아이템 데이터를 저장하고 있는 `UObject`
    * 언리얼 리플렉션의 **RTTI**를 사용하기 위해 `FTableRow` 구조체를 `UObject`로 변환해서 사용
3. **ItemActor**
    * 아이템이 월드에 존재할 때만 사용
    * 해당 클래스를 상속받은 액터를 통해 기능 추가

중복적인 `UItemData` 생성을 막기 위해 `UItemDataAsset` 데이터애셋 객체에서 관리     

### 구현 코드

#### ItemDataAsset.cpp
<details>
    <summary>코드 보기</summary>

```c++
//ItemDataTable을 저장하는 구조체. 해당 DataTable에 맞는 UItemData의 Class를 저장
USTRUCT(BlueprintType)
struct FItemDataTable
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UDataTable> DataTable;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag ItemTypeTag = FGameplayTag();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UItemData> ItemDataClass;
};

...
//FSL_ItemData의 하위구조체를 기반으로 UItemData를 생성하고 초기화 해주는 함수
UItemData* UItemDataAsset::FindItemDataFromIndexAndItemType(const UObject* Outer, const FGameplayTag& ItemType, const FName& ItemID)
{
    for(const FItemDataTable& ItemDataTableStruct : ItemDataTables)
    {
        if(ItemType.MatchesTagExact(ItemDataTableStruct.ItemTypeTag))
        {
            FSL_ItemData* ItemData = ItemDataTableStruct.DataTable->FindRow<FSL_ItemData>(ItemID, FString("Not Found"));
            if(ItemData == nullptr) return nullptr;
    
            if (Outer && ItemDataTableStruct.ItemDataClass != nullptr)
            {
                //저장해 둔 TMap에 ItemID를 통해 UItemData재활용
                if (TObjectPtr<UItemData>* FoundItemPtr = ItemDataToItemID.Find(ItemData->ItemID))
                {
                    if (IsValid(*FoundItemPtr))
                    {
                        return *FoundItemPtr;
                    }
                    ItemDataToItemID.Remove(ItemData->ItemID);
                }
                
                UItemData* ItemDataObject = NewObject<UItemData>(const_cast<UObject*>(Outer), ItemDataTableStruct.ItemDataClass.Get());
                ItemDataObject->Init(ItemData);
                
                //TMap에 저장해서 UItemData재활용
                ItemDataToItemID.Add(ItemData->ItemID, ItemDataObject);
    
                return ItemDataObject;
            }
        
        }
    }
    
    return nullptr;
}
```
</details>      

### 아이템 데이터를 UObject화 하는 이유
언리얼 엔진의 DataTable은 기본적으로 **FStruct**(`FTableRowBase`) 구조를 사용한다.       
하지만 해당 프로젝트에서는 FStruct를 **UObject**(`UItemData`)로 변환하는 구조로 구현했다.

* 리플렉션 시스템과 RTTI의 한계 극복     
    - 문제점: `FStruct`는 리플렉션 시스템`UStruct`에 등록되지만, UClass 기반이 아니기 때문에 **RTTI(RunTime Type Information)** 를 완벽하게 지원하지 않음.
    - 발생 문제: 언리얼 엔진의 핵심 기능인 `Cast<T>`을 사용할 수 없으며, 또한 블루프린트의 **Cast 노드**를 사용할 수 없음


* 다형성(Polymorphism) 및 확장성 확보
  - 해결책: 정적 데이터(`FStruct`)를 `UObject` 기반의 클래스(`UItemData`, `UWeaponData` 등)로 래핑하여 관리
  - 이점: 
    1. **안전한 캐스팅:** `Cast<UWeaponData>(ItemData)`와 같은 엔진 표준 캐스팅 기능을 C++와 블루프린트 모두에서 사용 가능.
    2. **상속 구조 활용:** 아이템 타입(무기, 방어구, 소모품)에 따른 데이터 계층 구조 설계 및 확장 용이.
    3. **블루프린트 호환성:** `UObject`의 메모리 관리(GC) 및 리플렉션 기능을 활용하여 블루프린트에서 사용 용이.


* **DataTable**에 저장된 `FSL_ItemData`를 `UItemData`로 변환 과정
  1. **ItemInstance를** 생성할 때 `FindItemDataFromIndexAndItemType()` 팩토리 함수 사용
  2. **ItemID**와 **ItemTag**로 `FSL_ItemData`를 탐색
  3. 탐색한 `FSL_ItemData`로 `UItemData`를 생성, 생성한 **ItemData**를 **ItemInstance**에 저장

### 구현 코드

#### SoulLikeItemTypes.h
<details>
    <summary>코드 보기</summary>

```c++
UCLASS(BlueprintType)
class UWeaponData : public UEquipmentData
{
    GENERATED_BODY()

public:
    
    virtual void Init(const FSL_ItemData* ItemData) override
    {
        Super::Init(ItemData);
        const FSL_WeaponData* WeaponData = static_cast<const FSL_WeaponData*>(ItemData);

        if(WeaponData == nullptr) return;
	    
        ...
    }
}
```
</details>

## UI
![WidgetController](Images/WidgetControllerDiagram-2.png)       
UI와 게임간의 상호작용 구현을 위해 **MVC패턴**을 채용      
각 기능의 **Widget**에 따라 `WidgetController`를 생성해 관리

### WidgetController        
- 게임 로직과 UI 사이를 중계해 주는 **Controller**
- **Model**은 **Widget**을 대상으로 명령을 내리지 않고 **Broadcast**를 통해 동시에 **Widget**에 메세지를 보냄
- **Widget**에서 `WidgetController`를 통해 **Model**의 **Logic**을 실행
- 싱글톤 패턴으로 구현

`WidgetController`는 `AHUD`에서 생성하고 관리.       
`UBlueprintFunctionLibrary`를 재정의 한 **Static Helper Functio**n을 사용해서 생성하거나 사용 가능

### 구현 코드

#### SoulLikeFunctionLibrary.cpp
<details>
    <summary>코드 보기</summary>

```c++
//WidgetController를 초기화 하기 위한 FWidgetControllerParams 생성 함수
bool USoulLikeFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                          ASoulLikeHUD*& OutSoulLikeHUD, FWidgetControllerParams& OutWCParams)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if(PC == nullptr) return false;
	
	OutSoulLikeHUD = Cast<ASoulLikeHUD>(PC->GetHUD());
	if(OutSoulLikeHUD == nullptr) return false;

	ASoulLikePlayerState* PS = PC->GetPlayerState<ASoulLikePlayerState>();
	if(PS == nullptr) return false;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet* AS = PS->GetAttributeSet();
	OutWCParams.PlayerController = PC;
	OutWCParams.PlayerState = PS;
	OutWCParams.AbilitySystemComponent = ASC;
	OutWCParams.AttributeSet = AS;
	
	return true;
}

UInventoryWidgetController* USoulLikeFunctionLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams Params;
    ASoulLikeHUD* SoulLikeHUD;
    const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
    if(bSuccessfulMakingParams)
    {
        return SoulLikeHUD->GetInventoryWidgetController(Params);
    }
    return nullptr;
}
```
</details>

#### SoulLikeHUD.cpp
<details>
    <summary>코드 보기</summary>

```c++
UInventoryWidgetController* ASoulLikeHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
    if(InventoryWidgetController == nullptr){
    
        InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
        InventoryWidgetController->SetWidgetControllerParams(WCParams);
        InventoryWidgetController->BindCallbacksToModels();
    }
    return InventoryWidgetController;
}
```
</details>

### WidgetController 상호작용 시퀀스

>![WidgetControllerSequenceDiagram](Images/WidgetControllerSequenceDiagram-2.png)     
> 장착 시, MVC 상호작용 시퀀스

결합도를 낮추고 코드의 재사용성을 높이기 위해 MVC 패턴을 기반으로 한 단방향 데이터 흐름 구조를 설계      
**Model(`InventoryComponent`)** 과 **View(`Widget`)** 는 서로를 전혀 모르며, `WidgetController`를 통해 소통   
- **데이터 흐름 (Data Flow)**
   - **Input (Control)**: View → Controller → Model
   - 유저 입력은 **함수 호출(Function Call)** 을 통해 명확한 명령으로 전달됩니다.<br/>
     Ex) 아이템 장착 요청 (`RequestEquip`), 스탯 포인트 투자
   - **Output (Update)**: Model → Controller → View
   - 데이터 변경은 **Delegate(Broadcast)** 를 통해 전파<br/>
     Ex) 인벤토리 슬롯 변경, 스탯 값 변화 등.

이로 인해 `InventoryComponent`와 `Widget`은 서로의 존재를 몰라도 상호작용이 가능      
또한 **Widget**과 `InventoryComponent`가 변경 되더라도 서로 의존성을 분리 했기 때문에 코드를 추가적으로 수정할 필요가 없음

## KeyBind

>![KeyBindChangeScreenShot](Images/KeyBindChangeScreenShot.png)      
Input을 대기하는 상태(InputMode_KeyBind)

![KeyBindFlow](Images/KeyBindFlow-2.png)      

**EnhancedInput**과 **GameplayTag**를 결합하여, 하드코딩 없는 유연한 입력 시스템 구현          
플레이어의 입력을 `InputTag`(`FGameplayTag`)로 정의해서, Ability에 대한 입력을 `InputTag`를 통해 동적으로 할당할 수 있도록 구현 

### 입력 파이프라인
입력별로 별도의 함수를 만들지 않고, 공통 함수가 `InputTag`를 전달받아 처리하는 구조를 설계
1. **Input Action 트리거** 
   - `InputComponent`에 Bind 할 때, `InputAction`에 해당하는 `InputTag` 매핑
   - 키 입력 시, 해당 Action에 매핑된 `Tag`를 파라미터(Payload)로 전달

2. **InputTag 확인**
    - `PlayerController`와 `ASC`(`AbilitySystemComponent`)는 전달받은 Tag를 트리거된 함수에 전달
    - 함수 흐름: `IA_Trigger` → `PC::AbilityInputTagPressed(InputTag)` → `ASC::AbilityInputTagPressed(InputTag)`

3. **Ability 활성화**
    - `ASC`는 현재 부여된 어빌리티 중, 전달받은 Tag를 DynamicTags로 가지고 있는 어빌리티를 찾아 활성화(`TryActivateAbility()`)

![KeyBindBeforeAfter](Images/KeyBindBeforeAfter.png)     
InputAction에 Ability가 고정적으로 매핑되어 활성화 되는 하드코딩에서
런타임에 Ability의 키 바인딩이 변경 가능한 유연성있는 동적 바인딩으로 개선

### 구현 코드

#### SoulLikeInputComponent.h, SoulLikePlayerController.h
<details>
    <summary>코드 보기</summary>

```c++
template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
inline void USoulLikeInputComponent::BindAbilityActions(const USL_InputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
    check(InputConfig);
  
    //InputAction에 맞는 InputTag를 매개변수로 Callback함수를 Bind
    for(const FSL_InputAction& InputAction : InputConfig->InputActions)
    {
        if(PressedFunc) BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputAction.InputTag);
        if(HeldFunc) BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.InputTag);
        if(ReleasedFunc) BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.InputTag);
    }
}

void ASoulLikePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	USoulLikeInputComponent* SL_InputComponent = Cast<USoulLikeInputComponent>(InputComponent);

    ...
    //Event에 해당하는 3가지 Callback 함수 호출
	SL_InputComponent->BindAbilityActions(InputConfig, this,
		&ASoulLikePlayerController::AbilityInputTagPressed,
		&ASoulLikePlayerController::AbilityInputTagHeld,
		&ASoulLikePlayerController::AbilityInputTagReleased);
	
}
```
</details>

#### SoulLikeAbilitySystemComponent.cpp
<details>
    <summary>코드 보기</summary>

```c++
void USoulLikeAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!InputTag.IsValid()) return;

    //ActivateAbility가 RaceCondition에 의해 AbilitySpec이 변경될 수 있으므로 ScopeLock를 사용하여 Mutex Lock
    FScopedAbilityListLock ActivateScopeLock(*this);
    for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            ...
            AbilitySpecInputPressed(AbilitySpec);
            if(!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}
```
InputAction에 해당하는 InputTag를 바인드해서, InputAction이 트리거 될 때 InputTag를 기반으로 Ability를 활성화
</details>

### 단축키 런타임 동적 바인딩
![KeyRebindDiagram](Images/KeyRebindDiagram.png)        
어빌리티에 대해 입력 매핑을 하드코딩 하지 않고, `FGameplayTag`를 통해 동적으로 구현하여      
`AbilitySpec`의 `DynamicAbilityTag`에 **InputTag**를 넣고 빼는 가벼운 연산만으로 키 바인딩을 변경 가능 하도록 구현        

* **바인딩**<br/>
    바인딩   : 특정 어빌리티가 `InputTag.Q`를 가지고 있다면, **Q키**를 눌렀을 때 그 어빌리티가 활성화<br/>
    리바인딩 : Q키를 E키로 바꾸고 싶다면, 어빌리티의 태그 컨테이너에서 `InputTag.Q`를 제거하고 `InputTag.E`를 추가<br/>

* **InputTag변경 알고리즘**<br/>
   기존에 어빌리티가 InputTag를 가지고 있다면, 키 충돌을 방지하기 위해 기존 연결을 자동으로 정리하는 로직을 구현

* **바인딩 변경 로직**
  1. **Search (검색)**: 변경하려는 키(`InputTag`)를 이미 사용 중인 어빌리티가 있는지 탐색   
  2. **Unbind (해제)**: 만약 있다면, 해당 어빌리티에서 태그를 제거하여 연결 해제 (중복 방지)       
  3. **Bind   (연결)**: 내가 선택한 어빌리티에 새로운 태그를 추가    
  4. **Sync (동기화)**: `MarkAbilitySpecDirty`를 호출하여 변경된 태그 정보를 서버와 동기화     

### 구현 코드

#### SoulLikePlayerController.cpp
<details>
    <summary>코드 보기</summary>

```c++
void ASoulLikePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
   ...
        //위 사진과 같이 키 입력을 대기하고있는 InputMode_KeyBind 일 경우, 입력된 InputTag로 선택된 Ability의 InputTag 설정
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
### SoulLikeAbilitySystemComponent.cpp
```c++
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
		
		//변경된 AbilitySpec 갱신
		MarkAbilitySpecDirty(*PrevAbilitySpec);
		
		KeybindMenuWidgetController->OnReceiveInputTagDelegate.Broadcast(PrevAbilityTag);
	}
	//변경할 어빌리티 태그에 InputTag추가
	if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(SelectedAbilityTag))
	{
		AbilitySpec->DynamicAbilityTags.RemoveTag(AbilityInfo->GetAbilityInputTag(SelectedAbilityTag));
		
		AbilityInfo->ChangeAbilityInputTag(SelectedAbilityTag, InputTag);

		AbilitySpec->DynamicAbilityTags.AddTag(InputTag);
		
		//변경된 AbilitySpec 갱신
		MarkAbilitySpecDirty(*AbilitySpec);
		
		//Widget을 Delegate를 통해 갱신
		KeybindMenuWidgetController->SelectedAbilityTag = FGameplayTag();
		KeybindMenuWidgetController->OnReceiveInputTagDelegate.Broadcast(SelectedAbilityTag);
	}
}
```
</details>

## ObjectPoolingSubsystem
![ObjectPoolingSubsystem](Images/ObjectPoolingSystem.png)   

적 생성 시스템을 **데이터 기반 패턴(Data-Driven)** 으로 설계해서 공통된 Enemy 객체가 **데이터 주입(Data Injection)** 을 통해 다양한 적을 생성할 수 있도록 구현     
또한 **ObjectPool**을 통해 공통된 Enemy 객체를 재활용 하여 최적화 할 수 있도록 구현

### 데이터 주입(Data Injection) 식 생성
![SpawnActorRequire](Images/SpawnActorRequire.png)      
- **적 객체와 데이터의 분리**
    - 적 객체를 특정개체에 한정하지 않고, 초기화 되지 않은 기본 Enemy 클래스로 생성
    - 생성 후 `SpawnActorDeferred`를 통해 생성이 완료하기 전, 적 데이터 주입을 통해 먼저 적의 외형과 능력치를 제공.

>#### Enemy Data Table
>![EnemyData](Images/EnemyData.png)
>```c++
>void ASoulLikeEnemy::Init(AActor* InSpawnerActor, FEnemyData Data)
>{
>	EnemyData = Data;
>	SpawnerActor = InSpawnerActor;
>	//EnemyData로 ASoulLikeEnmey를 초기화
>	SetupActorWithEnmeyData();
>	...
>}
>```
>DataTable에 EnemyData를 정의하여 생성할 때 사용

이러한 구현으로 인해 코드를 추가하거나 수정하지 않고 데이터 테이블을 추가하는 방식으로 적의 종류를 자유롭게 확장 가능

### ObjectPool을 통한 최적화
![ObjectPoolSequenceDiagram](Images/ObjectPoolSequenceDiagram.png)      

적의 생성과 소멸이 빈번하게 발생하므로, **ObjectPool**을 사용해 재사용 로직 구현
또한 적 데이터를 설정하는 로직과 생성 함수의 실행 순서를 고정하여 발생할 수 있는 오류 차단

- **ObjectPool을 활용한 Generic Enemy Instance 재사용**
  - 적이 스폰해야 할 때, 비활성화 되어있는 적 객체를 Dequeue해서 데이터 주입 후 활성화 해서 재사용
  - 스폰 시 ObjectPool의 크기를 넘어서는 생성을 하게 될 때, Pool에서 꺼내서 재사용 할 수 있도록 구현

- **Enemy 생성 주기 수동 제어**
  - `SpawnActorDeferred`를 통해 액터 생성시 호출되는 초기화 함수의 순서를 직접 제어
  - **Race Condition**을 방지하고 명확한 순서로 인해 버그 발생시 디버그 용이

#### ObjectPoolingSubsystem.h
<details>
    <summary>코드 보기</summary>

```c++
ASoulLikeEnemy* UObjectPoolingSubsystem::SpawnEnemy(AActor* SpawnerActor, const TSubclassOf<ASoulLikeEnemy>& SpawnActorClass, FName RowName)
{
	FScopeLock Lock(&PoolLock);
	
	if(SpawnerActor == nullptr) return nullptr;
	if(!SpawnActorClass->IsValidLowLevel()) return nullptr;
	
	FEnemyData* Data = EnemyDataAssset->EnemyDataTable->FindRow<FEnemyData>(RowName, FString(""), false);
	if(Data == nullptr) return nullptr;
	
 	if(EnemyObjectPool.Num() < MaxPoolSize)
	{
		FTransform SpawnTransform = SpawnerActor->GetTransform();
		
		if(UWorld* World = SpawnerActor->GetWorld())
		{
			ASoulLikeEnemy* EnemyActor = World->SpawnActorDeferred<ASoulLikeEnemy>(Data->EnemyClass, SpawnTransform, SpawnerActor);
			if(IObjectPoolingInterface* Interface = Cast<IObjectPoolingInterface>(EnemyActor))
			{
				if(Data != nullptr)
				{
					EnemyActor->Init(SpawnerActor, *Data);
				}
				EnemyActor->OnDisabledObjectDelegate.AddDynamic(this, &UObjectPoolingSubsystem::OnEnemyDisabledObject);
				EnemyActor->SpawnDefaultController();
				EnemyActor->FinishSpawning(SpawnTransform);
			
				EnemyObjectPool.Add(EnemyActor);
				return EnemyActor;
			}
		}
	}
	else
	{
 		if(!EnemyObjectPoolQueue.IsEmpty())
		{
			AActor* PooledObject  = nullptr;
			if(EnemyObjectPoolQueue.Dequeue(PooledObject))
			{
				if(ASoulLikeEnemy* EnemyActor = Cast<ASoulLikeEnemy>(PooledObject))
				{
					EnemyActor->SetActorTransform(SpawnerActor->GetTransform(), false,  nullptr, ETeleportType::ResetPhysics);
					if(Data != nullptr)
					{
						EnemyActor->Init(SpawnerActor, *Data);
					}
					EnemyActor->SpawnDefaultController();
					return EnemyActor;
				}
			}
		}
	}
	return nullptr;
}
```
</details>

### Enemy Pool반환
![ObjectPoolDeathEnemy](Images/ObjectPoolDeathEnemy.png)      
적 객체가 체력이 0이 되어서 처지 되었을 경우, **Broadcast**를 통해 다시 Pool로 반환       
- **Delegate를 사용한 반환**
    - `ObjectPoolingSubsystem` 이 모든 적의 상태를 Tick마다 확인하지 않고 **Broadcast**를 통해 반환
    - 적 캐릭터가 초기화 될 때, Bind 유무를 확인하고 죽었을 때 Pool에 반환시키는 함수 Bind

Enemy가 직접 `ObjectPoolingSubsystem`을 참조 하지 않고, 메세지를 통해 반환되므로 결합도를 낮춤

### Pool ScopeLock
![ObjectPoolingSubsystem_EnemySpawn](Images/ScopeLock.png)     
여러개의 **SpawnPoint**가 동시에 하나의 **Subsystem**(싱글톤 객체)의 Pool Queue에 접근, 사용할 경우 **Race Condition**이 발생할 수 있기 때문에     
`ScopeLock`을 통해 **PoolQueue**를 사용하는 동안에 다른 접근을 차단

<details>
    <summary>코드 보기</summary>

```c++
ASoulLikeEnemy* UObjectPoolingSubsystem::SpawnEnemy(AActor* SpawnerActor, const TSubclassOf<ASoulLikeEnemy>& SpawnActorClass, FName RowName)
{
        ...
        if(Data != nullptr)
        {
            EnemyActor->Init(SpawnerActor, *Data);
        }
        EnemyActor->OnDisabledObjectDelegate.AddDynamic(this, &UObjectPoolingSubsystem::OnEnemyDisabledObject);
}

void UObjectPoolingSubsystem::OnEnemyDisabledObject(AActor* Actor)
{
	FScopeLock Lock(&PoolLock);
	
	EnemyObjectPoolQueue.Enqueue(Actor);
}

```
</details>

## Save/Load System

게임 진행 상황을 저장하기 위해 `USaveGame`을 사용하여 `USaveGame` 기반 직렬화 시스템 구현       
또한 Subsystem을 사용하여 주기적으로 자동 저장을 하도록 구현      
짧은 주기의 저장으로 인한 오버헤드를 줄이기 위해 관련 최적화 기능 구현        

**1.USaveGame**
- SaveGame은 3가지를 중심적으로 저장
  1. Player정보 : `PlayerState`,`AbilitySystemComponent`
  2. Inventory정보 : `InventoryComponent`
  3. SavedActor정보 : `ISaveInterface`

- 저장할 때 Character에서 `SaveProgress`를 통해 세이브데이터를 갱신
- 이후 `SaveWorldObject()`를 통해 World의 `ISaveInterface`를 상속받은 액터 직렬화 및 저장

<details>
    <summary>코드 보기</summary>

**1. ASoulLikeCharacter::SaveProgress() 호출**
```c++
void ASoulLikeCharacter::SaveProgress_Implementation() const
{
    if(!IsLocallyControlled()) return;
    ASoulLikeGameModeBase* AuraGameMode = Cast<ASoulLikeGameModeBase>(UGameplayStatics::GetGameMode(this));
    ASoulLikePlayerState* AuraPlayerState = Cast<ASoulLikePlayerState>(GetPlayerState());
    if(!IsValid(AuraGameMode)) return;
    if(AuraGameMode && AuraPlayerState)
    {
        USoulLikeSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
        if(SaveData == nullptr) return;		
    
        SaveData->bFirstTimeLoadIn = false;
        
        if(AuraPlayerState->IsDirty())
        {
            //PlayerState 저장
            SaveData->ProfileName = AuraPlayerState->GetProfileName();
            SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
            SaveData->EXP = AuraPlayerState->GetExp();
            SaveData->MaxPotion = AuraPlayerState->GetMaxPotion();
            
            SaveData->Vigor = USoulLikeAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
            SaveData->Mind = USoulLikeAttributeSet::GetMindAttribute().GetNumericValue(GetAttributeSet());
            SaveData->Endurance = USoulLikeAttributeSet::GetEnduranceAttribute().GetNumericValue(GetAttributeSet());
            SaveData->Strength = USoulLikeAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
            SaveData->Dexterity = USoulLikeAttributeSet::GetDexterityAttribute().GetNumericValue(GetAttributeSet());
            SaveData->Intelligence = USoulLikeAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
            
            //Player의 Ablity 저장
            USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(AbilitySystemComponent);
            FForEachAbility SaveAbilityDelegate;
            SaveData->SavedAbilities.Empty();
            SaveAbilityDelegate.BindLambda([this, SL_ASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
            {
                const FGameplayTag AbilityTag = SL_ASC->GetAbilityTagFromSpec(AbilitySpec);
                UAbilityInfo* AbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfo(this);
                FSoulLikeAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
            
                FSavedAbility SavedAbility;
                SavedAbility.GameplayAbility = Info.Ability;
                SavedAbility.InputTag = SL_ASC->GetInputTagFromAbilityTag(AbilityTag);
                SavedAbility.AbilityTag = AbilityTag;
                SavedAbility.AbilityType = Info.AbilityType;
            
                SaveData->SavedAbilities.AddUnique(SavedAbility);
            });
            SL_ASC->ForEachAbility(SaveAbilityDelegate);
            
            //Player InventoryComponent 저장
            if(InventoryComponent)
            {
                SaveData->SavedItems = InventoryComponent->GetSavedItemFromInventoryList();
                
                SaveData->RightWeaponSlotIndex = InventoryComponent->GetRightWeaponSlotIndex();
                SaveData->LeftWeaponSlotIndex = InventoryComponent->GetLeftWeaponSlotIndex();
                SaveData->ToolSlotIndex = InventoryComponent->GetToolSlotIndex();
            }
        }
        //Player위치 저장
        SaveData->Transform = GetActorTransform();
        AuraGameMode->SaveInGameProgressData(SaveData);
        
        AuraPlayerState->MarkAsClean();
    }
}
```
2. ASoulLikeGameModeBase::SaveInGameProgressData()로 SaveGame 갱신
```c++
void ASoulLikeGameModeBase::SaveInGameProgressData(USoulLikeSaveGame *SaveObject) const
{
	USoulLikeGameInstance* SoulLikeGameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());

	SoulLikeGameInstance->SetClientSaveData(SaveObject);

	const FString InGameLoadSlotName = FString::Printf(TEXT("%s_%d"), *SoulLikeGameInstance->LoadSlotName, SoulLikeGameInstance->LoadSlotIndex);
	const int32 InGameLoadSlotIndex = SoulLikeGameInstance->LoadSlotIndex;

	SaveWorldObject(GetWorld(), SaveObject);
	
	UGameplayStatics::AsyncSaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}
```

</details>

**2.FSavedActor**</br>
저장해야 하는 객체들을 직렬화 된 Byte와 액터의 이름을 저장하는 `FSavedActor`로 구조체화 시켜서 저장
- **ISaveInterface** : 저장해야 하는 캐릭터를 제외한 Actor는 이 인터페이스를 구현 
- 직렬화은 `SaveWorldObject()` 함수에서 진행하며, UWorld에 존재하는 `ISavedInterface`를 구현한 모든 액터를 대상으로 함
- **직렬화 단계**:
  1. `FActorIterator`를 사용하여 World 내의 모든 Actor를 For문으로 순회
  2. 해당 액터가 `ISavedInterface`를 구현했다면 직렬화 진행
  3. 직렬화 된 `FSavedActor`를 SaveGame에 저장

<details>
    <summary>코드 보기</summary>

**1. FActorIterator(World)를 통해 월드에 모든 세이브 가능한 객체를 직렬화 하여 FSavedActor에 저장**
```c++
void ASoulLikeGameModeBase::SaveWorldObject(UWorld* World, USoulLikeSaveGame* SaveGame) const
{
    if(World == nullptr) return;
    USoulLikeGameInstance* SL_GameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
    check(SL_GameInstance);
    
    if(SaveGame)
    {
        TArray<FSavedActor> SavedActors;
        for(FActorIterator It(World); It; ++It)
        {
            AActor* Actor = *It;
    
            if(!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
    
            if(TScriptInterface<ISaveInterface> SaveInterface = Actor)
            {
                if(!SaveInterface->IsDirty() || Actor->Implements<ASoulLikePlayerState>())
                {
                    continue;
                }
            }
        
            FSavedActor SavedActor;
            SavedActor.ActorName = Actor->GetFName();
            SavedActor.bIsUsedObject = ISaveInterface::Execute_GetIsUsedObject(Actor);
            
            FMemoryWriter MemoryWriter(SavedActor.Bytes);

            FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
            Archive.ArIsSaveGame = true;

            Actor->Serialize(Archive);
            
            SaveGame->SavedActorsMap.Add(SavedActor.ActorName, SavedActor);
        }
    }
}
```

</details>

**3.[트러블 슈팅]자동 저장 병목 현상 최적화**</br>

![beforeInsight](images/beforeInsight.png)
- **문제 발생**:
  - 2000개 이상의 액터를 배치해 자동저장 로직을 테스트 해 보니, **Hitching** 발생
  - 자동 저장이 계속 될 수록 프리징 시간이 길어짐
1. **자료구조 개선을 통한 최적화**
   - **현상** : 저장을 진행할 수록 SaveSlot의 크기와 늘어나는 것을 확인
   - **원인** : 기존에 `FSavedActor`를 저장하는 컨테이너가 `TArray`이고 해당 컨테이너에 누적해서 저장하고 있는 로직을 확인<br/>
   또한 로드 로직에서 for를 사용해서 모든 Array Index를 순회해서 시간복잡도가 0(NM)이 되어버리는 문제 확인 
   - **해결** : `TArray`에서 `TMap`을 사용해 로드시 시간복잡도를 개선하고, `TMap`은 `TSet`을 기반으로<br/> 
   하므로 Key에 중복된 Value가 누적되는 오류도 해결가능

<details>
    <summary>코드 보기</summary>

SaveGame의 SavedActor저장 컨테이너 변경
```c++
    //SoulLikeSaveGame.h
    //기존 코드
    //UPROPERTY()
    //TArray<FSavedActor> SavedActors;
    
    //TMap으로 개선
    UPROPERTY()
    TMap<FName, FSavedActor> SavedActorsMap;
    
//SoulLikeGameModeBase.cpp   
void ASoulLikeGameModeBase::SaveWorldObject(UWorld* World, USoulLikeSaveGame* SaveGame) const
{
    ...
    
    //SaveGame->SavedActorsMap.Add(SavedActor);	
    SaveGame->SavedActorsMap.Add(SavedActor.ActorName, SavedActor);
}
```

</details>

2. **비동기 함수로 전환**<br/>
1번째 수정으로 저장시간은 줄었지만, 근본적인 문제는 게임스레드가 멈추는 것
   - 저장할 때 동기함수 `UGameplayStatics::SaveGameToSlot()`를 사용해, 게임스레드와 렌더스레드가 Hitching되는 문제
   - **해결** : 비동기 함수 `UGameplayStatics::AsyncSaveGameToSlot()`를 통해 다른 Thread에서 처리

<details>
    <summary>코드 보기</summary>

```c++
void ASoulLikeGameModeBase::SaveInGameProgressData(USoulLikeSaveGame *SaveObject) const
{

    ...
    
    //동기함수에서 비동기함수로 개선
    //UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
    UGameplayStatics::AsyncSaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}
```
</details>

3. **직렬화 비용 절감**<br/>
![BeforeSerialize](images/BeforeSerialize.png)      
- **문제 발생** : 모든 세이브 가능 객체가 직렬화 진행
- **원인** : 데이터가 변하지 않은 객체까지 저장하게 되어서 직렬화가 필요 이상으로 진행
- **해결** : 데이터가 변화한 객체만 직렬화를 시키도록 `SaveInterface`를 통해 구현

<details>
    <summary>코드 보기</summary>

**1. SaveInterface에 IsDirty()를 통해 변경사항이 있는지 체크하는 로직 추가/상속받은 클래스에도 bIsDirty 변수 정의**
```c++
//SaveInterface.h
class SOULLIKE_API ISaveInterface
{
	GENERATED_BODY()

public:
    
    ...
    
    virtual bool IsDirty() const = 0;
    virtual void MarkAsDirty() = 0;
    virtual void MarkAsClean() = 0;
};

//AutoSaveTestActor.h
UCLASS()
class SOULLIKE_API AAutoSaveTestActor : public AActor, public ISaveInterface
{
    GENERATED_BODY()
	
public:
    AAutoSaveTestActor();
    
    /** Save Interface */
    virtual bool IsDirty() const override;
    virtual void MarkAsDirty() override;
    virtual void MarkAsClean() override;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bIsDirty = true;
};

```
**2. bIsDirty에 따라 직렬화 유무를 결정**
```c++
    for(FActorIterator It(World); It; ++It)
    {
        AActor* Actor = *It;
    
        if(!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
    
        if(TScriptInterface<ISaveInterface> SaveInterface = Actor)
        {
            if(!SaveInterface->IsDirty() || Actor->Implements<ASoulLikePlayerState>())
            {
                continue;
            }
        }
        ...
    }
```

</details>

4. **최종 최적화 결과**<br/>
    
- **테스트 환경** :
    - 테스트 세이브 액터 1200개/ 600개 IsDirty True, 600개 IsDirty False
    - 자동 저장 주기는 5초, 게임 시작 후 1분 후 기준
- **결과** :  
    최적화 전
    ![beforeInsight](images/beforeInsight.png)    
    최적화 후
    ![BeforeSerialize](images/BeforeSerialize.png)

## Multiplayer Session (Sign-Based Summoning)

![MultiplayerSession](images/MultiplayerSession.png)        

**OnlineSubsystemSteam**을 활용해서 Listen Server에서 다크소울의 협력플레이를 구현     

**1.메타데이터 브로드캐스팅**

로비를 탐색해서 Join하는 구조가 아닌 세션을 해당 Client의 정보를 담는 컨테이너로써 활용      
- **Guest Sign Session**:
  - 게스트가 사인을 남기면 Seesion을 생성해서 등록
  - 해당 Session은 접속을 위한 것이 아니라 데이터를 저장하고 현재 이 클라이언트가 협력 요청을 대기중임을 표기함
  - 세션을 생성할 때 `FOnlineSessionSettings` 에 캐릭터 이름과 레벨, 사인의 위치 정보등을 삽입하여 session을 생성

- **Sign생성(Server)**:
  - 호스트는 `FindSessio`n을 통해 SignSession을 탐색
  - 탐색성공시, `FOnlineSessionSettings`을 파싱해서 관련 데이터를 기반으로 `SignActor`를 생성

<details>
    <summary>코드 보기</summary>

</details>

**2. 소환 프로세스**

1. Guest는 Sign생성 아이탬을 사용해서 SignSession을 등록
2. Host가 SignSession을 탐색, 탐색 후 SignActor 생성
3. Host가 생성된 SignActor 상호작용, Guest의 `NetId`로 초대 전송
4. Guest가 초대를 수락할 경우, SignSession을 파괴하고 Host의 Session으로 **JoinSession**

<details>
    <summary>코드 보기</summary>

</details>

**3. [트러블 슈팅] Steam Subsystem의 InviteReceived 델리게이트 미작동**
- **문제 발생**:
    - 기존 구상한 계획에는 다크소울 같이 초대를 수락하는 것이 아니라, Sign을 작동한 시점부터 동의한것으로 간주하고 자동으로 소환되어야 함
    - 하지만 `OnlineSubsystemSteam`의 경우, 초대를 받기만 해도 Callback되는 델리게이트가 작동을 하지 않음
- **해결**:
    - Guest에게 Steam게임초대를 보내는 방법으로 우회 구현
    - Steam 자체의 초대수락 시스템을 통해 `OnSessionUserInviteAcceptedDelegates`를 사용하여 Host의 Session에 Join
    - Steam친구초대 기반이지만, 게임내에서 소환하는 것처럼 느끼도록 구현


## MeleeTrace
전투의 핵심인 공격판정을 **Collision Overlap**후 SweepTrace으로 검증하고 **NetRole**과 서버환경에 따른 3가지 분기로 나눠 지도록 구현      

### MeleeTrace구현
![MeleeTraceOverlap](Images/MeleeTraceOverlap.png)        

- **Montage**와 **AnimNotifyState**를 사용하여 공격 구간 정의
- 무기의 공격판정 콜리전을 활성화 되었을 때 **Overlap**이벤트 감지 시, Collision 양 쪽 끝을 **SweepTrace**로 최종 검증 진행
- 검증이 완료됬다면, 피격 대상에게 판정 적용(데미지, 경직 Montage)

### 네트워크 및 캐릭터 별 구현
공격판정시 본 트랜스폼 새로고침의 최소화와 치팅감지를 위해 **MeleeTrace**의 구현을 3가지로 분리

- **Case 1: Authority(Host/Listen Server)**
    - 자기자신이 서버이므로 즉시 판정
    - 별도의 추가 로직 없이 Overlap후 Trace로 공격판정
  
- **Case 2: Autonomous Proxy(Client/Host or Dedicated Server)**
    - 캐릭터 상대 좌표 계산으로 치팅방지
    - 데디케이티드 서버는 렌더링 하지 않으므로 **Bone Transform**을 갱신하지 않고, <br/> 
  만약 클라이언트가 치팅을 통해 불가능한 판정을 시도할 수 있음
    - 서버에게 **ServerRPC**로 클라이언트 캐릭터의 상대적인 공격판정 콜리전의 시작과 끝 위치를 전송,<br/>
    서버에선 서버의 캐릭터를 기준으로 다시 월드 트랜스폼으로 계산 후 Trace판정 진행
    - 만약 판정에 성공한다면, 서버에서 판정 적용(데미지, 경직 Montage)
  
- **Case 3: AI Enemy(SimulatedProxy/Dedicated Server)**
    - 렌더링 된 Client에서 초기 Overlap판정을 진행해야 하는데,<br/> AI Character는 **ServerRPC**를 사용할 수 없음
    - 즉, 데디케이티드 서버에서 적AI의 공격판정을 독자적으로 판정해야 함.


| 구분         | 대상 | 네트워크 권한 (Role)     | 로직            | 상세                                                                                                   |
|:-----------| :--- |:-------------------|:--------------|:-----------------------------------------------------------------------------------------------------|
| **Case 1** | **Host / Authority** | `Authority`        | **즉시 판정**<br> | 자기 자신이 서버이므로 별도의 보정 없이<br>**Overlap** 즉시 **Trace**를 수행하여 판정                                                  |
| **Case 2** | **Client Player** | `Autonomous Proxy` | **서버 검증**<br> | 클라이언트가 타격 시점의 상대 좌표(`Relative Transform`)를 **ServerRPC**로 전송<br>서버는 이를 서버 캐릭터 기준 월드 좌표로 재구성하여 검증 **Trace** 수행  |
| **Case 3** | **AI Enemy** | `Simulated Proxy`  | **서버 최적화**    | **[RPC 불가 해결]** AI는 Client 소유가 아니므로 **ServerRPC** 사용 불가<br>서버가 독자적으로 판정하되, **MontagePlay**중 일때만 **Bone**을 갱신하여 성능 확보 |

<details>
    <summary>코드 보기</summary>

#### ItemActor.h
```c++
    void AItemActor::OnOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
    {
    if(OtherActor == GetOwner()) return;
    
    
    FTransform TipStartTransform = MeshComponent->GetSocketTransform("TipStart", RTS_World);
    FTransform TipEndTransform = MeshComponent->GetSocketTransform("TipEnd", RTS_World);
    
    bool bRelative = false;
    if(GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
    {
        // 클라이언트에서의 무기와 Owner와의 상대적인 Transform을 보내서 서버에서 캐릭터의 위치를 기반으로 socket의 Transform을 다시계산해서 검증
        bRelative = true;
        FTransform ParentWorldTransform = GetOwner()->GetActorTransform();
        TipStartTransform = TipStartTransform.GetRelativeTransform(ParentWorldTransform);
        TipEndTransform = TipEndTransform.GetRelativeTransform(ParentWorldTransform);
    }
    
    UWeaponData* WeaponData = Cast<UWeaponData>(ItemData);
    
    const float Radius = WeaponData ? WeaponData->Radius : 5.f;
        
    if(GetOwner()->Implements<UCombatInterface>())
    {
        ICombatInterface::Execute_MeleeTrace(GetOwner(), TipStartTransform, TipEndTransform, Radius, bRelative);
    }
}
```

#### SoulLikeCharacterBase.h
```c++
void ASoulLikeCharacterBase::TryMeleeTrace(const FTransform& TraceStartTransform,
	const FTransform& TraceEndTransform, float Radius, bool bRelativeLoc)
{       
    const bool bDebug = static_cast<bool>(CVarShowAttackTrace.GetValueOnAnyThread());
    
    TArray<FHitResult> HitResults;
    
    FVector TraceStart = TraceStartTransform.GetLocation();
    FVector TraceEnd = TraceEndTransform.GetLocation();
    
    //상대처리가 필요 여부에 따라 추가 로직
    if(bRelativeLoc)
    {
        FTransform RelativeTraceStartTransform = TraceStartTransform * GetActorTransform();
        FTransform RelativeTraceEndTransform = TraceEndTransform * GetActorTransform();
    
        TraceStart = RelativeTraceStartTransform.GetLocation();
        TraceEnd = RelativeTraceEndTransform.GetLocation();
    }
    ...
    
    //SweepMultiTrace 진행 후 피격 판정 검사
    
    ...
    
    //DamageEffect 적용
    FDamageEffectParams DamageEffectParams;

	DamageEffectParams.WorldContextObject = this;
	DamageEffectParams.DamageGameplayEffectClass = DamageGameplayEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponent();

	SetupDamageParams(DamageEffectParams);

	for(FHitResult& HitResult : HitResults)
	{
		if(HitResult.GetActor() == nullptr) continue;
		
		IgnoreActors.Add(HitResult.GetActor());

		USoulLikeAbilitySystemComponent* TargetASC = Cast<USoulLikeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor()));
		if(TargetASC && TargetASC->HasAnyMatchingGameplayTags(HitImmunityStateTags)) return;
		
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		DamageEffectParams.KnockbackForce = (HitResult.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		
		TargetASC->Server_ApplyDamageEffect(DamageEffectParams);
	}
}
```
</details>

### AI공격 판정과 데디케이티드 서버 최적화(트러블 슈팅)      
* **발생 문제** :  
    - 적 AI 캐릭터는 ServerRPC를 사용할 수 없음
    - 데디케이티드 서버는 렌더링 하지 않으므로, 기본적으로 Bone Transform을 갱신하지 않음
* **해결** :
    - 평상시에는 서버에서 Bone Transform을 새로고침 하지 않다가 Montage가 재생중일 때만 Bone Tranform을 갱신<br/>
      (VisibilityBasedAnimTickOption : OnlyTickMontagesAndRefreshBonesWhenPlayingMontages)
    - 이를 통해 서버의 부하를 최소한으로 하면서 서버에서 AI의 공격판정 구현할 수 있어짐


## GameplayAbilitySystem
GAS는 캐릭터의 액션/상태 구현을 위해 사용한 프레임워크
공격, 상호작용, 스테이터스, 아이템 사용, 타겟고정, 구르기등의 전투 기능 구현

### AbilityState(Combo & Input Buffering)
![AbilityState](Images/AbilityState.png) 
```c++
UENUM(BlueprintType)
enum class EAbilityState : uint8
{   
    EAS_None        UMETA(DisplayName = "None"),
    EAS_WaitInput   UMETA(DisplayName = "Input Wait"),
    EAS_NextAction  UMETA(DisplayName = "Next Action")
};
```

부드러운 콤보 연계를 위해 공격 몽타주 구간을 3단계 상태로 나누어 관리하는 **선입력 시스템** 을 구현

**EAbilityState**
- **None**: 입력 불가 구간 
- **WaitInput** (선입력 구간): 유저의 입력을 받아 **버퍼(Buffer)** 에 저장해두는 구간.
- **NextAction** (분기 구간): 다음 콤보로 넘어갈지 결정하는 구간.

**콤보 로직**
- **Input Buffering**: `WaitInput` 구간에 입력이 들어오면 `InputTag`를 Ability에 저장
- **Combo Branching**: 몽타주에서 `NextAction` 구간에 진입하면 로직을 수행
  - **선입력 있음**: 버퍼를 확인하여 현재 어빌리티를 종료하고 다음 콤보 어빌리티를 활성화
    - 해당 어빌리티가 현재 활성화 중인 어빌리티일 경우, 다음 Section 재생
  - **선입력 없음**: 입력을 대기하며, 입력 시 즉시 다음 콤보 활성화

<details>
    <summary>코드 보기</summary>

#### 1. 어빌리티 활성화 시 AnimNotify_MontageEvent 수신 대기
```c++
void USoulLikeComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	...
	
	WaitInputEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage_WaitInput);
	WaitInputEventTask->EventReceived.AddDynamic(this, &USoulLikeComboAbility::ReceiveWaitInputEvent);
	WaitInputEventTask->Activate();
	
	NextActionEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage_NextAction);
	NextActionEventTask->EventReceived.AddDynamic(this, &USoulLikeComboAbility::ReceiveNextActionEvent);
	NextActionEventTask->Activate();
}
```
#### 2. 이벤트 수신 시 Callback 함수 호출
```c++
void USoulLikeComboAbility::ReceiveWaitInputEvent(FGameplayEventData Payload)
{	
    ...
    //AbilityState 변경
    AbilityState = EAbilityState::EAS_WaitInput;
    
    //InputTag의 수신대기 Task
    WaitInputTask = UWaitInputTask::WaitInputTag(GetAbilitySystemComponentFromActorInfo());
    WaitInputTask->WaitInputTagDelegate.AddDynamic(this, &USoulLikeComboAbility::ReceiveInputTag);
    //활성화 된 어빌리티 InputPress 수신대기하는 Task
    InputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
    InputPressTask->OnPress.AddDynamic(this, &USoulLikeComboAbility::ReceiveInputPress);
    InputPressTask->Activate();
}
```
#### 3. Callback함수를 통해 분기 관리
```c++
void USoulLikeComboAbility::ReceiveInputPress(float TimeWaited)
{
    switch(AbilityState)
    {
        case EAbilityState::EAS_NextAction:
            //바로 다음 액션이 가능하다면, 다음 섹션 재생
            MontageJumpToNextCombo();
            break;
        case EAbilityState::EAS_WaitInput:
            //다음 콤보를 재생한다는 플래그
            bNextCombo = true;
            break;
        default:
            break;
    }
}

void USoulLikeComboAbility::ReceiveInputTag(const FGameplayTag& InInputTag)
{
    //InputTag Buffer
    InputTag = InInputTag;
}
```
#### 4. 
```c++
void USoulLikeComboAbility::ReceiveNextActionEvent(FGameplayEventData Payload)
{
    //AbilityState 변경
    AbilityState = EAbilityState::EAS_NextAction;
    
    //만약 같은 입력으로 인해 다음 섹션을 재생해야 한다면
    if(bNextCombo)
    {
        MontageJumpToNextCombo();
    }
    else
    {
        if(CheckAvatarInput()) return;
    
        //저장한 InputTag가 올바른 Tag라면
        if(InputTag.IsValid())
        {
            EndAbility(GetCurrentAbilitySpec()->Handle, CurrentActorInfo, CurrentActivationInfo, true, true);
    
            if(USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
            {
                SL_ASC->TryActivateAbilityFromInputTag(InputTag);
            }
        }
    }
}
```
</details>

### TargetLock
타겟방향으로 카메라를 회전시키는 어빌리티. 비동기 작업인 `AbilityTask`를 사용   
타겟을 변경하거나 고정하는 경우
>![TargetLockFromAim](Images/TargetLockFromAim.gif)       
>1. 애임으로 적을 직접 선택 했거나, 범위 내 가장 가까운 적에 고정        
<details>
    <summary>코드 보기</summary>

```c++
//TargetLockGameplayAbility.cpp

FHitResult AimHitResult;
FTraceProperties AnimTraceProperties(TraceLength, SingleTraceCollisionChannel, SingleTraceSphereRadius, DebugLifeTime);
FTraceProperties MultiTraceProperties(TraceLength, MultiTraceCollisionChannel, MultiTraceSphereRadius, DebugLifeTime);

if(USoulLikeFunctionLibrary::SingleTraceFromCameraLocation(GetAvatarActorFromActorInfo(), AimHitResult, AnimTraceProperties, bDebug))
{
    bIsTargetLock = true;
    if(AimHitResult.GetActor())
    {
        TargetActor = AimHitResult.GetActor();
    }
}

//SoulLikeFunctionLibrary.cpp
bool USoulLikeFunctionLibrary::SingleTraceFromCameraLocation(const AActor* Instigator, FHitResult& HitResult,
                                                             FTraceProperties TraceProperties, bool bDebug)
{
    FVector TraceStartLocation = FVector::ZeroVector;
    FVector TraceEndLocation = FVector::ZeroVector;
    FVector TraceForwardVector = FVector::ZeroVector;
    
    if(!SetupTraceProperties(Instigator, TraceStartLocation, TraceEndLocation, TraceForwardVector, TraceProperties.TraceLength)) return false;
    
    FCollisionQueryParams Params(NAME_None, false, Instigator);
    
    UWorld* World = Instigator->GetWorld();
    if(World == nullptr) return false;
    
    bool bResult = World->SweepSingleByChannel(
        HitResult,
        TraceStartLocation + (TraceForwardVector * TraceProperties.TraceSphereRadius),
        TraceEndLocation - (TraceForwardVector * TraceProperties.TraceSphereRadius),
        FQuat::Identity,
        TraceProperties.TraceCollisionChannel,
        FCollisionShape::MakeSphere(TraceProperties.TraceSphereRadius),
        Params);
    
    if(bResult)
    {
        if(HitResult.GetActor())
        {
            bResult = HitResult.GetActor()->IsA(ASoulLikeCharacterBase::StaticClass());
            
            if(bDebug)
            {
                FVector TraceVec = TraceForwardVector;
                FVector Center = TraceStartLocation + TraceForwardVector * TraceProperties.TraceLength / 2;
                float HalfHeight = TraceProperties.TraceLength / 2.f;
                FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
                FColor DrawColor = bResult ? FColor::Green : FColor::Red;
    
                DrawDebugCapsule(World,
                    Center,
                    HalfHeight,
                    5.0f,
                    CapsuleRot,
                    DrawColor,
                    false,
                    TraceProperties.DebugLifeTime);
            }
            
            return bResult;
        }
    }
    
    return false;
}
```
</details>

>![SwapTargetEnemy](Images/SwapTargetEnemy.gif)     
>2. 현재 타겟 고정이 되어있는 상태에서 다른 타겟으로 타겟을 변경
<details>
    <summary>코드 보기</summary>

```c++
//AbilityTask_TargetLock.cpp
void UAbilityTask_TargetLock::TryChangeTargetActor()
{
    if(TargetActor.IsValid() && !USoulLikeFunctionLibrary::CheckTargetOnViewport(TargetActor.Get(), 0.2f)) return;
    if(!bCanChangeTarget) return;
    
    bool LeftTrace = false;
    bool RightTrace = false;
    
    float NextTargetDegree = 0.f;
    
    ASoulLikeCharacter* Character = Cast<ASoulLikeCharacter>(GetAvatarActor());
    if(Character == nullptr) return;
    
    
    UCameraComponent* Camera = Character->GetPlayerCameraComponent();
    if(Camera == nullptr) return;
    
    if(Character->GetMouseXInput() > 8.f)
    {
        RightTrace = true;
        NextTargetDegree = 360.f;
    }
    else if(Character->GetMouseXInput() < -8.f)
    {
        LeftTrace = true;
        NextTargetDegree = -360.f;
    }
    
    if(!LeftTrace && !RightTrace) return;
    
    AActor* NewTargetActor = nullptr;
    TArray<FHitResult> HitResults;
    
    const bool bDebug = static_cast<bool>(CVarShowTargetLockTrace.GetValueOnAnyThread());
    USoulLikeFunctionLibrary::MultiTraceFromCameraLocation(Character, HitResults, TraceProperties, bDebug);
    
    for(const FHitResult& HitResult : HitResults)
    {
        AActor* CurrentTargetActor = HitResult.GetActor();
        if(CurrentTargetActor == nullptr) continue;
        
        float LookAtTargetRotatorYaw = FRotationMatrix::MakeFromX(CurrentTargetActor->GetActorLocation() - Camera->GetComponentLocation()).Rotator().Yaw;
        float CurrentTargetDegree = FRotator::ClampAxis(LookAtTargetRotatorYaw - Camera->GetComponentRotation().Yaw);
    
        if(bDebug)
        {
            DrawDebugString(GetWorld(), CurrentTargetActor->GetActorLocation() + FVector(0.f, 0.f, 20.f), FString::SanitizeFloat(CurrentTargetDegree), 0, FColor::White, 5.f);
        }
    
        if(TargetActor != CurrentTargetActor && 
            ((CurrentTargetDegree > 180.f && CurrentTargetDegree > NextTargetDegree && LeftTrace)||
                (CurrentTargetDegree < 180.f && CurrentTargetDegree < NextTargetDegree && RightTrace)))
        {
            NextTargetDegree = CurrentTargetDegree;
            NewTargetActor = CurrentTargetActor;
        }
    }
    
    if(NewTargetActor != nullptr && NewTargetActor != TargetActor && NewTargetActor->Implements<UCombatInterface>() && USoulLikeFunctionLibrary::CheckTargetOnViewport(NewTargetActor, 0.2f))
    {
        ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());
        TargetActor = NewTargetActor;
        
        ChangedTargetActorDelegate.Broadcast(TargetActor.Get());
        
        ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());
    
        bCanChangeTarget = false;
        GetWorld()->GetTimerManager().ClearTimer(LockOnTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(LockOnTimerHandle, FTimerDelegate::CreateLambda([&](){
            bCanChangeTarget = true;
        }), 1.f, false);
    }
}
```
</details>

>![OnDeathSwapTarget](Images/OnDeathSwapTarget.gif)
>3. 타겟이 죽었다면, 가장 가까운 적에게 타겟고정
<details>
    <summary>코드 보기</summary>

```c++
//AbilityTask_TargetLock.cpp
void UAbilityTask_TargetLock::OnDeathTargetActor()
{
    GetWorld()->GetTimerManager().ClearTimer(TargetDeathTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(TargetDeathTimerHandle, FTimerDelegate::CreateLambda([&]()
    {
        TArray<FHitResult> TargetHitResults;
        AActor* NewTargetActor = nullptr;
        const bool bDebug = static_cast<bool>(CVarShowTargetLockTrace.GetValueOnAnyThread());
        if(USoulLikeFunctionLibrary::MultiTraceFromCameraLocation(GetAvatarActor(), TargetHitResults, TraceProperties, bDebug))
        {
            float NearestDistance = 5000.f;
    
            for(const FHitResult& TargetHitResult : TargetHitResults)
            {
                AActor* CurrentTargetActor = TargetHitResult.GetActor();
                if(CurrentTargetActor == nullptr) return;
    
                float DistanceToTarget = FVector::Distance(GetAvatarActor()->GetActorLocation(), CurrentTargetActor->GetActorLocation());
    
                if(DistanceToTarget < NearestDistance)
                {
                    NearestDistance = DistanceToTarget;
                    NewTargetActor = CurrentTargetActor;
                }
            }
        }
    
        if(NewTargetActor && TargetActor.Get())
        {
            ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());
            TargetActor = NewTargetActor;
            ICombatInterface::Execute_ToggleTargetMark(NewTargetActor);
            if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
            {
                CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAbilityTask_TargetLock::OnDeathTargetActor);
            }
        }
        ChangedTargetActorDelegate.Broadcast(NewTargetActor);
    }), 1.f, false);
}
```
</details>

### InteractionAbility
![InteractionAbility](Images/InteractionAbility.png)      

여러 상호작용(사다리, 보물상자, NPC 대화 등)을 하나의 `Ability`에서 효율적으로 처리하기 위해, **AbilityTask**를 활용한 전략 패턴 구조로 설계     
`PreActivate`에서 상호작용 오브젝트의 `InteractionTag`를 통해 `FInteractionTaskInfo`를 가져오고, 해당하는 **Task**를 실행함으로써, 

#### 설계 구조
- **발생 문제** : 각각 다른 InteractionAbility를 추가하는 것은 비효율적이고 확장성이 떨어지게 됨
- **해결 방안** : InteractionAbility는 매니저가 되어 InteractionObject가 가진 InteractionTag에 따라 적절한 Task가 Interaction 로직을 수행(전략 패턴)
- Interaction마다 Ability를 추가하는 방법보다 쉽고 코드 수정없이 확장할 수 있는 구조. 확장성 증가

#### Interaction Interface
![InteractionInterface](Images/InteractionInterface.png)        

상호작용 기능이 필요한 클래스에 상속하여 구현해야 하는 인터페이스
- Interaction을 구분하는 `GameplayTag`를 저장시켜서 구분
- `Ineraction()` 함수를 각 InteractionObject에 구현하여 해당 클래스에 각각 다른 상호작용 구현
- InteractionObject이 Character와 Overlap된다면, Character에 InteractionActor 저장 하고 상호작용 어빌리티 작동시,<br/>
    저장된 InteractionActor을 Interaction 함수 호출하고 해당하는 `FInteractionTaskInfo` 가져옴

<details>
    <summary>코드 보기</summary>

1. InteractionInterface 
```c++
class SOULLIKE_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetInteractionTag() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FString GetActorName() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetWarpingLocation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator GetWarpingRotation() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interaction(AActor* InteractionActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const FInteractionTaskInfo GetInteractionActorInfo() const;
};
```
2. InteractionAbility 활성화 전 FInteractionTaskInfo 가져오기
```c++
void UInteractionGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	if(GetAvatarActorFromActorInfo()->Implements<UInteractionInterface>())
	{
		InteractionTaskInfo = IInteractionInterface::Execute_GetInteractionActorInfo(GetAvatarActorFromActorInfo());
		ActivationOwnedTags = FGameplayTagContainer(InteractionTaskInfo.StatusTag);
	}
	
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

const FInteractionTaskInfo ASoulLikeCharacter::GetInteractionActorInfo_Implementation() const
{
	if(SelectedInteractionActor == nullptr) return FInteractionTaskInfo();
	if(SelectedInteractionActor->Implements<UInteractionInterface>())
	{
		return Execute_GetInteractionActorInfo(SelectedInteractionActor);
	}
	
	return FInteractionTaskInfo();
}
```
</details>

>![InteractionTask](Images/AbilityInfoInteractionTask.png)      
> DataAsset "DA_AbilityInfo"에서 관리
> ```c++
> if(InteractionTaskInfo.InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder))
>	{
>		InteractionTask = UAbilityTask_Ladder::CreateLadderTask(this, GetAvatarActorFromActorInfo(), InteractionTaskInfo);
>	}
>	else
>	{
>		InteractionTask = UAbilityTask_Interaction::CreateInteractionTask(this, GetAvatarActorFromActorInfo(), InteractionTaskInfo);
>	}
> ```
> FGameplayTag로 구분해서 Interaction 실행

각 상호 작용은 별도의 InteractionAbility를 만들지 않고, 비동기 작업 AbilityTask를 통해 추가, 구현     

#### InteractionTask_Ladder
> ![InteractionTask_Ladder](Images/InteractionTask_Ladder.gif)      
> 사다리타기 상호작용

1. 구현
- AbilityTask_Interaction에서 Tick마다 위, 아래 InputValue을 가져와 LadderClimbMontageTask를 실행
- ClimbMontage에 각 상황별 Section으로 구분해 Section이동으로 자연스러운 움직임 구현 

<details>
    <summary>코드 보기</summary>

```c++
void UAbilityTask_Ladder::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);
    
    //Input을 받을 수 있는 상태이면서 ClimbLoop Animation이 활성화 되지 않았을경우
    //ClimbMontage의 중복작동 막음
    if(bCanReceiveInput &&
        (!IsValid(LadderClimbMontageTask) || !LadderClimbMontageTask->IsActive()))
    {
        FName SectionName = FName("");
        FString Direction = FString("");
        FString RL = FString("");
        
        if(Ability && Ability->GetAvatarActorFromActorInfo()->Implements<UCombatInterface>() && Ability->GetAvatarActorFromActorInfo()->Implements<UInteractionInterface>())
        {
            //InputValue를 가져옴
            float InputValue = ICombatInterface::Execute_GetLadderMoveInput(Ability->GetAvatarActorFromActorInfo());
            //입력이 없다면 함수종료
            if(InputValue == 0.f) return;
            //종결 Montage를 작동해야 하는지에 대한 여부를 확인하기 위해 현재 Overlay된 InteractionActor의 Tag가져옴
            FGameplayTag InteractionTag = IInteractionInterface::Execute_GetInteractionTag(Ability->GetAvatarActorFromActorInfo());
            
            if(InputValue > 0.f)
            {
                Direction = FString("Up");
                //만약 Overlap된 Interaction의 방향이 아래일경우, 위로 올라가는 종결Montage 재생
                if(InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder_Down))
                {
                    bCanReceiveInput = false;
                    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Ability->GetAvatarActorFromActorInfo(), InteractionTag, FGameplayEventData());
                    ICombatInterface::Execute_SetLadderMirror(Ability->GetAvatarActorFromActorInfo(), false);
                    ICombatInterface::Execute_SetMirror(Ability->GetAvatarActorFromActorInfo(), bIsRight);
                    return;
                }
            }
            else if(InputValue < 0.f)
            {
                Direction = FString("Down");
                //만약 Overlap된 Interaction의 방향이 위일경우, 아래로 내려가는 종결Montage 재생
                if(InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder_Up))
                {
                    bCanReceiveInput = false;
                    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Ability->GetAvatarActorFromActorInfo(), InteractionTag, FGameplayEventData());
                    ICombatInterface::Execute_SetLadderMirror(Ability->GetAvatarActorFromActorInfo(), false);
                    ICombatInterface::Execute_SetMirror(Ability->GetAvatarActorFromActorInfo(), bIsRight);
                    return;
                }
            }
            //현재 상태에 따른 LadderIdle Animation Mirror설정
            //ICombatInterface::Execute_SetLadderMirror(Ability->GetAvatarActorFromActorInfo(), !bIsRight);
        }
        
        if(bIsRight)
        {
            RL = FString("R");
        }
        else
        {
            RL = FString("L");
        }
        //현재 LadderIdle Mirror상태 스위치
        bIsRight = !bIsRight;
        
        SectionName = FName(*FString::Printf(TEXT("%sLoop_%s_%s"), *InteractionTaskInfo.SectionName, *Direction, *RL));
        
        LadderClimbMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, FName("LadderLoop"), InteractionTaskInfo.Montage, 1, SectionName);
        LadderClimbMontageTask->ReadyForActivation();
    }
}
```
</details>

2. **[트러블 슈팅]** 멀티플레이에서 사다리 이동 입력과 동기화
    - **발생 문제** :
        - Ladder의 경우, 추가적인 입력을 받아 위/아래로 움직여야 함
        - 클라이언트의 입력은 자동으로 서버에 전송 되지 않음
        - 결과적으로, 클라이언트와 서버사이에 위치 불일치가 발생

    - **해결** :
        - ServerRPC를 통해 클라이언트의 입력값을 전송
        - 단순하게 RPC를 통해 전송하면, 수많은 RPC로 인해 네트워크 오버헤드가 발생하므로, 입력값이 변하거나, 임계값 이상일 때만 전송

![InteractionLadderTrableShotting](Images/InteractionLadderTrableShotting.png)

<details>
    <summary>코드 보기</summary>

#### SoulLikePlayerController.cpp
```c++
void ASoulLikePlayerController::LadderMove(const FInputActionValue& InputActionValue)
{
	if(GetASC() && !GetASC()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Ladder))
	{
		LadderMoveInput = 0.f;
		return;
	}
	
	const float InputValue = InputActionValue.Get<float>();
	//기존 InputValue와 값이 비슷하다면, ServerRPC를 호출하지 않음
	if (FMath::Abs(InputValue - LadderMoveInput) > KINDA_SMALL_NUMBER)
	{
		LadderMoveInput = InputValue;
		Server_SetLadderMoveInput(InputValue);
	}
}
```
</details>

## 플레이 영상
https://www.youtube.com/watch?v=HrCWYucMnuY
