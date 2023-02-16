// Fill out your copyright notice in the Description page of Project Settings.


#include "KayDataAsset.h"

FPrimaryAssetId UKayDataAsset::GetPrimaryAssetId() const
{
	 //return Super::GetPrimaryAssetId();
	return FPrimaryAssetId(ItemType, GetFName());
}
