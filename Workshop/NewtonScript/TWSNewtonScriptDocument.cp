//
//  TWSNewtonScriptDocument.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#include "TWSNewtonScriptDocument.h"


TWSNewtonScriptDocument::TWSNewtonScriptDocument(TWorkshop *inWorkshop)
:	TWSProjectItem(inWorkshop)
{
}


TWSNewtonScriptDocument::~TWSNewtonScriptDocument()
{
}


bool TWSNewtonScriptDocument::CreateEditor(void *inParentView)
{
	pEditor = NewtonScriptEditorCreate(this, inParentView);
	return true;
}
