// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_AttackTrace ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Weapon ECollisionChannel::ECC_GameTraceChannel2
#define ECC_TargetTrace ECollisionChannel::ECC_GameTraceChannel3

static TAutoConsoleVariable<int32> CVarShowTargetLockTrace(
	TEXT("ShowTargetLockTrace"),
	0,
	TEXT("Draws debug info about TargetLcok trace")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarShowAttackTrace(
	TEXT("ShowAttackTrace"),
	0,
	TEXT("Draws debug info about Attack trace")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarShowBackstabTrace(
	TEXT("ShowBackstabTrace"),
	0,
	TEXT("Draws debug info about Backstab trace")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

static TAutoConsoleVariable<int32> CVarShowRollDirection(
	TEXT("ShowRollDirection"),
	0,
	TEXT("Draws debug info about Roll Direction")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);