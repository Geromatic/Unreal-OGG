// Fill out your copyright notice in the Description page of Project Settings.

#include "OggAssetPrivatePCH.h"
#include "SoundProcessingLibrary.h"

#include "OggAsset.h"

#include "Sound/SoundWave.h"
#include "AudioDevice.h"
#include "Runtime/Engine/Public/VorbisAudioInfo.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"

/// Functions to load Data from the HardDrive

USoundWave* USoundProcessingLibrary::LoadOggFile(const FString& InFilePath)
{
		// TArray that holds the binary and encoded Sound data
	TArray<uint8> RawFile;

	// Load file into RawFile Array
	bool bLoaded = FFileHelper::LoadFileToArray(RawFile, InFilePath.GetCharArray().GetData());

	if (!bLoaded)
		return nullptr;

	return USoundProcessingLibrary::LoadData(RawFile);
}

USoundWave* USoundProcessingLibrary::LoadData(const TArray<uint8>& RawFile)
{
	// Create new SoundWave Object
	USoundWave* CompressedSoundWaveRef = NewObject<USoundWave>(USoundWave::StaticClass());

	// Make sure the SoundWave Object is Valid
	if (!CompressedSoundWaveRef) {

		PrintError(TEXT("Failed to create new SoundWave Object!"));
		return nullptr;
	}

	// Fill the SoundData into the SoundWave Object
	if (RawFile.Num() > 0) {

		if (!FillSoundWaveInfo(CompressedSoundWaveRef, (TArray<uint8>*)&RawFile)) {

			PrintError(TEXT("Something went wrong while loading the Sound Data!"));
			return nullptr;
		}
	}
	else {

		PrintError(TEXT("RawFile Array is empty! Seams like Sound couldn't be loaded correctly."));
		return nullptr;
	}
		

	// Get Pointer to the Compressed OGG Data
	FByteBulkData* BulkData = &CompressedSoundWaveRef->CompressedFormatData.GetFormat(FName("OGG"));

	// Set the Lock of the BulkData to ReadWrite
	BulkData->Lock(LOCK_READ_WRITE);

	// Copy compressed RawFile Data to the Address of the OGG Data of the SW File
	FMemory::Memmove(BulkData->Realloc(RawFile.Num()), RawFile.GetData(), RawFile.Num());

	// Unlock the BulkData again
	BulkData->Unlock();

	return CompressedSoundWaveRef;
}

void USoundProcessingLibrary::LoadSoundWave(USoundWave* CompressedSoundWaveRef, const TArray<uint8>& RawFile)
{
	// Make sure the SoundWave Object is Valid
	if (!CompressedSoundWaveRef) {

		PrintError(TEXT("Failed to create new SoundWave Object!"));
		return;
	}

	// Fill the SoundData into the SoundWave Object
	if (RawFile.Num() > 0) {

		if (!FillSoundWaveInfo(CompressedSoundWaveRef, (TArray<uint8>*)&RawFile)) {

			PrintError(TEXT("Something went wrong while loading the Sound Data!"));
			return;
		}
	}
	else {

		PrintError(TEXT("RawFile Array is empty! Seams like Sound couldn't be loaded correctly."));
		return;
	}

	// Get Pointer to the Compressed OGG Data
	FByteBulkData* BulkData = &CompressedSoundWaveRef->CompressedFormatData.GetFormat(FName("OGG"));

	// Set the Lock of the BulkData to ReadWrite
	BulkData->Lock(LOCK_READ_WRITE);

	// Copy compressed RawFile Data to the Address of the OGG Data of the SW File
	FMemory::Memmove(BulkData->Realloc(RawFile.Num()), RawFile.GetData(), RawFile.Num());

	// Unlock the BulkData again
	BulkData->Unlock();
}


bool USoundProcessingLibrary::FillSoundWaveInfo(USoundWave* InSoundWave, TArray<uint8>* InRawFile)
{
	// Info Structs
	FSoundQualityInfo SoundQualityInfo;
	FVorbisAudioInfo VorbisAudioInfo;

	// Save the Info into SoundQualityInfo
	if (!VorbisAudioInfo.ReadCompressedInfo(InRawFile->GetData(), InRawFile->Num(), &SoundQualityInfo))
	{
		return false;
	}

	// Fill in all the Data we have
	InSoundWave->DecompressionType = EDecompressionType::DTYPE_RealTime;
	InSoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	InSoundWave->NumChannels = SoundQualityInfo.NumChannels;
	InSoundWave->Duration = SoundQualityInfo.Duration;
	InSoundWave->RawPCMDataSize = SoundQualityInfo.SampleDataSize;
	InSoundWave->SampleRate = SoundQualityInfo.SampleRate;

	return true;
}

/// Function to decompress the compressed Data that comes with the .ogg file

void USoundProcessingLibrary::GetPCMDataFromFile(USoundWave* InSoundWave)
{
	if (InSoundWave == nullptr)	{

		PrintError(TEXT("Passed SoundWave pointer is a nullptr!"));
		return;
	}

	if (InSoundWave->NumChannels < 1 || InSoundWave->NumChannels > 2) {

		PrintError(TEXT("SoundWave Object has not the right amount of Channels. Plugin only supports 1 or 2!"));
		return;
	}

	if (GEngine)
	{
		// Get a Pointer to the Main Audio Device
		FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();

		if (AudioDevice) {

			InSoundWave->InitAudioResource(AudioDevice->GetRuntimeFormat(InSoundWave));

			PrintLog(TEXT("Creating new DecompressWorker."));

			// Creates a new DecompressWorker and starts it
			InitNewDecompressTask(InSoundWave);
		}
		else {

			PrintError(TEXT("Couldn't get a valid Pointer to the Main AudioDevice!"));
			return;
		}
	}
}

void USoundProcessingLibrary::InitNewDecompressTask(USoundWave* InSoundWaveRef)
{
	// Do we already have a valid Runnable? If not, create a new one
	if (FAudioDecompressWorker::Runnable == NULL)
	{
		// Init new Worker and pass the SoundWaveRef to decompress it
		FAudioDecompressWorker::InitializeWorker(InSoundWaveRef);
	}
	else if(FAudioDecompressWorker::Runnable->IsFinished())
	{
		// The Worker is finished and still valid, shut it down!
		FAudioDecompressWorker::ShutdownWorker();

		// Init new Worker and pass the SoundWaveRef to decompress it
		FAudioDecompressWorker::InitializeWorker(InSoundWaveRef);
	}
	else {
		PrintLog(TEXT("Worker not finished!"));
	}
}
