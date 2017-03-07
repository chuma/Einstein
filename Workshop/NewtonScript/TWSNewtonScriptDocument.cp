//
//  TWSNewtonScriptDocument.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#include "TWSNewtonScriptDocument.h"


TWSNewtonScriptDocument::TWSNewtonScriptDocument(TWorkshop *inWorkshop)
:	TWSProjectItem(inWorkshop),
	pFilename(0L)
{
}


TWSNewtonScriptDocument::~TWSNewtonScriptDocument()
{
	if (pFilename) free(pFilename);
}


bool TWSNewtonScriptDocument::CreateEditor(void *inParentView)
{
	pEditor = NewtonScriptEditorCreate(this, inParentView);
	return true;
}


/** 
 * Connect this document interface to a file on disk.
 */
void TWSNewtonScriptDocument::AssociateFile(const char *inFilename)
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


/**
 * Load the content of the associated file and provide a UTF8 string.
 * Result must be freed by caller!
 *
 * TODO: this class must manage getting text from editor or from the file, whichever is appropriate 
 */
char *TWSNewtonScriptDocument::GetText()
{
	FILE *f = fopen(pFilename, "rb");
	if (!f) {
		perror("Can't open file for reading");
		return 0L;
	}
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *text =  (char*)malloc(size+1);
	int found = fread(text, 1, size, f);
	if (found!=size) {
		perror("Can't read script form file");
	}
	text[size] = 0;
	fclose(f);
	return text;
}

