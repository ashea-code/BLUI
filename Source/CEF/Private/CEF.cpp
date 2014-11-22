#include "CEFPrivatePCH.h"

class FCEF : public ICEF
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		// Setup the default settings from CEFManager
		CEFManager::settings.windowless_rendering_enabled = true;
		CEFManager::settings.log_severity = LOGSEVERITY_VERBOSE;
		CefString(&CEFManager::settings.log_file).FromASCII("./CEF.log");
		CefString(&CEFManager::settings.browser_subprocess_path).FromASCII("cef_ue4_process.exe");

		CefExecuteProcess(CEFManager::main_args, NULL, NULL);
		CefInitialize(CEFManager::main_args, CEFManager::settings, NULL, NULL);

		UCEFUIComponent::StaticClass();

		UE_LOG(LogCEF, Log, TEXT(" STATUS: Loaded"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogCEF, Log, TEXT(" STATUS: Shutdown"));
		CefShutdown();
	}

};




IMPLEMENT_MODULE( FCEF, CEF )
DEFINE_LOG_CATEGORY(LogCEF);