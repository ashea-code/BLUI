#include "BluPrivatePCH.h"

#include "SlateBasics.h"
#include "SlotBase.h"
#include "SPanel.h"
#include "TabManager.h"
#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"


static const FName BluTabName("BLUITab");

class FBlu : public IBlu
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{

		FString GameDir = FPaths::ConvertRelativePathToFull(FPaths::GameDir() + "/BluCache");
		CefString GameDirCef = *GameDir;

		// Setup the default settings for BluManager
		BluManager::settings.windowless_rendering_enabled = true;

		BluManager::settings.remote_debugging_port = 7777;

		CefString(&BluManager::settings.browser_subprocess_path).FromASCII("blu_ue4_process.exe");
		CefString(&BluManager::settings.cache_path).FromString(GameDirCef);

		CefExecuteProcess(BluManager::main_args, NULL, NULL);
		CefInitialize(BluManager::main_args, BluManager::settings, NULL, NULL);

		UBluEye::StaticClass();

		// Time to register the BLUI debugger window in editor
		#if WITH_EDITOR
		TSharedRef<class FGlobalTabmanager> tm = FGlobalTabmanager::Get();
		FBluPluginCommands::Register();

		PluginCommands = MakeShareable(new FUICommandList);
		PluginCommands->MapAction(
			FBluPluginCommands::Get().MyButton,
			FExecuteAction::CreateRaw(this, &FBlu::DebuggerClicked),
			FCanExecuteAction());

		ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtension = ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FBlu::AddToolbarExtension));
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

		ExtensionManager = LevelEditorModule.GetToolBarExtensibilityManager();

		tm->RegisterTabSpawner(BluTabName, FOnSpawnTab::CreateRaw(this, &FBlu::SpawnTab))
			.SetDisplayName(FText::FromString(TEXT("BLUI Debugger")));

		// End registering debugger window
		#endif

		UE_LOG(LogBlu, Log, TEXT("STATUS: Loaded"));
	}

	void DebuggerClicked()
	{
		TSharedRef<class FGlobalTabmanager> tm = FGlobalTabmanager::Get();
		tm->InvokeTab(BluTabName);
	}

	void AddToolbarExtension(FToolBarBuilder &builder)
	{
		#define LOCTEXT_NAMESPACE "LevelEditorToolBar"

			UE_LOG(LogBlu, Log, TEXT("Starting Extension logic"));

			FSlateIcon IconBrush = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.ViewOptions", "LevelEditor.ViewOptions.Small");

			builder.AddToolBarButton(FBluPluginCommands::Get().MyButton, NAME_None, LOCTEXT("MyButton_Override", "BLUI Debugger"), LOCTEXT("MyButton_ToolTipOverride", "Click to open BLUI debugger window to inspect running web views"), IconBrush, NAME_None);

		#undef LOCTEXT_NAMESPACE
	}

	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& TabSpawnArgs)
	{
		TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("BLUI will have an integrated WebView debugger in Unreal Engine 4.7, when the SWebBrowser class is exposed!")))
		];

		return SpawnedTab;
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogBlu, Log, TEXT(" STATUS: Shutdown"));
		return CefShutdown();
	}

};




IMPLEMENT_MODULE( FBlu, Blu )
DEFINE_LOG_CATEGORY(LogBlu);