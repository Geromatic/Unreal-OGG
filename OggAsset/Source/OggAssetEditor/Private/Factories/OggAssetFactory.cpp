// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "OggAssetEditorPrivatePCH.h"
#include "OggAssetFactory.h"

#include "../OggAsset/Public/SoundProcessingLibrary.h"


/* UOggAssetFactory structors
 *****************************************************************************/

UOggAssetFactory::UOggAssetFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	Formats.Add(FString(TEXT("ogg;")) + NSLOCTEXT("UOggAssetFactory", "Sound", "OGG File").ToString());
	SupportedClass = USoundWave::StaticClass();
	bCreateNew = false;
	bEditorImport = true;

}


/* UFactory overrides
 *****************************************************************************/

UObject* UOggAssetFactory::FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	UOggAsset* OggAsset = nullptr;
	TArray<uint8> data;

	if (FFileHelper::LoadFileToArray(data, *CurrentFilename))
	{
		OggAsset = NewObject<UOggAsset>(InParent, Name, Flags);
		USoundProcessingLibrary::LoadSoundWave(OggAsset, data);
		OggAsset->Data = data;
	}

	return OggAsset;
}
