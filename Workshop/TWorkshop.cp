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
#include "Workshop/TWSProjectItem.h"

#include "Newt0/NewtCore.h"
#include "Newt0/NewtBC.h"
#include "Newt0/NewtVM.h"
#include "Newt0/NewtParser.h"
#include "Newt0/NewtNSOF.h"
#include "Newt0/NewtPrint.h"
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



TWorkshop::TWorkshop(TEmulator* inEmulator)
:	pEmulator(inEmulator),
	pProject(0L)
{
}


TWorkshop::~TWorkshop()
{
	delete pProject;
}


void TWorkshop::NewProject(const char *name)
{
	pProject = new TWSProject(this);
	pProject->SetName("TestProject");
	{
		TWSProjectItemWithChildren *sources = new TWSProjectItemWithChildren(this);
		sources->SetName("Sources");
		pProject->AddChild(sources);
		{
			TWSProjectItem *src = new TWSProjectItem(this);
			src->SetName("test.lyt");
			sources->AddChild(src);
		}
		TWSProjectItemWithChildren *products = new TWSProjectItemWithChildren(this);
		products->SetName("Products");
		pProject->AddChild(products);
		{
			TWSProjectItem *pkg = new TWSProjectItem(this);
			pkg->SetName("test.pkg");
			products->AddChild(pkg);
		}
	}
	UpdateProjectOutline();
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
//	FILE *f = fopen("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.ntk", "rb");
	FILE *f = fopen("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.lyt", "rb");
	size = fread(data, 1, 1024*1024*1, f);
	fclose(f);
	result = NVMInterpretStr("printDepth := 10;\n", &err);
	newt_result_message(result, err);
	result = NewtReadNSOF(data, size);
	//result = NVMInterpretStr("print(1+1);\n", &err);
	//newt_result_message(result, kNErrNone);
	NewtPrintObject(stdout, result);
	NewtCleanup();
	free(data);

#endif
	
}


//	{
//		layoutSettings: {
//			ntkPlatform: 1,
//			ntkVersion: 1.000000,
//			windowRect: {
//				left: 55,
//				top: 64,
//				bottom: 455,
//				right: 334
//			},
//			layoutName: "",
//			layoutType: 0,
//			platformName: "MessagePad",
//			layoutSize: {
//				h: 240,
//				v: 336
//			},
//			gridState: NIL,
//			gridSnap: NIL,
//			gridSize: {
//				h: 5,
//				v: 5
//			}, ...
//
//		},
//		templateHierarchy: {
//			__ntObjectPointer: <Binary, class "pointer", length 4>,
//			value: {
//				__ntTemplate: {
//					value: 180,
//					__ntDatatype: "PROT",
//					__ntFlags: 16
//				},
//				viewBounds: {
//					value: {
//						left: -3,
//						top: 18,
//						bottom: 136,
//						right: 215
//					},
//					__ntDatatype: "RECT"
//				},
//				stepChildren: {
//					value: [
//						stepChildren:
//						{
//							__ntObjectPointer: <Binary, length 4>,
//							value: {
//								__ntTemplate: {
//								value: 218,
//								__ntDatatype: "PROT",
//								__ntFlags: 16
//							},
//							text: {
//								value: "\"Static Text\"",
//								__ntDatatype: "EVAL"
//							},
//							viewBounds: {
//								value: {
//									top: 4,
//									left: 4,
//									bottom: 104,
//									right: 214
//								},
//								__ntDatatype: "RECT"
//							},
//							viewSetupDoneScript: {
//								value: "func()\rbegin\r\tlocal theText := \"ROM version = \" && Gestalt(kGestalt_SystemInfo).ROMversionstring;\r\tsetValue(self, 'text, theText);\rend",
//								__ntDatatype: "SCPT",
//								__ntStatusInfo: {
//								state: 1,
//								by: 3709979
//							}
//						},
//						viewJustify: {
//							value: 0,
//							__ntDatatype: "NUMB"
//						},
//						viewFont: {
//							value: "ROM_fontSystem10Bold",
//							__ntDatatype: "EVAL"
//						}
//					},
//					__ntId: 'protoStaticText
//				}
//			],
//			__ntDatatype: "ARAY",
//			__ntFlags: 64
//		}
//	},
//__ntId: 'protoFloatNGo
//}
//}




