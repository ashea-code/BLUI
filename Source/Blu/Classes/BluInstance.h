#pragma once
#include "../Private/BluPrivatePCH.h"

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