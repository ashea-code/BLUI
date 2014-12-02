#pragma once
#include "../Private/BluPrivatePCH.h"

class BLU_API BluManager {

	public:
		static void doBluMessageLoop();
		static CefSettings settings;
		static CefMainArgs main_args;

};