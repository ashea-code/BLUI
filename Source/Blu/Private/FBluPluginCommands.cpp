#include "BluPrivatePCH.h"

#include "FBluPluginCommands.h"

PRAGMA_DISABLE_OPTIMIZATION
void FBluPluginCommands::RegisterCommands()
{
	UI_COMMAND(MyButton, "BLUI Debugger", "Debug current running web views", EUserInterfaceActionType::Button, FInputGesture());
}
PRAGMA_ENABLE_OPTIMIZATION
