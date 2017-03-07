//
//  TWSNewtonScriptEditor.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#include "Workshop/NewtonScript/TWSMacNewtonScriptEditor.h"
#include "Workshop/NewtonScript/TWSNewtonScriptDocument.h"


@implementation TWSNewtonScriptEditor

//- (void)addHistoryLine:(NSString *)line type:(int)type
//{
//	[view addHistoryLine:line type:type];
//}


//- (void)applicationWillTerminate:(NSNotification *)aNotification
//{
//	[[self window] setDelegate:nil];	// so windowWillClose doesn't get called
//	[[NSNotificationCenter defaultCenter] removeObserver:self];
//}
//
//
//- (void)awakeFromNib
//{
//	[[self window] setDelegate:self];
//
//	//	[view setController:self];
//	//
//	//	[self update];
//
//	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillTerminate:) name:NSApplicationWillTerminateNotification object:nil];
//}


//- (void)executeCommand:(NSString*)command
//{
//	[self addHistoryLine:[NSString stringWithFormat:@"> %@", command] type:MONITOR_LOG_USER_INPUT];
//
//	monitor->ExecuteCommand([command UTF8String]);
//	[self performSelector:@selector(update) withObject:nil afterDelay:0.0];
//
//	if ( [command isEqualToString:@"run"] )
//	{
//		[stopStartButton setTitle:@"Stop"];
//	}
//	else if ( [command isEqualToString:@"stop"] )
//	{
//		[stopStartButton setTitle:@"Run"];
//	}
//}


//- (void)showWindow:(id)sender
//{
//	[super showWindow:sender];
//
//	[[NSUserDefaults standardUserDefaults] setBool:YES forKey:kOpenWorkshopAtLaunch];
//	[[NSUserDefaults standardUserDefaults] synchronize];
//}


+(id)LoadFromNIB:(TWSNewtonScriptDocument*)inDocument
{
	NSArray* arrayOfViews = [[NSArray alloc] init];
	BOOL found = [[NSBundle mainBundle] loadNibNamed:@"TWSMacNewtonScriptView"
											   owner:nil
									 topLevelObjects:&arrayOfViews];
	if (!found)
		return 0L;

	id tab = 0;
	for (id i in arrayOfViews ){
		if ([i isKindOfClass:[TWSNewtonScriptEditor class]]) {
			tab = i;
			break;
		}
	}
	if (!tab)
		return 0L;

	TWSNewtonScriptEditor* ed = (TWSNewtonScriptEditor*)tab;
	ed->documentBackRef = inDocument;
	return tab;
}


-(void)AddTo:(NSTabView*)tabView
{
	tabViewItem = [[NSTabViewItem alloc] initWithIdentifier:@"Jo"];
	[tabViewItem setView:viewFromNIB];
	[tabView addTabViewItem:tabViewItem];
	// TODO: show the tab view
}

@end


void *NewtonScriptEditorCreate(TWSNewtonScriptDocument *inDocument, void* inParentView)
{
	NSTabView* contentTab = (__bridge NSTabView*)inParentView;
	id tab = [TWSNewtonScriptEditor LoadFromNIB:inDocument];
	[tab AddTo:contentTab];
#if 0
	// set the text
	NSString* text = [NSString stringWithUTF8String:ns->GetName()];
	NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:text];
	[attrString addAttribute:NSFontAttributeName value:[NSFont fontWithName:@"Menlo" size:11.0f] range:NSMakeRange(0, text.length)];
	[[theTextView textStorage] appendAttributedString:attrString];
	[tab setView:scrollview];
	ns->editorId = (__bridge_retained void*)theTextView;
#endif
	return (__bridge_retained void*)tab;
}


void NewtonScriptEditorDestroy(void *inEditor)
{
	NSTabViewItem* tab = (__bridge_transfer NSTabViewItem*)inEditor;
	tab = 0;
}


//
//// Set and get the text in the editor
//void NewtonScriptEditorSetText(void *inEditor, const char *text);
//const char* NewtonScriptEditorGetText(void *inEditor);
//
//// Receive messages from the UI
//void NewtonScriptEditorChangedCallback(TWSNewtonScriptEditorCallback* inCallback);
