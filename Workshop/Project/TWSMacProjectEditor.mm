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
	// TODO: initialize empty project editor
}


/**
 * Load the document from the associated document manager and show it in the editor
 */
-(void)LoadProject
{
	// TODO: initialize project editor
}

@end


void *ProjectEditorCreate(TWSProject *inProject, void* inParentView)
{
	// create the editor using a prototype from the associated nib file
	NSTabView* contentTab = (__bridge NSTabView*)inParentView;
	id tab = [TWSProjectEditor LoadFromNIB:inProject];
	[tab AddTo:contentTab];
	[tab LoadProject];
	return (__bridge_retained void*)tab;
}


void ProjectEditorDestroy(void *inEditor)
{
	NSTabViewItem* tab = (__bridge_transfer NSTabViewItem*)inEditor;
	tab = 0;
}


//
//// Set and get the text in the editor
//void ProjectEditorSetText(void *inEditor, const char *text);
//const char* ProjectEditorGetText(void *inEditor);
//
//// Receive messages from the UI
//void ProjectEditorChangedCallback(TWSProjectEditorCallback* inCallback);
