//
//  TWorkshop.cpp
//  Einstein
//
//  Created by Matthias Melcher on 2/27/17.
//
//

#include "TWorkshop.h"

#include "TEmulator.h"
#include "TPlatformManager.h"


TWorkshop::TWorkshop(TEmulator* inEmulator) :
pEmulator(inEmulator)
{
}


TWorkshop::~TWorkshop()
{
}


void TWorkshop::CompileAndRun()
{
	// TODO: compile the app, build a pkg, install it, and run it
	fprintf(stderr, "Compile and Run the current Project\n");

	TPlatformManager *platform = pEmulator->GetPlatformManager();

	const char* result = 0L;

	platform->ClearLogEvent();
	platform->EvalNewtonScript("GetRoot().|MiniNewtApp:40hz|:Close();");
	puts("GetRoot().|MiniNewtApp:40hz|:Close();");
	result = platform->WaitForLogEvent(1.0);

	if (result) {
		printf("Result: %s\n", result);
	} else {
		puts("Sorry!");
	}

	platform->ClearLogEvent();
	platform->EvalNewtonScript("SafeRemovePackage(GetPkgRef(\"MiniNewtApp:40hz\", GetStores()[0]));");
	puts("SafeRemovePackage(GetPkgRef(\"MiniNewtApp:40hz\", GetStores()[0]));");
	result = platform->WaitForLogEvent(1.0);

	if (result) {
		printf("Result: %s\n", result);
	} else {
		puts("Sorry!");
	}

//	platform->ClearLogEvent();
	platform->InstallPackage("/Users/matt/dev/tntk2/MiniNewtApp.pkg");
//	puts("/Users/matt/dev/tntk2/MiniNewtApp.pkg");
//	result = platform->WaitForLogEvent(3.0);
//
//	if (result) {
//		printf("Result: %s\n", result);
//	} else {
//		puts("Sorry!");
//	}

	platform->ClearLogEvent();
	platform->EvalNewtonScript("GetPkgRef(\"MiniNewtApp:40hz\", GetStores()[0]);");
	puts("GetPkgRef(\"MiniNewtApp:40hz\", GetStores()[0]);");
	result = platform->WaitForLogEvent(2.0);

	if (result) {
		printf("Result: %s\n", result);
	} else {
		puts("Sorry!");
	}
	
	platform->ClearLogEvent();
	platform->EvalNewtonScript("GetRoot().|MiniNewtApp:40hz|:Open();");
	puts("GetRoot().|MiniNewtApp:40hz|:Open();");
	result = platform->WaitForLogEvent(2.0);

	if (result) {
		printf("Result: %s\n", result);
	} else {
		puts("Sorry!");
	}

}

