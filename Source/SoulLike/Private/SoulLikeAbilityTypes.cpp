﻿#include "SoulLikeAbilityTypes.h"

const FSoulLikeExecCalcCaptureStatics& FSoulLikeExecCalcCaptureStatics::CaptureStatics(){

	static FSoulLikeExecCalcCaptureStatics Statics;
	Statics.Init();
	return Statics;
}

bool FSoulLikeGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	
    if(Ar.IsSaving()){
    
        if (bReplicateInstigator && Instigator.IsValid())
        {
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
    	{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
    	if(DamageType.IsValid())
    	{
    		RepBits |= 1 << 7;
    	}
    	if(AttackForce > 0.f)
    	{
    		RepBits |= 1 << 8;
    	}
    	if(Toughness > 0.f)
    	{
    		RepBits |= 1 << 9;
    	}
    	if(!KnockbackForce.IsZero())
    	{
    		RepBits |= 1 << 10;
    	}
    }
    Ar.SerializeBits(&RepBits, 12);

    if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if(RepBits & (1 << 7))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if(RepBits & (1 << 8))
	{
		Ar << AttackForce;
	}
	if(RepBits & (1 << 9))
	{
		Ar << Toughness;
	}
	if(RepBits & (1 << 10))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}

    if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;

    return true;
}
