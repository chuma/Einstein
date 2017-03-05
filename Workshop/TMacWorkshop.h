//
//  TMacWorkshop.h
//  Einstein
//
//  Created by Matthias Melcher on 2/27/17.
//
//

#ifndef TMacWorkshop_h
#define TMacWorkshop_h

#include "Workshop/TWorkshop.h"

@class TCocoaWorkshopController;


class TMacWorkshop : public TWorkshop
{
public:
	TMacWorkshop(TCocoaWorkshopController *inWorkshopController, TEmulator *inEmulator);
	virtual ~TMacWorkshop();
protected:
	virtual void UpdateProjectOutline();
private:
	TCocoaWorkshopController *workshopController;
};



#endif /* TMacWorkshop_h */
