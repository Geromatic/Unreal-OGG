// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "OggAssetEditorPrivatePCH.h"
#include "ModuleInterface.h"


#define LOCTEXT_NAMESPACE "FOggAssetEditorModule"


/**
 * Implements the TextAssetEditor module.
 */
class FOggAssetEditorModule
	: public IModuleInterface
{
public:

	// IModuleInterface interface

	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};


IMPLEMENT_MODULE(FOggAssetEditorModule, OggAssetEditor);


#undef LOCTEXT_NAMESPACE
