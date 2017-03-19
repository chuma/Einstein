//
//  TWSLayoutEditor.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSLayoutEditor_h
#define TWSLayoutEditor_h

class TWSLayoutDocument;

#import <Cocoa/Cocoa.h>

@interface TWSLayoutEditor : NSObject //<NSTextFieldDelegate>
{
	IBOutlet NSTabViewItem* tabViewItem;
	IBOutlet NSView* viewFromNIB;
	IBOutlet NSTextView* textView;

	TWSLayoutDocument* documentBackRef;
}

+(id)LoadFromNIB:(TWSLayoutDocument*)inDocument;
-(void)AddTo:(NSTabView*)tabView;
-(void)LoadDocument;

@end


#endif /* TWSLayoutEditor_h */
