//
//  TMacWorkshop.m
//  Einstein
//
//  Created by Matthias Melcher on 2/27/17.
//
//

#include "Workshop/TMacWorkshop.h"
#include "TCocoaWorkshopController.h"


TMacWorkshop::TMacWorkshop(TCocoaWorkshopController *inController, TEmulator* inEmulator) :
	workshopController(inController),
	TWorkshop(inEmulator)
{
}


TMacWorkshop::~TMacWorkshop()
{
}


//void TMacWorkshop::SetController(TCocoaWorkshopController *inController)
//{
//	controller = inController;
//}


void TMacWorkshop::UpdateProjectOutline()
{
	WorkshopControllerUpdateProjectOutline(workshopController);
}
