//
//  TWSPackage.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSPackage_h
#define TWSPackage_h

#include "Workshop/TWSProjectItem.h"


class TWSPackage : public TWSProjectItem
{
public:
	TWSPackage(TWorkshop*);
	~TWSPackage();
	virtual const char *GetIcon() { return "tree_nsc.png"; }
	virtual bool CreateEditor(void *inParentView);
	void AssociateFile(const char *inFilename);
	char *GetText();

protected:
	char *pFilename;	// TODO: currently, this is the full path and name. We will need to differentiate this form the project relative filename
};



// ---- the functions below provide a platform independent interface to create
//		and manage the UI for Package Workshop Project Items.
//		These must be implemented in the platform specific UI file.

typedef void (*TWSPackageEditorCallback)(void *inEditor);

// create and destroy a Package editor interface
void *PackageEditorCreate(TWSPackage *inItem, void* inParentView);
void PackageEditorDestroy(void *inEditor);

// Set and get the text in the editor
void PackageEditorSetText(void *inEditor, const char *text);
const char* PackageEditorGetText(void *inEditor);

// Receive messages from the UI
void PackageEditorChangedCallback(TWSPackageEditorCallback* inCallback);


#endif /* TWSPackage_h */
