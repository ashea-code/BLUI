#pragma once
#include "CoreUObject.h"
#include "Engine.h"
#include "Engine/Console.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "ModuleManager.h"
#include "../Public/IBlu.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBlu, Log, All);

// CEF
#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

// Blu Classes
#include "../Classes/BluInstance.h"
#include "../Classes/BluManager.h"
#include "../Classes/BluUIComponent.h"
#include "../Classes/RenderHandler.h"
#include "../Classes/BluViewPort.h"
#include "../Classes/BluHUD.h"
#include "../Classes/BluWidget.h"