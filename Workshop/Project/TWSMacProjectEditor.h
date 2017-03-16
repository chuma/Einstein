//
//  TWSProjectEditor.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSProjectEditor_h
#define TWSProjectEditor_h

class TWSProject;

#import <Cocoa/Cocoa.h>

@interface TWSProjectEditor : NSObject //<NSTextFieldDelegate>
{
	IBOutlet NSTabViewItem* tabViewItem;
	IBOutlet NSView* viewFromNIB;
	
	IBOutlet NSTextField* languageTextField;
	IBOutlet NSPopUpButton* platformPopUp;
	IBOutlet NSButton* debugBuildButton;
	IBOutlet NSButton* ignoreNativeButton;
	IBOutlet NSButton* checkGlobalFunctionsButton;
	IBOutlet NSButton* oldBuildRulesButton;
	IBOutlet NSButton* useStepChildrenButton;
	IBOutlet NSButton* suppressByteCodesButton;
	IBOutlet NSButton* fasterFunctionsButton;

	IBOutlet NSTextField* applicationNameTextField;
	IBOutlet NSTextField* applicationSymbolTextField;
	//IBOutlet NSTextField* partTypeTextField;
	IBOutlet NSTextField* topFrameExpressionTextField;
	IBOutlet NSButton* autoCloseButton;
	//IBOutlet NSTextField* customPartTypeTextField;
	IBOutlet NSButton* fasterSoupsButton;
	// TODO: icon

	// -- package settings
	IBOutlet NSTextField* packageNameTextField;
	//IBOutlet NSTextField* versionTextField;
	IBOutlet NSTextField* copyrightTextField;
	//IBOutlet NSButton* optimizeSpeedButton;
	IBOutlet NSButton* copyProtectButton;
	IBOutlet NSButton* deleteOnDownloadButton;
	//IBOutlet NSButton* dispatchOnlyButton;
	IBOutlet NSButton* fourByteAlignmentButton;
	IBOutlet NSButton* zippyCompressionButton;
	IBOutlet NSButton* newton20OnlyButton;

	// -- profiler settings
	//IBOutlet NSButton* profileMemoryButton;
	//IBOutlet NSButton* profilePercentButton;
	IBOutlet NSButton* compileForProfilingButton;
	//IBOutlet NSButton* compileForSpeedButton;
	//IBOutlet NSButton* detailedSystemCallsButton;
	//IBOutlet NSButton* detailedUserFunctionsButton;

	TWSProject* project;
}

+(id)LoadFromNIB:(TWSProject*)inProject;
-(void)AddTo:(NSTabView*)tabView;
-(void)UpdateUIFromProjectData;
-(void)UpdateProjectDataFromUI;

@end


#endif /* TWSProjectEditor_h */
