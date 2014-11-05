#pragma once

#include "CoreUObject.h"
#include "Engine.h"
#include "Engine/Console.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "ModuleManager.h"
#include "../Public/ICEF.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCEF, Log, All);

// Classes
#include "../Classes/CEFInstance.h"
#include "../Classes/CEFHandler.h"
#include "../Classes/CEFViewPort.h"

// CEF Things
#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"


