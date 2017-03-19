//
//  TWSLayoutEditor.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#include "Workshop/Layout/TWSMacLayoutEditor.h"
#include "Workshop/Layout/TWSLayoutDocument.h"


@implementation TWSLayoutEditor

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


+(id)LoadFromNIB:(TWSLayoutDocument*)inDocument
{
	NSArray* arrayOfViews = [[NSArray alloc] init];
	BOOL found = [[NSBundle mainBundle] loadNibNamed:@"TWSMacLayoutView"
											   owner:nil
									 topLevelObjects:&arrayOfViews];
	if (!found)
		return 0L;

	id tab = 0;
	for (id i in arrayOfViews ){
		if ([i isKindOfClass:[TWSLayoutEditor class]]) {
			tab = i;
			break;
		}
	}
	if (!tab)
		return 0L;

	TWSLayoutEditor* ed = (TWSLayoutEditor*)tab;
	ed->documentBackRef = inDocument;
	return tab;
}


-(void)AddTo:(NSTabView*)tabView
{
	tabViewItem = [[NSTabViewItem alloc] initWithIdentifier:(__bridge id)documentBackRef->outlineId];
	[tabViewItem setView:viewFromNIB];
	[tabView addTabViewItem:tabViewItem];
	[tabView selectTabViewItem:tabViewItem];

	[[textView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
	[[textView textContainer] setWidthTracksTextView:NO];
	[textView setHorizontallyResizable:YES];
}


/**
 * Load the document from the associated document manager and show it in the editor
 */
-(void)LoadDocument
{
	char *script = documentBackRef->GetText();
	if (!script) script = strdup("");
	NSString* text = [NSString stringWithUTF8String:script];
	NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:text];
	[attrString addAttribute:NSFontAttributeName value:[NSFont fontWithName:@"Menlo" size:11.0f] range:NSMakeRange(0, text.length)];
	[[textView textStorage] appendAttributedString:attrString];
}

@end


void *LayoutEditorCreate(TWSLayoutDocument *inDocument, void* inParentView)
{
	// create the editor using a prototype from the associated nib file
	NSTabView* contentTab = (__bridge NSTabView*)inParentView;
	id tab = [TWSLayoutEditor LoadFromNIB:inDocument];
	[tab AddTo:contentTab];
	[tab LoadDocument];
	return (__bridge_retained void*)tab;
}


void LayoutEditorDestroy(void *inEditor)
{
	NSTabViewItem* tab = (__bridge_transfer NSTabViewItem*)inEditor;
	tab = 0;
}


//
//// Set and get the text in the editor
//void LayoutEditorSetText(void *inEditor, const char *text);
//const char* LayoutEditorGetText(void *inEditor);
//
//// Receive messages from the UI
//void LayoutEditorChangedCallback(TWSLayoutEditorCallback* inCallback);
