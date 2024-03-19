// All Rights Reserved - 2024 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMulti_3D_WidgetsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
