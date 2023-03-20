#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class ILoadingScreenModule : public IModuleInterface
{
public:
    static inline ILoadingScreenModule& Get()
    {
        return FModuleManager::LoadModuleChecked<ILoadingScreenModule>("LoadingScreen");
    }
    
    virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) = 0;

    virtual void StopInGameLoadingScreen() = 0;
};
