//
//  TMacWorkshop.m
//  Einstein
//
//  Created by Matthias Melcher on 2/27/17.
//
//

#include "Workshop/TMacWorkshop.h"


TMacWorkshop::TMacWorkshop(TEmulator* inEmulator) :
TWorkshop(inEmulator)
{
}


TMacWorkshop::~TMacWorkshop()
{
}


void TMacWorkshop::SetController(TCocoaWorkshopController *inController)
{
	controller = inController;
}


