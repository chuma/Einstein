//
//  TWSProjectEditor.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#include "Workshop/Project/TWSMacProjectEditor.h"
#include "Workshop/Project/TWSProject.h"


@implementation TWSProjectEditor

+(id)LoadFromNIB:(TWSProject*)inProject
{
	NSArray* arrayOfViews = [[NSArray alloc] init];
	BOOL found = [[NSBundle mainBundle] loadNibNamed:@"TWSMacProjectView"
											   owner:nil
									 topLevelObjects:&arrayOfViews];
	if (!found)
		return 0L;

	id tab = 0;
	for (id i in arrayOfViews ){
		if ([i isKindOfClass:[TWSProjectEditor class]]) {
			tab = i;
			break;
		}
	}
	if (!tab)
		return 0L;

	TWSProjectEditor* ed = (TWSProjectEditor*)tab;
	ed->project = inProject;
	return tab;
}


-(void)AddTo:(NSTabView*)tabView
{
	tabViewItem = [[NSTabViewItem alloc] initWithIdentifier:(__bridge id)project->outlineId];
	[tabViewItem setView:viewFromNIB];
	[tabView addTabViewItem:tabViewItem];
	[tabView selectTabViewItem:tabViewItem];
	[self UpdateUIFromProjectData];
}


/**
 */
-(void)UpdateUIFromProjectData;
{
	[platformPopUp selectItemWithTag:project->GetPlatform()];
	[languageTextField setStringValue:[NSString stringWithUTF8String:project->GetLanguage()]];
	[debugBuildButton setState:project->IsDebugBuild()];
	[ignoreNativeButton setState:project->IgnoreNative()];
	[checkGlobalFunctionsButton setState:project->CheckGlobalFunctions()];
	[oldBuildRulesButton setState:project->UseOldBuildRules()];
	[useStepChildrenButton setState:project->UseStepChildren()];
	[suppressByteCodesButton setState:project->SuppressByteCodes()];
	[fasterFunctionsButton setState:project->UseFasterFunctions()];

	[applicationNameTextField setStringValue:[NSString stringWithUTF8String:project->GetApplicationName()]];
	[applicationSymbolTextField setStringValue:[NSString stringWithUTF8String:project->GetApplicationSymbol()]];
	//IBOutlet NSTextField* partTypeTextField;
	[topFrameExpressionTextField setStringValue:[NSString stringWithUTF8String:project->GetTopFrameExpression()]];
	[autoCloseButton setState:project->IsAutoClose()];
	//IBOutlet NSTextField* customPartTypeTextField;
	[fasterSoupsButton setState:project->UseFasterSoups()];

	[packageNameTextField setStringValue:[NSString stringWithUTF8String:project->GetPackageName()]];
	//IBOutlet NSTextField* versionTextField;
	[copyrightTextField setStringValue:[NSString stringWithUTF8String:project->GetCopyright()]];
	//IBOutlet NSButton* optimizeSpeedButton;
	[copyProtectButton setState:project->CopyProtect()];
	[deleteOnDownloadButton setState:project->DeleteOnDownload()];
	//IBOutlet NSButton* dispatchOnlyButton;
	[fourByteAlignmentButton setState:!project->UseFourByteAlignment()];
	[zippyCompressionButton setState:project->UseZippyCompression()];
	[newton20OnlyButton setState:project->IsNewton20Only()];

	// -- profiler settings
	//IBOutlet NSButton* profileMemoryButton;
	//IBOutlet NSButton* profilePercentButton;
	[compileForProfilingButton setState:project->CompileForProfiling()];
	//IBOutlet NSButton* compileForSpeedButton;
	//IBOutlet NSButton* detailedSystemCallsButton;
	//IBOutlet NSButton* detailedUserFunctionsButton;
}


/**
 */
-(void)UpdateProjectDataFromUI;
{
	project->SetPlatform(platformPopUp.selectedTag);
	project->SetLanguage(languageTextField.stringValue.UTF8String);
	project->SetDebugBuild(debugBuildButton.state==NSOnState);
	project->IgnoreNative(ignoreNativeButton.state==NSOnState);
	project->CheckGlobalFunctions(checkGlobalFunctionsButton.state==NSOnState);
	project->UseOldBuildRules(oldBuildRulesButton.state==NSOnState);
	project->UseStepChildren(useStepChildrenButton.state==NSOnState);
	project->SuppressByteCodes(suppressByteCodesButton.state==NSOnState);
	project->SetFasterFunctions(fasterFunctionsButton.state==NSOnState);

	project->SetApplicationName(applicationNameTextField.stringValue.UTF8String);
	project->SetApplicationSymbol(applicationSymbolTextField.stringValue.UTF8String);
	//IBOutlet NSTextField* partTypeTextField;
	project->SetTopFrameExpression(topFrameExpressionTextField.stringValue.UTF8String);
	project->SetAutoClose(autoCloseButton.state==NSOnState);
	//IBOutlet NSTextField* customPartTypeTextField;
	project->SetFasterSoups(fasterSoupsButton.state==NSOnState);

	project->SetPackageName(packageNameTextField.stringValue.UTF8String);
	//IBOutlet NSTextField* versionTextField;
	project->SetCopyright(copyrightTextField.stringValue.UTF8String);
	//IBOutlet NSButton* optimizeSpeedButton;
	project->SetCopyProtect(copyProtectButton.state==NSOnState);
	project->SetDeleteOnDownload(deleteOnDownloadButton.state==NSOnState);
	//IBOutlet NSButton* dispatchOnlyButton;
	project->SetFourByteAlignment(fourByteAlignmentButton.state==NSOffState);
	project->SetZippyCompression(zippyCompressionButton.state==NSOnState);
	project->SetNewton20Only(newton20OnlyButton.state==NSOnState);

	// -- profiler settings
	//IBOutlet NSButton* profileMemoryButton;
	//IBOutlet NSButton* profilePercentButton;
	project->SetCompileForProfiling(compileForProfilingButton.state==NSOnState);
	//IBOutlet NSButton* compileForSpeedButton;
	//IBOutlet NSButton* detailedSystemCallsButton;
	//IBOutlet NSButton* detailedUserFunctionsButton;
}

@end


void *ProjectEditorCreate(TWSProject *inProject, void* inParentView)
{
	// create the editor using a prototype from the associated nib file
	NSTabView* contentTab = (__bridge NSTabView*)inParentView;
	id tab = [TWSProjectEditor LoadFromNIB:inProject];
	[tab AddTo:contentTab];
	[tab UpdateUIFromProjectData];
	return (__bridge_retained void*)tab;
}


void ProjectEditorDestroy(void *inEditor)
{
	TWSProjectEditor* tab = (__bridge_transfer TWSProjectEditor*)inEditor;
	tab = 0;
}


void ProjectEditorUpdateUI(void *inEditor)
{
	TWSProjectEditor* tab = (__bridge_transfer TWSProjectEditor*)inEditor;
	[tab UpdateUIFromProjectData];
}


void ProjectEditorUpdateData(void *inEditor)
{
	TWSProjectEditor* tab = (__bridge_transfer TWSProjectEditor*)inEditor;
	[tab UpdateProjectDataFromUI];
}


//
//// Set and get the text in the editor
//void ProjectEditorSetText(void *inEditor, const char *text);
//const char* ProjectEditorGetText(void *inEditor);
//
//// Receive messages from the UI
//void ProjectEditorChangedCallback(TWSProjectEditorCallback* inCallback);
