// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AudioDecompress.h"

// Workers, to have an Async Decompress worker
#include "Runtime/Core/Public/Async/AsyncWork.h"

#include "AudioDecompressWorker.h"

#include "OggAsset.h"

#include "SoundProcessingLibrary.generated.h"

UCLASS()
class OGGASSET_API USoundProcessingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// FUNCTIONS ///

public:
	/// Functions to load Data from the HardDrive

	// Function to load a sound file from the HD
	UFUNCTION(BlueprintCallable, Category = "Ogg")
	static USoundWave* LoadOggFile(const FString& InFilePath);
	
	static USoundWave* LoadData(const TArray<uint8>& RawFile);

	static void LoadSoundWave(USoundWave* CompressedSoundWaveRef, const TArray<uint8>& RawFile);

	// Function to fill in the RawFile sound data into the USoundWave object
	static bool FillSoundWaveInfo(class USoundWave* InSoundWave, TArray<uint8>* InRawFile);

	/// Function to decompress the compressed Data that comes with the .ogg file

	static void GetPCMDataFromFile(class USoundWave* InSoundWave);

	/// Helper Functions

	// Function to Start a new DecompressTask
	static void InitNewDecompressTask(USoundWave* InSoundWaveRef);
};
