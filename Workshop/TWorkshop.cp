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
	Started the implementation of creating projects...
 
 TODO: create, load, edit, and store Newton Script files inside the project
	Started the implementation of creating, loading, and editing scripts...

 TODO: compile projects into packages
 
 TODO: UI: the left field is supposed to hold a list of files in the project
 
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
#else
	//DumpPkg("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.pkg");
	pProject->SetName("ROMVersion.pkg");
	pProject->ImportWinNTK("/Users/matt/Desktop/Newton/NewtonDev/ROMVersion/ROMVersion.ntk",
						   "/Users/matt/dev/Workshop/ROMVersion/");
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

#if 0

Adding ROM patch: Avoid screen calibration
Adding ROM patch: BeaconDetect (1/2)
Adding ROM patch: BeaconDetect (2/2)
Adding ROM patch: gDebugger patch
Adding ROM patch: gNewtConfig patch
Creating image from ROM and REX...
PATCHING THE ROM
1/19300 patches added 0.01%.
Removing 'extr' serial port driver from REx.
Checksum-0: 4B0119CA
Checksum-1: 1875D677
Checksum-2: 68481A07
Checksum-3: BBB55D42
Checksum-4: 2E7911DC
Checksum-5: 47558A61
Checksum-6: FFFFFFFF
Checksum-7: FFFFFFFF
Checksum-8: FFFFFFFF
Checksum-9: FFFFFFFF
Read 52817 symbols
0x0000618000638fe0
Compile and Run the current Project
{
	layoutSettings: {
		ntkPlatform: 1, 
		ntkVersion: 1.000000, 
		windowRect: {
			left: 55, 
			top: 64, 
			bottom: 455, 
			right: 334
		}, 
		layoutName: "", 
		layoutType: 0, 
		platformName: "MessagePad", 
		layoutSize: {
			h: 240, 
			v: 336
		}, 
		gridState: NIL, 
		gridSnap: NIL, 
		gridSize: {
			h: 5, 
			v: 5
		}, 
		previewState: NIL, 
		arrowKeyUnit: 1, 
		shiftKeyUnit: 5, 
		linkedTo: {
			class: 'fileReference, 
			projectPath: "D:\Newton\Newton Toolkit\DotClock 01s\", 
			deltaFromProject: ""
		}
	}, 
	templateHierarchy: {
		__ntObjectPointer: MakeBinaryFromHex("4CC2E200", 'pointer), 
		value: {
			__ntTemplate: {
				value: 180, 
				__ntDatatype: "PROT", 
				__ntFlags: 16
			}, 
			viewBounds: {
				value: {
					left: -3, 
					top: 18, 
					bottom: 136, 
					right: 215
				}, 
				__ntDatatype: "RECT"
			}, 
			stepChildren: {
				value: [
					stepChildren: 
					{
						__ntObjectPointer: MakeBinaryFromHex("ACC2E200", NIL), 
						value: {
							__ntTemplate: {
								value: 218, 
								__ntDatatype: "PROT", 
								__ntFlags: 16
							}, 
							text: {
								value: "\"Static Text\"", 
								__ntDatatype: "EVAL"
							}, 
							viewBounds: {
								value: {
									top: 4, 
									left: 4, 
									bottom: 104, 
									right: 214
								}, 
								__ntDatatype: "RECT"
							}, 
							viewSetupDoneScript: {
								value: "func()\rbegin\r\tlocal theText := \"ROM version = \" && Gestalt(kGestalt_SystemInfo).ROMversionstring;\r\tsetValue(self, 'text, theText);\rend", 
								__ntDatatype: "SCPT", 
								__ntStatusInfo: {
									state: 1, 
									by: 3709979
								}
							}, 
							viewJustify: {
								value: 0, 
								__ntDatatype: "NUMB"
							}, 
							viewFont: {
								value: "ROM_fontSystem10Bold", 
								__ntDatatype: "EVAL"
							}
						}, 
						__ntId: 'protoStaticText
					}
				], 
				__ntDatatype: "ARAY", 
				__ntFlags: 64
			}
		}, 
		__ntId: 'protoFloatNGo
	}
}

//-----------------

Adding ROM patch: Avoid screen calibration
Adding ROM patch: BeaconDetect (1/2)
Adding ROM patch: BeaconDetect (2/2)
Adding ROM patch: gDebugger patch
Adding ROM patch: gNewtConfig patch
Creating image from ROM and REX...
PATCHING THE ROM
1/19300 patches added 0.01%.
Removing 'extr' serial port driver from REx.
Checksum-0: 4B0119CA
Checksum-1: 1875D677
Checksum-2: 68481A07
Checksum-3: BBB55D42
Checksum-4: 2E7911DC
Checksum-5: 47558A61
Checksum-6: FFFFFFFF
Checksum-7: FFFFFFFF
Checksum-8: FFFFFFFF
Checksum-9: FFFFFFFF
Read 52817 symbols
0x0000608000c232a0
Compile and Run the current Project
*** PkgReader: PkgReadBinaryObject - unknown class 'mask
*** PkgReader: PkgReadBinaryObject - unknown class 'pointer
{
	class: 'PackageHeader, 
	type: 2021161080, 
	pkg_version: 0, 
	version: 1, 
	copyright: "\uffffffc2ffffffa9\u2001 Frank Gruendel", 
	name: "ROMVersion:SIG", 
	flags: 268435456, 
	parts: [
		{
			class: 'PackagePart, 
			info: MakeBinaryFromHex("41204E6577746F6E20546F6F6C6B6974206170706C69636174696F6E", 'binary), 
			flags: 129, 
			type: 1718579821, 
			data: {
				app: '|ROMVersion:FGSoft|, 
				text: "ROMVersion", 
				icon: {
					bits: MakeBinaryFromHex("000000000004000000000000001B0018000000000000000000001C0000003F00000033001FFF7B003FFF6E003000CE0037FCCC0037FD9C0034059800355338003403300035567000340660003554E000340CC000354FC000340B8000360F000037FE800037FD8000300B8000180300001FFF00000FFE000000000000", 'bits), 
					mask: MakeBinaryFromHex("000000000004000000000000001B0018000000000000000000001C0000003F0000003F001FFF7F003FFF7E003FFFFE003FFFFC003FFFFC003FFFF8003FFFF8003FFFF0003FFFF0003FFFE0003FFFE0003FFFC0003FFFC0003FFF80003FFF00003FFF80003FFF80003FFF80001FFF00001FFF00000FFE000000000000", 'mask), 
					bounds: {
						left: 0, 
						top: 0, 
						bottom: 27, 
						right: 24
					}
				}, 
				theForm: {
					viewBounds: {
						left: 1073741821, 
						top: 18, 
						bottom: 136, 
						right: 215
					}, 
					stepChildren: {
						value: [
							stepChildren: 
							{
								__ntObjectPointer: MakeBinaryFromHex("3C828B00", 'pointer), 
								value: {
									text: "Static Text", 
									viewBounds: {
										left: 4, 
										top: 4, 
										right: 214, 
										bottom: 104
									}, 
									viewSetupDoneScript: {
										class: 'CodeBlock, 
										instructions: MakeBinaryFromHex("18191A1B291C911D8BC70016A3031E7B1F00072B02", 'instructions), 
										literals: [
											literals: 
											"ROM version = ", 
											" ", 
											16777219, 
											'Gestalt, 
											'romVersionString, 
											'array, 
											'text, 
											'SetValue
										], 
										argFrame: {
											_nextArgFrame: {
												_nextArgFrame: NIL, 
												_parent: NIL, 
												_implementor: NIL
											}, 
											_parent: NIL, 
											_implementor: NIL, 
											theText: NIL
										}, 
										numArgs: 0
									}, 
									viewJustify: 0, 
									viewFont: @88, 
									_proto: @218
								}, 
								__ntId: 'protoStaticText
							}
						], 
						__ntDatatype: "ARAY", 
						__ntFlags: 64
					}, 
					_proto: @180, 
					viewChildren: [
						viewChildren: 
						{
							text: "Static Text", 
							viewBounds: {
								left: 4, 
								top: 4, 
								right: 214, 
								bottom: 104
							}, 
							viewSetupDoneScript: {
								class: 'CodeBlock, 
								instructions: MakeBinaryFromHex("18191A1B291C911D8BC70016A3031E7B1F00072B02", 'instructions), 
								literals: [
									literals: 
									"ROM version = ", 
									" ", 
									16777219, 
									'Gestalt, 
									'romVersionString, 
									'array, 
									'text, 
									'SetValue
								], 
								argFrame: {
									_nextArgFrame: {
										_nextArgFrame: NIL, 
										_parent: NIL, 
										_implementor: NIL
									}, 
									_parent: NIL, 
									_implementor: NIL, 
									theText: NIL
								}, 
								numArgs: 0
							}, 
							viewJustify: 0, 
							viewFont: @88, 
							_proto: @218
						}
					], 
					appSymbol: '|ROMVersion:FGSoft|
				}, 
				installScript: {
					class: 'CodeBlock, 
					instructions: MakeBinaryFromHex("20A4701991A57D1A296F00497D22C70011A700075F003C7F000724C2A67E1B917B1B91C46F003827000C1C1D1E281F00073B00225F0044007F0007057F0007066F00172222A70007007B1F00081F00092A6F00617B7B1F000839007B1F000822987B1F000A229902", 'instructions), 
					literals: [
						literals: 
						'vars, 
						'extras, 
						'IsArray, 
						'app, 
						"Extras Drawer", 
						"The application you just installed conflicts with another application. Please contact the application vendor for an updated version.", 
						'GetRoot, 
						'Notify, 
						'devInstallScript, 
						'HasSlot, 
						'installScript
					], 
					argFrame: {
						_nextArgFrame: {
							_nextArgFrame: NIL, 
							_parent: NIL, 
							_implementor: NIL
						}, 
						_parent: NIL, 
						_implementor: NIL, 
						P: NIL, 
						i: NIL, 
						extras: NIL, 
						one: NIL, 
						|one\|iter|: NIL
					}, 
					numArgs: 1
				}, 
				removeScript: {
					class: 'CodeBlock, 
					instructions: MakeBinaryFromHex("7B18192A6F000E7B7B18395F000F2202", 'instructions), 
					literals: [
						literals: 
						'devRemoveScript, 
						'HasSlot
					], 
					argFrame: {
						_nextArgFrame: {
							_nextArgFrame: NIL, 
							_parent: NIL, 
							_implementor: NIL
						}, 
						_parent: NIL, 
						_implementor: NIL, 
						P: NIL
					}, 
					numArgs: 1
				}
			}
		}
	]
}


#endif
