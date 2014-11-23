#pragma once
#include "../Private/BluPrivatePCH.h"
#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

class BLU_API BluManager {

	public:
		static void doBluMessageLoop();
		static CefSettings settings;
		static CefMainArgs main_args;

};