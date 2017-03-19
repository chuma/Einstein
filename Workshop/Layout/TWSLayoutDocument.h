//
//  TWSLayoutDocument.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSLayoutDocument_h
#define TWSLayoutDocument_h

#include "Workshop/TWSProjectItem.h"


class TWSLayoutDocument : public TWSProjectItem
{
public:
	TWSLayoutDocument(TWorkshop*);
	~TWSLayoutDocument();
	virtual const char *GetIcon() { return "tree_nsc.png"; }
	virtual bool CreateEditor(void *inParentView);
	void AssociateFile(const char *inFilename);
	char *GetText();

protected:
	char *pFilename;	// TODO: currently, this is the full path and name. We will need to differentiate this form the project relative filename
};



// ---- the functions below provide a platform independent interface to create
//		and manage the UI for Layout Workshop Project Items.
//		These must be implemented in the platform specific UI file.

typedef void (*TWSLayoutEditorCallback)(void *inEditor);

// create and destroy a Layout editor interface
void *LayoutEditorCreate(TWSLayoutDocument *inItem, void* inParentView);
void LayoutEditorDestroy(void *inEditor);

// Set and get the text in the editor
void LayoutEditorSetText(void *inEditor, const char *text);
const char* LayoutEditorGetText(void *inEditor);

// Receive messages from the UI
void LayoutEditorChangedCallback(TWSLayoutEditorCallback* inCallback);


#endif /* TWSLayoutDocument_h */
