//
//  TWSProject.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#include "TWSProject.h"


/*
 Project settings as taken form "NTK for Windows":
 
	Application
		Name ("MyTest")
		Symbol ("|MyTest:MatthiasM|") -> kAppSymbol -> create slot "appSymbol"
		Auto-Close (Checkbox) - On MessagePad only: app closes if another opens
	Icon
		Icons and masks for B/W and 4 gray (ignore 16 and 256 grays!)
	Package
		Name ("|MyTest:SIG|") ??not the application symbol??
		Delete Old Package on Download (check)
		Copy Protected (uncheck)
		Auto Remove Package (uncheck) (remove package from machine after successful installation)
		Use Compression (uncheck)
		Faster Compression (Newton 2.0 only) (check)
		Copyright ("(c) Me!")
		Version (1) 0-9999, differentiates between version of the same app
	Project
		Platform (MsgPad, Newton2.0, Newton 2.1)
		Language (English)
		Compile for Debugging
		Compile for Profiling
		Ignore Native Keyword
		Profile Native Functions
		Check Global Function Calls
		Newton OS 2.0 Only
		NTK 1.0 Build Rules
		Faster Functions (2.0 Only)
		Use stepChildren slot (always on!)
		Tighter Object Packing (2.0 Only)
		Suppress Byte Code (generate native functions only)
	Output
		Output (Part Type)
			Application / Store Part / Book / Stream File / Auto Part / Custom Part
		Result
		New Style Stores (Newton OS 2.0 Only)
 */

TWSProject::TWSProject(TWorkshop *inWorkshop)
:	TWSProjectItemWithChildren(inWorkshop),
	pFilename(0L)
{
}


TWSProject::~TWSProject()
{
	if (pFilename) free(pFilename);
}


bool TWSProject::CreateEditor(void *inParentView)
{
	pEditor = ProjectEditorCreate(this, inParentView);
	return true;
}


/**
 * Connect this document interface to a file on disk.
 */
void TWSProject::AssociateFile(const char *inFilename)
{
	if (pFilename && inFilename) {
		if (strcmp(pFilename, inFilename)==0)
			return;
	}
	if (pFilename) {
		free(pFilename);
		pFilename = 0L;
	}
	if (inFilename) {
		pFilename = strdup(inFilename);
		const char *nameOnly = strrchr(pFilename, '/');
		if (nameOnly) {
			nameOnly++;
		} else {
			nameOnly = pFilename;
		}
		SetName(nameOnly);
	}
}


