#pragma once

#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
#endif
#undef OVERRIDE // cef headers provide their own OVERRIDE macro
THIRD_PARTY_INCLUDES_START
#include "include/cef_app.h"
THIRD_PARTY_INCLUDES_END
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"
#endif

class BLU_API BluManager : public CefApp
{

	public:

		BluManager();

		static void doBluMessageLoop();
		static CefSettings settings;
		static CefMainArgs main_args;
		static bool CPURenderSettings;

	public:
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
			CefRefPtr< CefCommandLine > command_line) override;

	IMPLEMENT_REFCOUNTING(BluManager)

};