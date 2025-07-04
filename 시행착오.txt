# SoulLike

3월 1주차

Character 기본 구현<br>
ASC Character 구현<br>
Attribute, MMC 구현

MMC 클래스 통합

MMC_Secondary -> MMC_VitalAttributes로 개선<br>
Enemy AnimationBlueprint 임시 생성<br>
InputAction에 GameplayTag 매칭해서 Callback 하는 InputComponent 구현<br>
입력 받은 GameplayTag가 InputTag인 어빌리티 활성화<br>

ComboAbility 구현.<br>
어빌리티의 Montage 재생 중 추가 입력으로<br>
다음 액션(다음 콤보, 다른 어빌리티)를 실행시켜주는 어빌리티<br>
테스트 하기 위해 Hard Cording으로 Montage 재생<br>

Weapon에 따라 다른 Montage 실행을 위해 Inventory Component 구현<br>
FastArraySerializer를 사용해서 Replicate 또한 고려하는 아이템 컨테이너 구현.<br>
이때 아이템의 정보를 저장하고 서버-클라이언트에서 Replicate하는 과정에서 문제발생.

3월 2주차

문제해결방법<br>
처음에 시도한 방법은 Item의 모든 정보를 Replicate해서 클라이언트에 동기화<br>
해주는 방식이였는데, Struct->UObject로 정보를 저장하는 과정에서<br>
Replicate가 잘 되지 않는 문제와 너무 많은 정보를 Replicate해줘야 한다는 점 때문에<br>
클라이언트 서버 모두 ItemDataAsset을 가지고, ItemType과 ItemID만 Replicate해서<br>
각 인스턴스에 있는 DataAsset에서 검색해서 정보를 가져오도록 변경<br>

TODO<br>
그 이후 아이템에 추가적인 정보 또한 저장해줄 struct 필요<br>
해당 struct는 ItemInstance에서 저장될 예정<br>

3월 3주차

아에팀 관련해서 여러가지 시도를 해본 결과 기존의 방법을 사용하기로 함<br>
시도해 본 방법
1. UObject 자체를 Replicate 하는 방법
2. FStruct를 Replicate 하는법

해결 후 간단한 콤보 공격 어빌리티 구현<br>
몽타주에 SendEvent를 통해서 선입력을 받게 대기하거나, 특정 시점 이후에 다음 액션을 할 수 있도록 구현<br>
MMC를 사용해서 해당 어빌리티의 AbilityTag를 사용해 AbilityInfo에서 어빌리티 정보를 가져와서<br>
Stamina Cost를 설정 할 수 있도록 구현<br>

3월 4주차

AnimMontage에 MotionWarping추가<br>
이때 AnimNotifyState를 사용했는데 문제가 발생<br>
문제는 AnimNotifyState가 데디케이티드에서도 작동 하는것 뿐만아니라 <br>
클라이언트와 동시에 작동하지 않고 서버에서만 작동<br>

또한 MotionWarping Target을 설정하는 과정에서<br>
Pawn의 LastInputVector를 사용했는데<br>
해당 변수는 클라이언트에서만 저장되기 때문에 클라이언트=>서버 로 변수를 갱신해야 하는 문제 발생<br>

따라서 RPC를 사용해서 구현하려고 시도 >> RPC의 과다사용으로 인한 네트워크 부하 문제<br>
서버에서도 남아있는 CurrentAcceleration 을 사용해 WarpingTarget 설정<br>
==> 서버에도 있는 변수이므로 RPC사용할 필요 없음

4월 

WidgetController 구현<br>

MVC패턴을 구현하기 위해 WidgetController 구현<br>
WidgetController는 Widget과 AttributeSet같은 내부 로직사이에 델리게이트 등을 통해 상호작용한다.<br>
AbilitySystem이 Model, Widget이 View, WidgetController가 Controller 역활을 한다.<br>
WidgetController를 통해 Attribute의 변화를 Widget에 신호를 준다던가, <br>
InventoryComponent와 연동해서 인벤토리를 구현

InventoryWidget<br>
WBP_Inventory를 중복해서 사용하기 위해 GameplayTag를 사용해서<br>
InventoryComponent안에 있는 InventoryList에서 같은 ItemType을 가지고 있는 <br>
ItemInstance를 InventorySlot에 등록하고 초기화 함으로써 WBP_Inventory의 Slot을 갱신<br>
장비장착 또한 장착슬롯에 EquipSlot 태그로 분류해서 해당 슬롯에 장착될 수 있도록 구현.<br>
WidgetController를 사용해서 EquipSlot에 등록될경우, 캐릭터 또한 해당 ItemIntance를 스폰해서<br>
캐릭터가 장착할 수 있도록 함

UI를 구현하기 위한 명칭DataAsset
각 키워드의 한글이름을 저장하기 위해 DataAsset으로 GameInstance나 GameMode에 저장<br>

* DataAsset을 저장할 때 GameInstance와 GameMode로 분류하는 이유

GameInstance : Server, Client 모두 존재<br>
GameMode : Authority Instance에만 존재<br>

따라서 UI같이 클라이언트에도 존재해야하는 DataAsset은 GameInstance에 보관하고<br>
서버에만 존재해야 하는 DataAsset은 GameMode에 보관함

상호작용 구현<br>
어빌리티를 통해 상호작용을 구현<br>
상호작용은 이후 추가할 수 있도록 AbilityInfo DataAsset에 해당하는 상호작용 정보를 저장해서,<br>
GameplayTag로 분류해서 특정 상황에 맞는 어빌리티 실행<br>
상호작용 정보에는 Montage, InteractionTag, StatusTag 가 있고, <br>
해당하는 상호작용에 종료 Montage존재 여부또한 저장함<br>

AbilityTask_Interaction구현<br>
AbilityTask_Interaction으로 구현해서 Interaction이 추가되더라도, 간단하게 확장할 수 있도록 구현<br>
사다리 타기의 경우는 추가입력이 필요하므로 상속받아서 구현<br>
이때 기존 앞 뒤 입력인 W,S에 추가적으로 LadderStatus일경우에만 InputValue를 저장하도록<br>
구현하고, AbilityTask의 Tick마다 InputValue를 확인하고 위로 올라가거나 아래로 내려가도록 구현<br>


5월