//
//  TWorkshop.cpp
//  Einstein
//
//  Created by Matthias Melcher on 2/27/17.
//
//

#include "TWorkshop.h"

#include "TEmulator.h"
#include "TPathHelper.h"
#include "TPlatformManager.h"
#include "Workshop/TWSProjectItem.h"
#include "Workshop/Project/TWSProject.h"
#include "Workshop/NewtonScript/TWSNewtonScriptDocument.h"

#include "Newt0/NewtCore.h"
#include "Newt0/NewtBC.h"
#include "Newt0/NewtVM.h"
#include "Newt0/NewtParser.h"
#include "Newt0/NewtNSOF.h"
#include "Newt0/NewtPrint.h"
#include "Newt0/NewtPkg.h"
#include "Newt0/version.h"

#include <sys/stat.h>

/*
 Comments by Matthias:
 
 Reasoning:
 
 To develop new software for the Newton requires quite a big setup. Using a
 Classic MacOS emulator like BasiliskII confronts users with the particularities
 of MacOS 8 and the hellish NTK and even hellisherer MPW. Using Eckhar köppens
 "tntk" on OS X works well, but requires a steep command line learning curve.
 
 The Einstein Workshop shall eventually provide a simple "it's just there"
 interface to write and test small and medium size Newton apps without the need 
 to install any additional tools.
 
 March 28th, 2017: I decided to change the implementation a bit. TWSProject
 shall contain all information required to build a multipart package

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
	Started the implementation of creating projects...
	Started the implementation of MSWindows NTK project import
 
 TODO: create, load, edit, and store Newton Script files inside the project
	Started the implementation of creating, loading, and editing scripts...

 TODO: create, load, edit, and store Newton Layout files inside the project
	Started the implementation of creating, loading, and editing scripts...

 TODO: compile projects into packages
 
 DONE: UI: the left field is supposed to hold a list of files in the project
 
 TODO: UI: the top field should be a syntax highlighting editor

 TODO: UI: the bottom field should output messages and allow for a Toolbox connection
 
 TODO: write importer for NTK projects
 
 TODO: write importer for packages and recreate the source code
 
 TODO: add support for other package parts, fonts, books, etc.
 
 TODO: maybe even integrate the ARM assembler and C compiler

 */



void DumpPkg(const char *filename)
{
	uint8_t *data;
	size_t size;
	newtErr err;
	newtRefVar pkg;

	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	data = (uint8_t*)malloc(size);
	fread(data, 1, size, f);
	NewtInit(0, 0, 0);
	NVMInterpretStr("printDepth := 10;\n", &err);
	pkg = NewtReadPkg(data, size);
	NewtPrintObject(stdout, pkg);
	NewtCleanup();
	free(data);
}



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



TWorkshop::TWorkshop(TEmulator* inEmulator)
:	pEmulator(inEmulator),
	pProject(0L)
{
}


TWorkshop::~TWorkshop()
{
	delete pProject;
}


int TWorkshop::CreateMinimalProject(const char *path, const char *filename, const char *name, const char *sig, int nFiles, ...)
{
	return -1;
}


int TWorkshop::CreateMinimalScript(const char *path, const char *filename, const char *name, const char *sig)
{
	return -1;
}


int TWorkshop::Load(const char *path, const char *filename)
{
	return -1;
}


void TWorkshop::NewProject(const char *name)
{
	pProject = new TWSProject(this);
#if 0
	pProject->SetName("TestProject");
	{
		TWSNewtonScriptDocument *src = new TWSNewtonScriptDocument(this);
		src->AssociateFile("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.txt");
//		src->SetName("test.lyt");
		pProject->AddChild(src);

		TWSProjectItemWithChildren *sources = new TWSProjectItemWithChildren(this);
		sources->SetName("Sources");
		pProject->AddChild(sources);
		{
			TWSNewtonScriptDocument *src = new TWSNewtonScriptDocument(this);
			src->SetName("test.lyt");
			sources->AddChild(src);
			TWSNewtonScriptDocument *src2 = new TWSNewtonScriptDocument(this);
			src2->SetName("main.nsc");
			sources->AddChild(src2);
		}
		TWSProjectItemWithChildren *resources = new TWSProjectItemWithChildren(this);
		resources->SetName("Resources");
		pProject->AddChild(resources);
		{
			TWSProjectItem *icon = new TWSProjectItem(this);
			icon->SetName("appIcon.png");
			resources->AddChild(icon);
		}
		TWSProjectItemWithChildren *products = new TWSProjectItemWithChildren(this);
		products->SetName("Products");
		pProject->AddChild(products);
		{
			TWSPackage *pkg = new TWSPackage(this);
			pkg->SetName("test.pkg");
			products->AddChild(pkg);
		}
	}
#elif 0
	//DumpPkg("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.pkg");
	pProject->SetName("ROMVersion.pkg");
	pProject->ImportWinNTK("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.ntk",
						   "/Users/matt/dev/Workshop/ROMVersion/");
#elif 1
	std::string path = TPathHelper::GetSerialPipeBasePath();
	path.append("Workshop/");
	::mkdir(path.c_str(), 0777);
	path.append("Test/");
	::mkdir(path.c_str(), 0777);
	// TODO: generate ntk project file
	CreateMinimalProject(path.c_str(), "test.newtonprojet", "Test", "EinsteinWS", 1, "test.newtonscript");
	// TODO: generate minimal script
	CreateMinimalScript(path.c_str(), "test.newtonscript", "Test", "EinsteinWS");
	// TODO: load all that as the current project
	Load(path.c_str(), "test.newtonprojet");
#else
	pProject->SetName("Hello");
	{
		TWSNewtonScriptDocument *src = new TWSNewtonScriptDocument(this);
		src->AssociateFile("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.txt");
		//		src->SetName("test.lyt");
		pProject->AddChild(src);

		TWSProjectItemWithChildren *sources = new TWSProjectItemWithChildren(this);
		sources->SetName("Sources");
		pProject->AddChild(sources);
		{
			TWSNewtonScriptDocument *src = new TWSNewtonScriptDocument(this);
			src->SetName("test.lyt");
			sources->AddChild(src);
			TWSNewtonScriptDocument *src2 = new TWSNewtonScriptDocument(this);
			src2->SetName("main.nsc");
			sources->AddChild(src2);
		}
	}
#endif
	UpdateProjectOutline();
	//DumpPkg("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.pkg");
}


void TWorkshop::UpdateProjectOutline()
{
	puts("Why are we here?");
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

#elif 0


	newtRefVar	result;
	newtErr	err;

	NewtInit(0, 0, 0);
	result = NVMInterpretStr("print(1+1);\n", &err);
	newt_result_message(result, err);
	NewtCleanup();

#else

	// /Users/matt/Desktop/Newton/NewtonDev/NewtonDev.1/Sample Code/Drawing and Graphics/Drawing-4/Drawing.pkg
	newtRefVar	result;
	newtErr	err = kNErrNone;

	NewtInit(0, 0, 0);
	uint8_t *data = (uint8_t*)malloc(1024*1024*1);
	size_t size;
#if 1
	FILE *f = fopen("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.pkg", "rb");
#else
	FILE *f = fopen("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.lyt", "rb");
#endif
	size = fread(data, 1, 1024*1024*1, f);
	fclose(f);

//	INITSYM(printLength);
//	INITSYM(printBinaries);

	result = NVMInterpretStr("printDepth := 999; printLength := 999; printBinaries := 1;\n", &err);
	newt_result_message(result, err);
#if 1
	result = NewtReadPkg(data, size);
#else
	result = NewtReadNSOF(data, size);
#endif
	//result = NVMInterpretStr("print(1+1);\n", &err);
	//newt_result_message(result, kNErrNone);
	NewtPrintObject(stdout, result);
	NewtCleanup();
	free(data);

#endif
	
}


const char pkg[] =
"{\n\
	class: 'PackageHeader,\n\
	type: 2021161080,\n\
	pkg_version: 0,\n\
	version: 1,\n\
	copyright: \"\uffffffc2ffffffa9\u2001 Frank Gruendel\",\n\
	name: \"ROMVersion:SIG\",\n\
	flags: 268435456,\n\
	parts: [\n\
		{\n\
			class: 'PackagePart,\n\
			info:\n\
			MakeBinaryFromHex(\"41204E6577746F6E20546F6F6C6B6974206170706C69636174696F6E\", 'binary),\n\
			flags: 129,\n\
			type: 1718579821,\n\
			data: {\n\
						  app: '|ROMVersion:FGSoft|,\n\
						  text: \"ROMVersion\",\n\
						  icon: {\n\
						  bits:\n\
							  MakeBinaryFromHex(\"000000000004000000000000001B0018000000000000000000001C0000003F00000033001FFF7B003FFF6E003000CE0037FCCC0037FD9C0034059800355338003403300035567000340660003554E000340CC000354FC000340B8000360F000037FE800037FD8000300B8000180300001FFF00000FFE000000000000\",\n\
												'bits),\n\
												mask:\n\
												MakeBinaryFromHex(\"000000000004000000000000001B0018000000000000000000001C0000003F0000003F001FFF7F003FFF7E003FFFFE003FFFFC003FFFFC003FFFF8003FFFF8003FFFF0003FFFF0003FFFE0003FFFE0003FFFC0003FFFC0003FFF80003FFF00003FFF80003FFF80003FFF80001FFF00001FFF00000FFE000000000000\",\n\
																  'mask),\n\
																  bounds: {\n\
																  left: 0,\n\
																  top: 0,\n\
																  bottom: 27,\n\
																  right: 24\n\
																  }\n\
																  },\n\
																  theForm: {\n\
																  viewBounds: {\n\
																  left: 1073741821,\n\
																  top: 18,\n\
																  bottom: 136,\n\
																  right: 215\n\
																  },\n\
																  stepChildren: [\n\
																				 stepChildren:\n\
																				 {\n\
																				 text: \"Static Text\",\n\
																				 viewBounds: {\n\
																				 left: 4,\n\
																				 top: 4,\n\
																				 right: 214,\n\
																				 bottom: 104\n\
																				 },\n\
																				 viewSetupDoneScript: {\n\
																				 class:\n\
																				 'CodeBlock,\n\
																				 instructions:\n\
																				 MakeBinaryFromHex(\"18191A1B291C911D8BC70016A3031E7B1F00072B02\",\n\
																								   'instructions),\n\
																								   literals: [\n\
																											  literals:\n\
																											  \"ROM version = \",\n\
																											  \" \",\n\
																											  16777219,\n\
																											  'Gestalt,\n\
																											  'romVersionString,\n\
																											  'array,\n\
																											  'text,\n\
																											  'SetValue\n\
																											  ],\n\
																								   argFrame: {\n\
																								   _nextArgFrame:\n\
																									   {\n\
																									   _nextArgFrame:\n\
																										   NIL,\n\
																									   _parent:\n\
																										   NIL,\n\
																									   _implementor:\n\
																										   NIL\n\
																									   },\n\
																								   _parent:\n\
																									   NIL,\n\
																								   _implementor:\n\
																									   NIL,\n\
																								   theText:\n\
																									   NIL\n\
																								   },\n\
																								   numArgs: 0\n\
																								   },\n\
																								   viewJustify: 0,\n\
																								   viewFont: @88,\n\
																								   _proto: @218\n\
																								   }\n\
																								   ],\n\
																								   _proto: @180,\n\
																								   appSymbol: '|ROMVersion:FGSoft|\n\
																								   },\n\
																								   installScript: {\n\
																									   class: 'CodeBlock,\n\
																								   instructions:\n\
																									   MakeBinaryFromHex(\"20A4701991A57D1A296F00497D22C70011A700075F003C7F000724C2A67E1B917B1B91C46F003827000C1C1D1E281F00073B00225F0044007F0007057F0007066F00172222A70007007B1F00081F00092A6F00617B7B1F000839007B1F000822987B1F000A229902\",\n\
																														 'instructions), \n\
																														 literals: [\n\
																																	literals:\n\
																																	'vars, \n\
																																	'extras, \n\
																																	'IsArray, \n\
																																	'app, \n\
																																	\"Extras Drawer\", 
																																	\"The application you just installed conflicts with another application. Please contact the application vendor for an updated version.\",\n\
																																	'GetRoot, \n\
																																	'Notify, \n\
																																	'devInstallScript,\n\
																																	'HasSlot, \n\
																																	'installScript\n\
																																	], \n\
																														 argFrame: {\n\
																														 _nextArgFrame: {\n\
																														 _nextArgFrame: NIL,\n\
																														 _parent: NIL, \n\
																														 _implementor: NIL\n\
																														 }, \n\
																														 _parent: NIL,\n\
																														 _implementor: NIL,\n\
																														 P: NIL, \n\
																														 i: NIL, \n\
																														 extras: NIL, \n\
																														 one: NIL, \n\
																															 |one\|iter|: NIL\n\
																														 }, \n\
																														 numArgs: 1\n\
																														 }, \n\
																														 removeScript: {\n\
																															 class: 'CodeBlock,\n\
																														 instructions:\n\
																															 MakeBinaryFromHex(\"7B18192A6F000E7B7B18395F000F2202\", 'instructions),\n\
																																			   literals: [\n\
																																						  literals:\n\
																																						  'devRemoveScript,\n\
																																						  'HasSlot\n\
																																						  ], \n\
																																			   argFrame: {\n\
																																			   _nextArgFrame: {\n\
																																			   _nextArgFrame: NIL,\n\
																																			   _parent: NIL, \n\
																																			   _implementor: NIL\n\
																																			   }, \n\
																																			   _parent: NIL,\n\
																																			   _implementor: NIL,\n\
																																			   P: NIL\n\
																																			   }, \n\
																																			   numArgs: 1\n\
																																			   }\n\
																																			   }\n\
																																			   }\n\
																																			   ]\n\
																																			   }\n\
";
