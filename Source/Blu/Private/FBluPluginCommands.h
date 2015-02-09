#pragma once

#include "BluPrivatePCH.h"

#include "SlateBasics.h"
#include "Commands.h"
#include "EditorStyle.h"

class FBluPluginCommands : public TCommands<FBluPluginCommands>
{
public:

	FBluPluginCommands()
		: TCommands<FBluPluginCommands>(TEXT("BluDeugger"), NSLOCTEXT("Contexts", "BLUI", "BLUI Debugger"), NAME_None, FEditorStyle::GetStyleSetName())
	{

	}

	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > MyButton;

};