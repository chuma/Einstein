//
//  TWSProject.hpp
//  Einstein
//
//  Created by Matthias Melcher on 3/4/17.
//
//

#ifndef TWSProject_hpp
#define TWSProject_hpp

#include <vector>

class TWorkshop;
class TWSProjectItem;

typedef std::vector<TWSProjectItem*> TWSProjectItemList;


/**
 \brief A class that holds any item in the editing tree.
 */
class TWSProjectItem
{
public:
	TWSProjectItem(TWorkshop*);
	virtual ~TWSProjectItem();
	virtual bool CreateEditor(void *inParentView) { return false; }
	TWSProjectItem *SetName(const char*);
	const char *GetName();
	virtual const char *GetIcon() { return "tree_file.png"; }
	virtual int GetNumChildren() { return 0; }
	virtual TWSProjectItem *GetChild(int i) { return 0L; }
	virtual bool CanHaveChildren() { return false; }
	void *outlineId, *outlineTextId, *pEditor; // TODO: these variables should ne be here!
private:
	TWorkshop*	pWorkshop;
	char *pName;
};


class TWSProjectItemWithChildren : public TWSProjectItem
{
public:
	TWSProjectItemWithChildren(TWorkshop*);
	virtual ~TWSProjectItemWithChildren();
	virtual int GetNumChildren();
	virtual TWSProjectItem *GetChild(int i);
	virtual const char *GetIcon() { return "tree_folder.png"; }
	virtual bool CanHaveChildren() { return true; }
	void AddChild(TWSProjectItem* inChild);
protected:
	TWSProjectItemList pChildList;
};


#endif /* TWSProject_hpp */
