#pragma once
#include "../Classes/RenderHandler.h"
#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

class BLU_API BluInstance
{
	private:
		CefBrowserSettings browserSettings;
		CefSettings settings;
		CefWindowInfo info;
	
	public:
		BluInstance();
		~BluInstance();

};