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
	TMacWorkshop(TEmulator *emulator);
	~TMacWorkshop();
	void SetController(TCocoaWorkshopController *inController);
protected:
private:
	TCocoaWorkshopController *controller;
};



#endif /* TMacWorkshop_h */
