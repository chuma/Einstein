//
//  TCocoaWorkshopController.h
//  Einstein
//
//  Created by Matthias Melcher on 2/27/2017.
//

#import <Cocoa/Cocoa.h>

class TMacWorkshop;
class TWSNewtonScript;

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
- (void)CreateNewtonScriptEditor:(TWSNewtonScript*)forNewtonScriptItem;

@end

void WorkshopControllerUpdateProjectOutline(TCocoaWorkshopController *self);
void WorkshopControllerCreateNewtonScriptEditor(TCocoaWorkshopController *self, TWSNewtonScript *inNewtonScriptItem);

