//
//  TCocoaWorkshopController.mm
//  Einstein
//
//  Created by Matthias Melcher on 2/27/2017.
//

#import "TCocoaWorkshopController.h"
#import "TCocoaUserDefaults.h"
#import "TMacWorkshop.h"

#include "Workshop/TWSProjectItem.h"
#include "Workshop/Project/TWSProject.h"
#include "Workshop/NewtonScript/TWSNewtonScriptDocument.h"


@implementation TCocoaWorkshopController


- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	[[self window] setDelegate:nil];	// so windowWillClose doesn't get called
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}


- (void)awakeFromNib
{
	[[self window] setDelegate:self];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillTerminate:) name:NSApplicationWillTerminateNotification object:nil];
}


- (void)setWorkshop:(TMacWorkshop*)inWorkshop
{
	workshop = inWorkshop;
	//workshop->SetController(self);
}


- (void)showWindow:(id)sender
{
	[super showWindow:sender];

	[[NSUserDefaults standardUserDefaults] setBool:YES forKey:kOpenWorkshopAtLaunch];
	[[NSUserDefaults standardUserDefaults] synchronize];
}


- (void)newProject:(id)sender
{
	workshop->NewProject("HelloWorld");
}


- (void)compileAndRun:(id)sender
{
	printf("0x%016lx\n", (uintptr_t)workshop);
	workshop->CompileAndRun();
}


- (void)windowWillClose:(NSNotification *)notification
{
//	[[NSUserDefaults standardUserDefaults] setBool:NO forKey:kOpenWorkshopAtLaunch];
	[[NSUserDefaults standardUserDefaults] synchronize];
}


// Method returns count of children for given tree node item
- (NSInteger)outlineView:(NSOutlineView *)outlineView
  numberOfChildrenOfItem:(id)item
{
	if (!workshop) return 0;
	if (item) {
		TWSProjectItem *it = (TWSProjectItem*)[item pointerValue];
		return it->GetNumChildren();
	} else {
		if (workshop->GetProject()) {
			return 1;
		} else {
			return 0;
		}
	}
}


// Method returns flag, whether we can expand given tree node item or not
// (here is the simple rule, we can expand only nodes having one and more children)
- (BOOL)outlineView:(NSOutlineView *)outlineView
   isItemExpandable:(id)item
{
	if (!workshop) return 0;
	if (item) {
		TWSProjectItem *it = (TWSProjectItem*)[item pointerValue];
		return it->CanHaveChildren();
	} else {
		return YES;
	}
}


// Method returns value to be shown for given column of the tree node item
- (NSView*)outlineView:(NSOutlineView *)outlineView
	viewForTableColumn:(nullable NSTableColumn *)tableColumn
				  item:(nonnull id)item
{
	if (!workshop) return 0;
	if (!item) return 0;
	TWSProjectItem *it = (TWSProjectItem*)[item pointerValue];
	NSTableCellView *cell = [outlineView makeViewWithIdentifier:tableColumn.identifier owner:self];
	if(cell == nil) {
		NSTableCellView *cell = [[NSTableCellView alloc] initWithFrame:[outlineView frame]];
		cell.identifier = [tableColumn identifier];
	}
	[[cell imageView] setImage:[NSImage imageNamed:[NSString stringWithUTF8String:it->GetIcon()]]];
	[[cell textField] setStringValue:[NSString stringWithUTF8String:it->GetName()]];
	return cell;
}


// Method returns children item for given tree node item by given index
- (id)outlineView:(NSOutlineView *)outlineView
			child:(NSInteger)index
		   ofItem:(id)item
{
	TWSProjectItem *parent = 0;
	TWSProjectItem *child = 0;
	if (!workshop) return 0;
	if (item) {
		parent = (TWSProjectItem*)[item pointerValue];
		child = parent->GetChild(index);
	} else {
		child = workshop->GetProject();
	}
	if (child) {
		id childId;
		if (!child->outlineId) {
			childId = [NSValue valueWithPointer:child];
			child->outlineId = (__bridge_retained void*)childId;
		} else {
			childId = (__bridge id)child->outlineId;
		}
		return childId;
	} else {
		return 0L;
	}
}

/*
 Add a text view:
 
 NSTextView *text = [[NSTextView alloc] initWithFrame:CGRectMake(...)];
 text.backgroundColor = [UIColor greenColor];
 [text setDelegate:self];
 [self.view addSubView:text];

 -(BOOL)textView ... shouldChangeTextInRange...
*/
- (IBAction)selectProjectItem:(id)sender
{
	//NSTableView* tableView = (NSTableView*)sender;
	NSOutlineView *outline = (NSOutlineView*)sender;
	NSInteger row = [outline clickedRow];
	if (row==-1) return;
	id item = [sender itemAtRow:row];
	TWSProjectItem *it = (TWSProjectItem*)[item pointerValue];
	if (it) {
		if (it->pEditor) {
			[contentTab selectTabViewItemWithIdentifier:item];
		} else {
			// TODO: shall we create an editor for this class? Use a virtual function!
			TWSNewtonScriptDocument *ns = dynamic_cast<TWSNewtonScriptDocument*>(it);
			if (ns) [self CreateNewtonScriptEditor:ns];
			TWSProject *proj = dynamic_cast<TWSProject*>(it);
			if (proj) [self CreateProjectEditor:proj];
		}
	}
}


- (void)UpdateProjectOutline
{
	[projectOutlineView reloadItem:0L reloadChildren:YES];
}


- (void)CreateNewtonScriptEditor:(TWSNewtonScriptDocument*)inDocument
{
	void *parentView = (__bridge void*)contentTab;
	inDocument->CreateEditor(parentView);
}


- (void)CreateProjectEditor:(TWSProject*)inProject
{
	void *parentView = (__bridge void*)contentTab;
	inProject->CreateEditor(parentView);
}


void WorkshopControllerUpdateProjectOutline(TCocoaWorkshopController *self)
{
	return [(id) self UpdateProjectOutline];
}

@end
