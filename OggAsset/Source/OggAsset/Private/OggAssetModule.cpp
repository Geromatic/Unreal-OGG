// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "OggAssetPrivatePCH.h"
#include "ModuleInterface.h"


/**
 * Implements the OggAsset module.
 */
class FOggAssetModule
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


IMPLEMENT_MODULE(FOggAssetModule, OggAsset);
