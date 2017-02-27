//
//  TCocoaWorkshopController.h
//  Einstein
//
//  Created by Matthias Melcher on 2/27/2017.
//

#import <Cocoa/Cocoa.h>

class TMacWorkshop;
//@class TMacMonitorView;

@interface TCocoaWorkshopController : NSWindowController <NSTextFieldDelegate, NSWindowDelegate>
{
//	IBOutlet TMacMonitorView* view;
	IBOutlet NSToolbarItem* compileAndRunToolbarItem;
//	IBOutlet NSButton* stepIntoButton;
//	IBOutlet NSButton* stepOverButton;

	TMacWorkshop* workshop;
}

//- (void)update;

//- (void)addHistoryLine:(NSString *)line type:(int)type;
//
- (IBAction)compileAndRun:(id)sender;
//- (IBAction)stepInto:(id)sender;
//- (IBAction)stepOver:(id)sender;
//
//- (void)executeCommand:(NSString*)command;
- (void)setWorkshop:(TMacWorkshop*)inWorkshop;

@end
