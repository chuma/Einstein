//
//  TWSProject.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/4/17.
//
//

#include "TWSProjectItem.h"



TWSProjectItem::TWSProjectItem(TWorkshop *inWorkshop)
:	pWorkshop(inWorkshop),
	outlineId(0L),
	outlineTextId(0L),
	pEditor(0L),
	pName(0L)
{
}


TWSProjectItem::~TWSProjectItem()
{
	if (pName) free(pName);
}


TWSProjectItem *TWSProjectItem::SetName(const char *inName)
{
	if (pName) free(pName);
	if (inName) {
		pName = strdup(inName);
	} else {
		pName = 0L;
	}
	return this;
}


const char *TWSProjectItem::GetName()
{
	return pName;
}




TWSProjectItemWithChildren::TWSProjectItemWithChildren(TWorkshop *inWorkshop)
:	TWSProjectItem(inWorkshop)
{
}


TWSProjectItemWithChildren:: ~TWSProjectItemWithChildren()
{
}


int TWSProjectItemWithChildren::GetNumChildren()
{
	return (int)pChildList.size();
}


TWSProjectItem* TWSProjectItemWithChildren::GetChild(int inIndex)
{
	return pChildList.at(inIndex);
}


void TWSProjectItemWithChildren::AddChild(TWSProjectItem* inChild)
{
	pChildList.push_back(inChild);
}





