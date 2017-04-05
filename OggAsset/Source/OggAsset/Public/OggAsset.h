// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OggAsset.generated.h"

/**
 * Implements an asset that can be used to store an Ogg file
 */
UCLASS(BlueprintType, HideCategories=(Object))
class OGGASSET_API UOggAsset
	: public UObject
{
	GENERATED_BODY()

public:

	/** Holds the stored data. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category =Object)
	TArray<uint8> Data;
};
