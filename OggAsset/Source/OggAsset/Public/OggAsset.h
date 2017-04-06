// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Sound/SoundWave.h"

#include "OggAsset.generated.h"

/**
 * Implements an asset that can be used to store an Ogg file
 */
UCLASS(BlueprintType, HideCategories=(Object))
class OGGASSET_API UOggAsset
	: public USoundWave
{
	GENERATED_BODY()

public:

	/** Holds the stored data. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category =Object)
	TArray<uint8> Data;

	virtual void PostLoad() override {
		USoundBase::PostLoad();

		if (Data.Num() == 0)
			return;

		// Get Pointer to the Compressed OGG Data
		FByteBulkData* BulkData = &this->CompressedFormatData.GetFormat(FName("OGG"));

		// Set the Lock of the BulkData to ReadWrite
		BulkData->Lock(LOCK_READ_WRITE);

		// Copy compressed RawFile Data to the Address of the OGG Data of the SW File
		FMemory::Memmove(BulkData->Realloc(Data.Num()), Data.GetData(), Data.Num());

		// Unlock the BulkData again
		BulkData->Unlock();
	}
};
