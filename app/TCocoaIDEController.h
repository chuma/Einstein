//
//  TCocoaMonitorController.h
//  Einstein
//
//  Created by Steven Frank on 2/26/11.
//

#import <Cocoa/Cocoa.h>

//class TMacMonitor;
//@class TMacMonitorView;

@interface TCocoaIDEController : NSWindowController <NSTextFieldDelegate, NSWindowDelegate>
{
//	IBOutlet TMacMonitorView* view;
	IBOutlet NSToolbarItem* compileAndRunButton;
//	IBOutlet NSButton* stepIntoButton;
//	IBOutlet NSButton* stepOverButton;

//	TMacMonitor* monitor;
}

//- (void)update;

//- (void)addHistoryLine:(NSString *)line type:(int)type;
//
- (IBAction)compileAndRun:(id)sender;
//- (IBAction)stepInto:(id)sender;
//- (IBAction)stepOver:(id)sender;
//
//- (void)executeCommand:(NSString*)command;
//- (void)setMonitor:(TMacMonitor*)inMonitor;

@end
