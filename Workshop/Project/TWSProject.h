//
//  TWSProject.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSProject_h
#define TWSProject_h

#include "Workshop/TWSProjectItem.h"


class TWSProject : public TWSProjectItemWithChildren
{
public:
	TWSProject(TWorkshop*);
	~TWSProject();
	virtual const char *GetIcon() { return "tree_app.png"; }
	virtual bool CreateEditor(void *inParentView);
	void AssociateFile(const char *inFilename);

protected:
	char *pFilename;	// TODO: currently, this is the full path and name. We will need to differentiate this form the project relative filename
};



// ---- the functions below provide a platform independent interface to create
//		and manage the UI for Workshop Projects.
//		These must be implemented in the platform specific UI file.

typedef void (*TWSProjectEditorCallback)(void *inEditor);

// create and destroy a Project editor interface
void *ProjectEditorCreate(TWSProject *inItem, void* inParentView);
void ProjectEditorDestroy(void *inEditor);

// Set and get the text in the editor
//void ProjectEditorSetValues(void *inEditor, const char *text);
//const char* ProjectEditorGetValue(void *inEditor);

// Receive messages from the UI
//void ProjectEditorChangedCallback(TWSProjectEditorCallback* inCallback);


#endif /* TWSProject_h */
