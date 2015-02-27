#pragma once

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#endif
#include "include/cef_client.h"
#include "include/cef_app.h"
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformTypes.h"
#endif

class BLU_API BluManager 
{

	public:
		static void doBluMessageLoop();
		static CefSettings settings;
		static CefMainArgs main_args;

};