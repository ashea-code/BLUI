#include "CEFPrivatePCH.h"

bool running = false;

class FCEF : public ICEF
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		UE_LOG(LogCEF, Warning, TEXT("STATUS: Loaded"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogCEF, Warning, TEXT("STATUS: Shutdown"));
		CefShutdown();
	}

};

IMPLEMENT_MODULE( FCEF, CEF )
DEFINE_LOG_CATEGORY(LogCEF);