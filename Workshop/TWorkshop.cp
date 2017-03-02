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


#include "Newt0/NewtCore.h"
#include "Newt0/NewtBC.h"
#include "Newt0/NewtVM.h"
#include "Newt0/NewtParser.h"
#include "Newt0/version.h"

/*
 Comments by Matthias:
 
 Reasoning:
 
 To develop new software for the Newton requires quite a big setup. Using a
 Classic MacOS emulator like BasiliskII confronts users with the particularities
 of MacOS 8 and the hellish NTK and even hellisherer MPW. Using Eckhar köppens
 "tntk" on OS X works well, but requires a steep command line learning curve.
 
 The EInstein Workshop shall eventually provide a simple "it's just there"
 interface to write and test small and medium size Newton apps without the need 
 to install any additional tools.

 DONE:
	Even though I do know nothing about Cocoa or ObjectiveC, I managed to got a
	somewhat decent basic layout for an IDE to run. Proof-of-concept style.
 DONE: 
	Newt/0 in a fork at https://github.com/MatthiasWM/NEWT0.git
	compiles and runds in 64 bit mode
 DONE:
	Newt/0 links and actually works inside Einstein. We will use Newt/0 to
	compile Newton Script code and store it as a .pkg file.
 DONE:
	I managed to move remove packages in the emulator, replace them with a new
	version of that package, and run it. Basically everything required to
	do little development iterations.
 
 TODO: Newt/0 must clean up the long pointer hash on NewtCleanup()
 
 TODO: create, load, and store a Newton project file
 
 TODO: create, load, edit, and store Newton Script files inside the project
 
 TODO: compile projects into packages
 
 TODO: UI: the left field is supposed to hold a list of files in the project
 
 TODO: UI: the top field should be a syntax highlighting editor

 TODO: UI: the bottom field should output messages and allow for a Toolbox connection
 
 TODO: write importer for NTK projects
 
 TODO: write importer for packages and recreate the source code
 
 TODO: add support for other package parts, fonts, books, etc.
 
 TODO: maybe even integrate the ARM assembler and C compiler

 */





void newt_result_message(newtRefArg r, newtErr err)
{
	if (err != kNErrNone)
	NewtErrMessage(err);
	else if (NEWT_DEBUG)
	NsPrint(kNewtRefNIL, r);
}


/*
 * Needed by Newt/0
 */
extern "C"
void yyerror(char * s)
{
	if (s[0] && s[1] == ':')
	NPSErrorStr(*s, s + 2);
	else
	NPSErrorStr('E', s);
}



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


#if 0

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

#else


	newtRefVar	result;
	newtErr	err;

	NewtInit(0, 0, 0);
	result = NVMInterpretStr("print(1+1);\n", &err);
	newt_result_message(result, err);
	NewtCleanup();
	
	
#endif
	
}




