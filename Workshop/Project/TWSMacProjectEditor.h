//
//  TWSProjectEditor.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSProjectEditor_h
#define TWSProjectEditor_h

class TWSProject;

#import <Cocoa/Cocoa.h>

@interface TWSProjectEditor : NSObject //<NSTextFieldDelegate>
{
	IBOutlet NSTabViewItem* tabViewItem;
	IBOutlet NSView* viewFromNIB;

	TWSProject* project;
}

+(id)LoadFromNIB:(TWSProject*)inProject;
-(void)AddTo:(NSTabView*)tabView;
-(void)LoadProject;

@end


#endif /* TWSProjectEditor_h */
