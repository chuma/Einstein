//
//  TCocoaWorkshopController.h
//  Einstein
//
//  Created by Matthias Melcher on 2/27/2017.
//

#import <Cocoa/Cocoa.h>

class TMacWorkshop;
class TWSProject;
class TWSNewtonScriptDocument;

@interface TCocoaWorkshopController : NSWindowController <NSTextFieldDelegate, NSWindowDelegate>
{
	IBOutlet NSToolbarItem* compileAndRunToolbarItem;
	IBOutlet NSOutlineView* projectOutlineView;
	IBOutlet NSTabView* contentTab;
	TMacWorkshop* workshop;
}

- (void)UpdateProjectOutline;
- (IBAction)newProject:(id)sender;
- (IBAction)compileAndRun:(id)sender;
- (IBAction)selectProjectItem:(id)sender;
- (void)setWorkshop:(TMacWorkshop*)inWorkshop;
- (void)CreateNewtonScriptEditor:(TWSNewtonScriptDocument*)inDocument;
- (void)CreateProjectEditor:(TWSProject*)inProject;

@end

void WorkshopControllerUpdateProjectOutline(TCocoaWorkshopController *self);

