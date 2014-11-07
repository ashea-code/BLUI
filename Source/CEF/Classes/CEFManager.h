#pragma once
#include "../Private/CEFPrivatePCH.h"
#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

class CEF_API CEFManager {

	public:
		static void doCEFMessageLoop();
		static CefSettings settings;
		static CefWindowInfo info;
		static CefMainArgs main_args;

};