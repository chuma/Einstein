//
//  TWSPackageEditor.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSPackageEditor_h
#define TWSPackageEditor_h

class TWSPackage;

#import <Cocoa/Cocoa.h>

@interface TWSPackageEditor : NSObject //<NSTextFieldDelegate>
{
	IBOutlet NSTabViewItem* tabViewItem;
	IBOutlet NSView* viewFromNIB;
	IBOutlet NSTextView* textView;

	TWSPackage* documentBackRef;
}

+(id)LoadFromNIB:(TWSPackage*)inDocument;
-(void)AddTo:(NSTabView*)tabView;
-(void)LoadDocument;

@end


#endif /* TWSPackageEditor_h */
