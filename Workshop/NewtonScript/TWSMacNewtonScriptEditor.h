//
//  TWSNewtonScriptEditor.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSNewtonScriptEditor_h
#define TWSNewtonScriptEditor_h

class TWSNewtonScriptDocument;

#import <Cocoa/Cocoa.h>

@interface TWSNewtonScriptEditor : NSObject //<NSTextFieldDelegate>
{
	IBOutlet NSTabViewItem* tabViewItem;
	IBOutlet NSView* viewFromNIB;
	IBOutlet NSTextView* textView;

	TWSNewtonScriptDocument* documentBackRef;
}

+(id)LoadFromNIB:(TWSNewtonScriptDocument*)inDocument;
-(void)AddTo:(NSTabView*)tabView;
-(void)LoadDocument;

@end


#endif /* TWSNewtonScriptEditor_h */
