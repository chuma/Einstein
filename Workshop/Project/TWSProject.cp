//
//  TWSProject.cpp
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#include "TWSProject.h"

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
 Project settings as taken form "NTK for Windows":
 
	Application
		Name ("MyTest")
		Symbol ("|MyTest:MatthiasM|") -> kAppSymbol -> create slot "appSymbol"
		Auto-Close (Checkbox) - On MessagePad only: app closes if another opens
	Icon
		Icons and masks for B/W and 4 gray (ignore 16 and 256 grays!)
	Package
		Name ("|MyTest:SIG|") ??not the application symbol??
		Delete Old Package on Download (check)
		Copy Protected (uncheck)
		Auto Remove Package (uncheck) (remove package from machine after successful installation)
		Use Compression (uncheck)
		Faster Compression (Newton 2.0 only) (check)
		Copyright ("(c) Me!")
		Version (1) 0-9999, differentiates between version of the same app
	Project
		Platform (MsgPad, Newton2.0, Newton 2.1)
		Language (English)
		Compile for Debugging
		Compile for Profiling
		Ignore Native Keyword
		Profile Native Functions
		Check Global Function Calls
		Newton OS 2.0 Only
		NTK 1.0 Build Rules
		Faster Functions (2.0 Only)
		Use stepChildren slot (always on!)
		Tighter Object Packing (2.0 Only)
		Suppress Byte Code (generate native functions only)
	Output
		Output (Part Type)
			Application / Store Part / Book / Stream File / Auto Part / Custom Part
		Result
		New Style Stores (Newton OS 2.0 Only)
 */

/*
 Brain dump:
 
 All data exist in up to three locations:
  - in a file (a current format project file, or any imported file format)
  - in this class (in memory)
  - in the GUI
 
 All data can be changed at any time:
  - files can vanish or be rewritten by other apps
  - memory can be changed by GUI or other events
  - GUI datat changes by user input
 
 Possible actions:
  - user wants to compile -> classic: save to disk and compile file
  - user wants to save -> write GUI to memory then memory to disk?
 */


TWSProject::TWSProject(TWorkshop *inWorkshop)
:	TWSProjectItemWithChildren(inWorkshop),
	pFilename(0L),
// -- project settings
	pPlatform(21),
	pLanguage(strdup("English")),
	pDebugBuild(false),
	pIgnoreNative(false),
	pCheckGlobalFunctions(true),
	pOldBuildRules(false),
	pUseStepChildren(true),
	pSuppressByteCodes(false),
	pFasterFunctions(true),
// -- output settings
	pApplicationName(strdup("MyApp")),
	pApplicationSymbol(strdup("MyApp:Matt")),
	pPartType(0),
	pTopFrameExpression(0),
	pAutoClose(false),
	pCustomPartType(strdup("UNKN")),
	pFasterSoups(true),
// -- package settings
	pPackageName(strdup("MyApp")), // FIXME: hmm, is this what we want?
	pVersion(2),
	pCopyright(strdup("(c) 2017, MatthiasM")),
	pOptimizeSpeed(true),
	pCopyProtect(false),
	pDeleteOnDownload(false),
	pDispatchOnly(false),
	pFourByteAlignment(false),
	pZippyCompression(false),
	pNewton20Only(true),
// -- profiler settings
	pProfileMemory(false),
	pProfilePercent(true),
	pCompileForProfiling(false),
	pCompileForSpeed(true),
	pDetailedSystemCalls(true),
	pDetailedUserFunctions(true),
	// FIXME: find the default values from NTK and put those in here!
// -- compile into package
	pCode(0L), pnCode(0), pNCode(0)
{
}


TWSProject::~TWSProject()
{
	if (pFilename) { free(pFilename); pFilename = 0L; }
	if (pLanguage) { free(pLanguage); pLanguage = 0L; }
	if (pCode) { free(pCode); pCode = 0L; pnCode = pNCode = 0; }
}


bool TWSProject::CreateEditor(void *inParentView)
{
	pEditor = ProjectEditorCreate(this, inParentView);
	return true;
}


/**
 * Connect this document interface to a file on disk.
 */
void TWSProject::AssociateFile(const char *inFilename)
{
	if (pFilename && inFilename) {
		if (strcmp(pFilename, inFilename)==0)
			return;
	}
	if (pFilename) {
		free(pFilename);
		pFilename = 0L;
	}
	if (inFilename) {
		pFilename = strdup(inFilename);
		const char *nameOnly = strrchr(pFilename, '/');
		if (nameOnly) {
			nameOnly++;
		} else {
			nameOnly = pFilename;
		}
		SetName(nameOnly);
	}
}


void TWSProject::SetFromNewtFrame(newtRefArg inFrame, newtRefArg inSymbol, bool &outValue, bool dflt)
{
	int ix = NewtFindSlotIndex(inFrame, inSymbol);
	if (ix==-1) {
		outValue = dflt;
		return;
	}
	newtRef ref = NewtGetFrameSlot(inFrame, ix);
	outValue = (ref==kNewtRefTRUE);
}


void TWSProject::SetFromNewtFrame(newtRefArg inFrame, newtRefArg inSymbol, char *&outValue, const char *dflt)
{
	const char *v = dflt;
	int ix = NewtFindSlotIndex(inFrame, inSymbol);
	if (ix>=0) {
		newtRef ref = NewtGetFrameSlot(inFrame, ix);
		if (NewtRefIsString(ref)) {
			v = NewtRefToString(ref);
		}
	}
	if (v && outValue && strcmp(v, outValue)==0) {
		return;
	}
	if (outValue) {
		free(outValue);
		outValue = 0;
	}
	if (v) {
		outValue = strdup(v);
	}
}


void TWSProject::SetFromNewtFrame(newtRefArg inFrame, newtRefArg inSymbol, int &outValue, int dflt)
{
	int ix = NewtFindSlotIndex(inFrame, inSymbol);
	if (ix==-1) {
		outValue = dflt;
		return;
	}
	newtRef ref = NewtGetFrameSlot(inFrame, ix);
	if (!NewtRefIsInteger(ref)) {
		outValue = dflt;
		return;
	}
	outValue = NewtRefToInteger(ref);
}


newtRefVar TWSProject::GetFrameInFrame(newtRefArg inFrame, newtRefArg inSymbol)
{
	int ix = NewtFindSlotIndex(inFrame, inSymbol);
	if (ix==-1)
		return kNewtRefNIL;
	newtRefVar slot = NewtGetFrameSlot(inFrame, ix);
	if (!NewtRefIsFrameOrArray(slot))
		return kNewtRefNIL;
	return slot;
}


/**
 * Import an MS Windows based NTK project.
 *
 * Read an NTK Project and all associated files and resources from disk. 
 * Convert file content to utf8 and prepare for writing to new directory.
 */
int TWSProject::ImportWinNTK(const char *inSourceFile, const char *inDestinationDirectory)
{
	int result = -1;

	FILE * f = fopen(inSourceFile, "rb");
	if (!f) {
		fprintf(stderr, "No such file: %s\n", inSourceFile);
		return -1;
	}
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	uint8_t *data = (uint8_t*)malloc(size);
	fread(data, 1, size, f);
	fclose(f);

	newtRefVar newtProject;
	newtErr	err = kNErrNone;
	NewtInit(0, 0, 0);
	try {
		NVMInterpretStr("printDepth := 999;\n", &err);
		newtProject = NewtReadNSOF(data, size);
		NewtPrintObject(stdout, newtProject);

		if (!NewtRefIsFrame(newtProject)) throw "newtProject must be a NSOF frame";

		newtRef projectSettings = GetFrameInFrame(newtProject, NSSYM(projectSettings));
		if (projectSettings!=kNewtRefNIL)
		{
			char *platform = 0;
			SetFromNewtFrame(projectSettings, NSSYM(platform), platform, "Newton2.1");
			SetPlatform(platform);
			free(platform);

			SetFromNewtFrame(projectSettings, NSSYM(language), pLanguage, "English");
			SetFromNewtFrame(projectSettings, NSSYM(debugBuild), pDebugBuild, false);
			SetFromNewtFrame(projectSettings, NSSYM(ignoreNative), pIgnoreNative, false);
			SetFromNewtFrame(projectSettings, NSSYM(checkGlobalFunctions), pCheckGlobalFunctions, true);
			SetFromNewtFrame(projectSettings, NSSYM(oldBuildRules), pOldBuildRules, false);
			SetFromNewtFrame(projectSettings, NSSYM(useStepChildren), pUseStepChildren, true);
			SetFromNewtFrame(projectSettings, NSSYM(suppressByteCodes), pSuppressByteCodes, false);
			SetFromNewtFrame(projectSettings, NSSYM(fasterFunctions), pFasterFunctions, true);
		}

		newtRef outputSettings = GetFrameInFrame(newtProject, NSSYM(outputSettings));
		if (outputSettings!=kNewtRefNIL)
		{
			SetFromNewtFrame(outputSettings, NSSYM(applicationName), pApplicationName, "AppName");
			SetFromNewtFrame(outputSettings, NSSYM(applicationSymbol), pApplicationSymbol, "AppName:Elektriktrick");
			SetFromNewtFrame(outputSettings, NSSYM(partType), pPartType, 0);
			SetFromNewtFrame(outputSettings, NSSYM(topFrameExpression), pTopFrameExpression, "");
			SetFromNewtFrame(outputSettings, NSSYM(autoClose), pAutoClose, true);
			SetFromNewtFrame(outputSettings, NSSYM(customPartType), pCustomPartType, "UNKN");
			SetFromNewtFrame(outputSettings, NSSYM(fasterSoups), pFasterSoups, false);
			// TODO: icon
			// I would like to avoid handling icons here. Instead, we could
			// allow all sorts of images, adding a pulldown that indicates the
			// use of the image insde the packe. Using it as the app icon is
			// just one option out of many.
		}

		newtRef packageSettings = GetFrameInFrame(newtProject, NSSYM(packageSettings));
		if (packageSettings!=kNewtRefNIL)
		{
			SetFromNewtFrame(packageSettings, NSSYM(packageName), pPackageName, "PkgName");
			SetFromNewtFrame(packageSettings, NSSYM(version), pVersion, 0);
			SetFromNewtFrame(packageSettings, NSSYM(copyright), pCopyright, "(C) MatthiasM");
			SetFromNewtFrame(packageSettings, NSSYM(optimizeSpeed), pOptimizeSpeed, true);
			SetFromNewtFrame(packageSettings, NSSYM(copyProtect), pCopyProtect, false);
			SetFromNewtFrame(packageSettings, NSSYM(deleteOnDownload), pDeleteOnDownload, false);
			SetFromNewtFrame(packageSettings, NSSYM(dispatchOnly), pDispatchOnly, false);
			SetFromNewtFrame(packageSettings, NSSYM(fourByteAlignment), pFourByteAlignment, true);
			SetFromNewtFrame(packageSettings, NSSYM(zippyCompression), pZippyCompression, true);
			SetFromNewtFrame(packageSettings, NSSYM(newton20Only), pNewton20Only, false);
		}

		newtRef profilerSettings = GetFrameInFrame(newtProject, NSSYM(profilerSettings));
		if (profilerSettings!=kNewtRefNIL)
		{
			SetFromNewtFrame(profilerSettings, NSSYM(memory), pProfileMemory, true);
			SetFromNewtFrame(profilerSettings, NSSYM(percent), pProfilePercent, true);
			SetFromNewtFrame(profilerSettings, NSSYM(compileForProfiling), pCompileForProfiling, false);
			SetFromNewtFrame(profilerSettings, NSSYM(compileForSpeed), pCompileForSpeed, false);
			SetFromNewtFrame(profilerSettings, NSSYM(detailedSystemCalls), pDetailedSystemCalls, false);
			SetFromNewtFrame(profilerSettings, NSSYM(detailedUserFunctions), pDetailedUserFunctions, true);
		}

		newtRef projectItems = GetFrameInFrame(newtProject, NSSYM(projectItems));
		if (projectItems!=kNewtRefNIL)
		{
			newtRef items = GetFrameInFrame(projectItems, NSSYM(items));
			if (items!=kNewtRefNIL)
			{
				int n = NewtArrayLength(items);
				for (int i=0; i<n; i++) {
					newtRefVar item = NewtGetArraySlot(items, i);
					int type;
					SetFromNewtFrame(item, NSSYM(type), type, -1);
					switch (type) {
						case 0: // 0 Layout file (also used for user-proto and print layout files)
							fprintf(stderr, "Layout files not supported yet\n");
							// TWSLayoutDocument
							break;
						case 1: // 1 Bitmap file
							fprintf(stderr, "Bitmap files not supported yet\n");
							break;
						case 3: // 3 Sound file
							fprintf(stderr, "Sound files not supported yet\n");
							break;
						case 4: // 4 Book file (deprecated in favor of script items)
							fprintf(stderr, "Book files not supported yet\n");
							break;
						case 5: // 5 Script file (NewtonScript source file)
							fprintf(stderr, "Script files not supported yet\n");
							// TWSNewtonScriptDocument
							break;
						case 6: // 6 Package file
							fprintf(stderr, "Package files not supported yet\n");
							break;
						case 7: // 7 Stream file
							fprintf(stderr, "Stream files not supported yet\n");
							break;
						case 8: // 8 Native C++ code module file
							fprintf(stderr, "Native C++ files not supported yet\n");
							break;
						default:
							fprintf(stderr, "Unexpected item type in project item list: %d\n", type);
							break;
					}
				}
				// TODO: loop through all items in the array and create some reference
				// These can be layouts, scripts, etc.
				// file: {
				//		class: 'fileReference,
				//		projectPath: "c:\windows\profiles\matt\Desktop\Newton\NewtonDev\ROMVersion\",
				//		deltaFromProject: "ROMVersion.lyt"
				//	},
				//	type: 0,
				//	isMainLayout: TRUE
			}
		}

		result = 0;
	} catch(const char*) {
		NewtCleanup();
		free(data);
	}

	return result;
}


void TWSProject::SetPlatform(char *inName)
{
	if (!inName || !*inName) {
		pPlatform = 0; // unkknown
	} else if (strcasecmp(inName, "MsgPad")==0) {
		pPlatform = 1;
	} else if (strcasecmp(inName, "Newton2.0")==0) {
		pPlatform = 20;
	} else if (strcasecmp(inName, "Newton2.1")==0) {
		pPlatform = 21;
	}
}


void UpdateCString(char* &outStr, const char *inStr)
{
	if (outStr && inStr) {
		if (strcmp(outStr, inStr)==0)
			return;
	}
	if (outStr) {
		free(outStr);
		outStr = 0L;
	}
	if (inStr) {
		outStr = strdup(inStr);
	}
}


// -- compile into package


void TWSProject::ClearCode()
{
	pnCode = 0;
	if (pCode) pCode[0] = 0;
}


void TWSProject::AppendCode(const char *addCode)
{
	const int kChunkSize = 0x1000;
	const int kChunkMask = kChunkSize-1;
	if (!addCode) return;
	int nOld = pnCode;
	int nAdd = strlen(addCode);
	int nNew = nOld + nAdd;
	int nAlloc = nNew+1; // make room for trailing 0
	if (nAlloc>pNCode) {
		// allocate memory in chunks to reduce the number of reallocations
		nAlloc = (nNew+1 + kChunkSize) & ~kChunkMask;
		pCode = (char*)realloc(pCode, nAlloc);
		pNCode = nAlloc;
	}
	// append the new source code to the existing code
	memcpy(pCode+nOld, addCode, nAdd+1); // copy trailing 0
	pnCode = nNew;
}


void TWSProject::AppendFormattedCode(const char *format, ...)
{
	if (!format) return;
	char *buffer;
	va_list ap;
	va_start(ap, format);
	vasprintf(&buffer, format, ap);
	va_end(ap);
	if (buffer) {
		AppendCode(buffer);
		free(buffer);
	}
}


newtRefVar TWSProject::BuildDefaultIcon()
{
	static uint8_t bits[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x18,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3f, 0x00,
		0x00, 0x00, 0x33, 0x00, 0x1f, 0xff, 0x7b, 0x00, 0x3f, 0xff, 0x6e, 0x00, 0x30, 0x00, 0xce, 0x00,
		0x37, 0xfc, 0xcc, 0x00, 0x37, 0xfd, 0x9c, 0x00, 0x34, 0x05, 0x98, 0x00, 0x35, 0x53, 0x38, 0x00,
		0x34, 0x03, 0x30, 0x00, 0x35, 0x56, 0x70, 0x00, 0x34, 0x06, 0x60, 0x00, 0x35, 0x54, 0xe0, 0x00,
		0x34, 0x0c, 0xc0, 0x00, 0x35, 0x4f, 0xc0, 0x00, 0x34, 0x0b, 0x80, 0x00, 0x36, 0x0f, 0x00, 0x00,
		0x37, 0xfe, 0x80, 0x00, 0x37, 0xfd, 0x80, 0x00, 0x30, 0x0b, 0x80, 0x00, 0x18, 0x03, 0x00, 0x00,
		0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};
	static uint8_t mask[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x18,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3f, 0x00,
		0x00, 0x00, 0x3f, 0x00, 0x1f, 0xff, 0x7f, 0x00, 0x3f, 0xff, 0x7e, 0x00, 0x3f, 0xff, 0xfe, 0x00,
		0x3f, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xf8, 0x00,
		0x3f, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0xe0, 0x00,
		0x3f, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00,
		0x3f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x1f, 0xff, 0x00, 0x00,
		0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};
	newtRefVar bBits = NewtMakeBinary(NSSYM(bits), bits, sizeof(bits), false);
	newtRefVar bMask = NewtMakeBinary(NSSYM(mask), mask, sizeof(mask), false);
	int bw=24, bh=27;

	// create the remaining bits for the icon frame
	newtRefVar iconBoundsA[] = {
		NSSYM(left),	NewtMakeInt30(0),
		NSSYM(top),		NewtMakeInt30(0),
		NSSYM(bottom),	NewtMakeInt30(bh),
		NSSYM(right),	NewtMakeInt30(bw)
	};
	newtRef iconBounds = NewtMakeFrame2(sizeof(iconBoundsA) / (sizeof(newtRefVar) * 2), iconBoundsA);

	newtRefVar iconA[] = {
		NSSYM(bits),	bBits,
		NSSYM(mask),	bMask,
		NSSYM(bounds),	iconBounds,
	};
	newtRef icon = NewtMakeFrame2(sizeof(iconA) / (sizeof(newtRefVar) * 2), iconA);

	return icon;
}


newtRefVar TWSProject::defaultInstallScript()
{
	static uint8_t installScriptBin[] = {
		0x20, 0xa4, 0x70, 0x19, 0x91, 0xa5, 0x7d, 0x1a, 0x29, 0x6f, 0x00, 0x49, 0x7d, 0x22, 0xc7, 0x00,
		0x11, 0xa7, 0x00, 0x07, 0x5f, 0x00, 0x3c, 0x7f, 0x00, 0x07, 0x24, 0xc2, 0xa6, 0x7e, 0x1b, 0x91,
		0x7b, 0x1b, 0x91, 0xc4, 0x6f, 0x00, 0x38, 0x27, 0x00, 0x0c, 0x1c, 0x1d, 0x1e, 0x28, 0x1f, 0x00,
		0x07, 0x3b, 0x00, 0x22, 0x5f, 0x00, 0x44, 0x00, 0x7f, 0x00, 0x07, 0x05, 0x7f, 0x00, 0x07, 0x06,
		0x6f, 0x00, 0x17, 0x22, 0x22, 0xa7, 0x00, 0x07, 0x00, 0x7b, 0x1f, 0x00, 0x08, 0x1f, 0x00, 0x09,
		0x2a, 0x6f, 0x00, 0x61, 0x7b, 0x7b, 0x1f, 0x00, 0x08, 0x39, 0x00, 0x7b, 0x1f, 0x00, 0x08, 0x22,
		0x98, 0x7b, 0x1f, 0x00, 0x0a, 0x22, 0x99, 0x02,
	};
	newtRef instructions = NewtMakeBinary(NSSYM(instructions), installScriptBin, sizeof(installScriptBin), false);

	newtRefVar literalsA[] = {
		NSSYM(vars), NSSYM(extras), NSSYM(IsArray), NSSYM(app), NewtMakeString("Extras Drawer", false),
		NewtMakeString("The application you just installed conflicts with another application."
					   " Please contact the application vendor for an updated version.", false),
		NSSYM(GetRoot), NSSYM(Notify), NSSYM(devInstallScript), NSSYM(HasSlot), NSSYM(installScript)
	};
	newtRef literals = NewtMakeArray2(NSSYM(literals), sizeof(literalsA)/sizeof(newtRefVar), literalsA);

	newtRefVar nextArgFrameA[] = {
		NSSYM(_nextArgFrame),	kNewtRefNIL,
		NSSYM(_parent),			kNewtRefNIL,
		NSSYM(_implementor),	kNewtRefNIL,
	};
	newtRef nextArgFrame = NewtMakeFrame2(sizeof(nextArgFrameA) / (sizeof(newtRefVar) * 2), nextArgFrameA);

	newtRefVar argFrameA[] = {
		NSSYM(_nextArgFrame),	nextArgFrame,
		NSSYM(_parent),			kNewtRefNIL,
		NSSYM(_implementor),	kNewtRefNIL,
		NSSYM(p),				kNewtRefNIL,
		NSSYM(i),				kNewtRefNIL,
		NSSYM(extras),			kNewtRefNIL,
		NSSYM(one),				kNewtRefNIL,
		NSSYM(one|iter),		kNewtRefNIL,
	};
	newtRef argFrame = NewtMakeFrame2(sizeof(argFrameA) / (sizeof(newtRefVar) * 2), argFrameA);

	newtRefVar scriptA[] = {
		NS_CLASS,				NSSYM(CodeBlock),
		NSSYM(instructions),	instructions,
		NSSYM(literals),		literals,
		NSSYM(argFrame),		argFrame,
		NSSYM(numArgs),			NewtMakeInt30(1),
		NSSYM(DebuggerInfo),	kNewtRefNIL
	};
	newtRef script = NewtMakeFrame2(sizeof(scriptA) / (sizeof(newtRefVar) * 2), scriptA);
	return script;
}


newtRefVar TWSProject::defaultRemoveScript()
{

	static uint8_t removeScriptBin[] = {
		0x7b, 0x18, 0x19, 0x2a, 0x6f, 0x00, 0x0e, 0x7b, 0x7b,
		0x18, 0x39, 0x5f, 0x00, 0x0f, 0x22, 0x02,
	};
	newtRef instructions = NewtMakeBinary(NSSYM(instructions), removeScriptBin, sizeof(removeScriptBin), false);

	newtRefVar literalsA[] = {
		NSSYM(devRemoveScript), NSSYM(HasSlot)
	};
	newtRef literals = NewtMakeArray2(NSSYM(literals), sizeof(literalsA)/sizeof(newtRefVar), literalsA);

	newtRefVar nextArgFrameA[] = {
		NSSYM(_nextArgFrame),	kNewtRefNIL,
		NSSYM(_parent),			kNewtRefNIL,
		NSSYM(_implementor),	kNewtRefNIL
	};
	newtRef nextArgFrame = NewtMakeFrame2(sizeof(nextArgFrameA) / (sizeof(newtRefVar) * 2), nextArgFrameA);

	newtRefVar argFrameA[] = {
		NSSYM(_nextArgFrame),	nextArgFrame,
		NSSYM(_parent),			kNewtRefNIL,
		NSSYM(_implementor),	kNewtRefNIL,
		NSSYM(p),				kNewtRefNIL
	};
	newtRef argFrame = NewtMakeFrame2(sizeof(argFrameA) / (sizeof(newtRefVar) * 2), argFrameA);
	// FIXME argFrame->map[0] = 0x0a000002 (probably marking the fact that the slot must not be sorted?)
	// FIXME seems to work anyways though

	newtRefVar scriptA[] = {
		NS_CLASS,				NSSYM(CodeBlock),
		NSSYM(instructions),	instructions,
		NSSYM(literals),		literals,
		NSSYM(argFrame),		argFrame,
		NSSYM(numArgs),			NewtMakeInt30(1),
		NSSYM(DebuggerInfo),	kNewtRefNIL
	};
	newtRef script = NewtMakeFrame2(sizeof(scriptA) / (sizeof(newtRefVar) * 2), scriptA);
	return script;
}


int TWSProject::CompileCodeToPackage()
{
#if 1
//	InspectorPrintf("Building package\n");

	newtRef theForm = kNewtRefNIL;
	newtRef theBase = kNewtRefNIL;

	//dtkPlatform->loadConstFile();

	// the new way which currently requires a script file on disk
	// write the package into a file
//	ExportPackageToText();
	// read it back into memory
	struct stat st;
	stat("/Users/matt/dev/Dyne/DyneTK/testing_script_writer.txt", &st);
	FILE *f = fopen("/Users/matt/dev/Dyne/DyneTK/testing_script_writer.txt", "rb");
	char *script = (char*)malloc(st.st_size);
	fread(script, 1, st.st_size, f);
	fclose(f);
	// compile and run ir
	newtErr	err;
	int prevDumpPC = NEWT_DUMPBC; NEWT_DUMPBC = 0;
	//NEWT_DEBUG = 0;
	//NEWT_TRACE = 1;
	NewtInit(0, 0L, 0);
	theForm = NVMInterpretStr(script, &err);
	//newt_result_message(result, err);
	NewtCleanup();
	NEWT_DUMPBC = prevDumpPC;
	// release the memory taken by the script
	free(script);
	// very simple error code output
	if (theForm==kNewtRefUnbind) {
		printf("**** ERROR while compiling or interpreting\n");
		newtRef a = NVMSelf();
		NewtPrintObject(stdout, a);
		a = NVMCurrentFunction();
		NewtPrintObject(stdout, a);
		a = NVMCurrentException();
		NewtPrintObject(stdout, a);
		/*
		 newtRef		NVMSelf(void);
		 newtRef		NVMCurrentFunction(void);
		 newtRef		NVMCurrentImplementor(void);
		 bool		NVMHasVar(newtRefArg name);
		 void		NVMThrowData(newtRefArg name, newtRefArg data);
		 void		NVMThrow(newtRefArg name, newtRefArg data);
		 void		NVMRethrow(void);
		 newtRef		NVMCurrentException(void);
		 void		NVMClearException(void);
		 */
//		if (err)
//			printf("**** %s: %s\n", newt_error_class(err), newt_error(err));
//		else
			printf("**** unknow error\n");
		return kNewtRefUnbind;
	} else {
	}

	//NewtPrintObject(stdout, theForm);

	theForm = kNewtRefUnbind;
	theForm = NcGetGlobalVar(NSSYM(theForm));
	theBase = NcGetGlobalVar(NSSYM(theBase));
	if (theForm==kNewtRefUnbind || theBase==kNewtRefUnbind) {
		printf("Error building project (no base or form found)!\n");
		return -1;
	}

	// the following little hack removes all local variables from the
	// base object to avoid a recursion when writing the package
	newtObjRef obj = NewtRefToPointer(theBase); //++++
	uint32_t i, index, len = NewtObjSlotsLength(obj);
	for (i=len-1; i>2; i--) {
		newtRefVar slot = NewtGetMapIndex(obj->as.map, i, &index);
		printf("Slot %d = <%s>\n", i, NewtRefToSymbol(slot)->name);
		NcRemoveSlot(theBase, slot);
	}
	//NewtPrintObject(stdout, theForm);
	//NewtPrintObject(stdout, theBase);
	// create the package
	newtRef icon = BuildDefaultIcon();

	newtRefVar dataA[] = {
		NSSYM(app),				NewtMakeSymbol(GetApplicationSymbol()),
		NSSYM(text),			NewtMakeString(GetName(), false),
		NSSYM(icon),			icon,
		NSSYM(theForm),			theForm,
		NSSYM(installScript),	defaultInstallScript(),
		NSSYM(removeScript),	defaultRemoveScript()
	};
	newtRef data = NewtMakeFrame2(sizeof(dataA) / (sizeof(newtRefVar) * 2), dataA);

	newtRefVar part0A[] = {
		NSSYM(class),			NSSYM(PackagePart),
		NSSYM(info),			NewtMakeBinary(NSSYM(binary), (uint8_t*)"A Newton Toolkit application", /*28*/24, false),
		NSSYM(flags),			NewtMakeInt30(129),
		NSSYM(type),			NewtMakeInt32('form'),
		NSSYM(data),			data
	};
	newtRef part0 = NewtMakeFrame2(sizeof(part0A) / (sizeof(newtRefVar) * 2), part0A);

	newtRef parts = NewtMakeArray(kNewtRefUnbind, 1);
	NewtSetArraySlot(parts, 0, part0);

	newtRefVar packageA[] = {
		NSSYM(class),			NSSYM(PackageHeader),
		NSSYM(type),			NewtMakeInt32('xxxx'),
		NSSYM(pkg_version),		NewtMakeInt32(0),
		NSSYM(version),			NewtMakeInt32(1),
		NSSYM(copyright),		NewtMakeString(GetCopyright(), false),
		NSSYM(name),			NewtMakeString(GetPackageName(), false),
		NSSYM(flags),			NewtMakeInt32(0x10000000),
		NSSYM(parts),			parts
	};
	newtRef package = NewtMakeFrame2(sizeof(packageA) / (sizeof(newtRefVar) * 2), packageA);

	newtRef rcvr = kNewtRefNIL;

	//NewtPrintObject(stdout, package);
	newtRef package_ = NsMakePkg(rcvr, package);
	//NewtPrintObject(stdout, package_);

	if (NewtRefIsBinary(package_)) {
		printf("Package build successfully (%dkBytes)\n", NewtBinaryLength(package_)/1024+1);
	} else {
		printf("Error assembling package\n");
		return -1;
	}


	printf("Writing package to %s\n", GetPackageName());
	if (!NewtRefIsBinary(package_)) {
		printf("Error: no package to write\n");
		return -1;
	}

	{
	int size = NewtBinaryLength(package_);
	uint8_t *data = NewtRefToBinary(package_);
	if (!size || !data) {
		printf("Error: package is empty\n");
		return -1;
	}

	FILE *f = fopen("/Users/matt/test.pkg", "wb");
	if (!f) {
		perror("Error opening file: ");
		return -1;
	}
	if (::fwrite(data, 1, size, f)!=(size_t)size) {
		perror("Error writing file: ");
		fclose(f);
		return -1;
	}
	fclose(f);
	printf("Done.\n");
	}


	// /Users/matt/dev/Dyne/DyneTK/test/mpg.pkg

	return 0;
#else
	if (pCode && pnCode) {
		newtRefVar	result;
		newtErr	err;

		NewtInit(0, 0, 0);
//		result = NVMInterpretStr(pCode, &err);
		result = NVMInterpretStr("return constant { tclass: 'PackageHeader };", &err);
		if (err != kNErrNone)
			NewtErrMessage(err);
		else
			NsPrint(kNewtRefNIL, result);
		NewtCleanup();
	}
	return -1;
#endif
}


int TWSProject::WritePackage(const char *filename)
{
	fprintf(stderr, "NOT YET IMPLEMENTED\n");
	return -1;
}


int TWSProject::Compile()
{
#if 0
	ClearCode();
	AppendFormattedCode("return {\n"
						"\tclass: 'PackageHeader,\n"
						"\ttype: 2021161080,\n"
						"\tpkg_version: 0,\n"
						"\tversion: 1,\n"
						"\tcopyright: \"%s\",\n"		// TODO: text must be escaped
						"\tname: \"%s\",\n"			// TODO: text must be escaped
						"\tflags: 268435456,\n"
						"\tparts: [\n",
						GetCopyright(),
						GetName());
	AppendAppPart();
	AppendFormattedCode("\t]\n"
						"}\n"
						);
	if (pCode && pnCode) fprintf(stderr, "--->\n%s\n<---\n", pCode);
#endif
	CompileCodeToPackage();

	return 0;
}


int TWSProject::AppendAppPart()
{
	AppendFormattedCode("{\n"
						"\tclass: 'PackagePart,\n"
						"\tinfo:\n"
						"\t\tMakeBinaryFromHex(\"41204E6577746F6E20546F6F6C6B6974206170706C69636174696F6E\", 'binary),\n" // FIXME: what is this?
						"\tflags: 129,\n"
						"\ttype: 1718579821,\n"
						"\tdata: {\n"
						"\t\tapp: '|%s|,\n" // GetApplicationSymbol() '|ROMVersion:FGSoft|
						"\t\ttext: \"%s\",\n" // GetApplicationName()  "ROMVersion"
						/*
						"\t\ticon: {\n"
						"\t\t\tbits:\n"
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
						 */
						"\t\ttheForm: {\n",
						GetApplicationSymbol(),
						GetApplicationName()
						);
	AppendFormattedCode("\t\t\tappSymbol: '|%s|\n", GetApplicationSymbol()); // |ROMVersion:FGSoft|
	AppendFormattedCode("\t\t}\n"
						// InstallScript
						// RemoveScript
						"\t}\n"
						"}\n"
						);
	/*
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
*/
	return 0;
}


int TWSProject::Upload()
{
	fprintf(stderr, "NOT YET IMPLEMENTED\n");
	return -1;
}


int TWSProject::Run()
{
	fprintf(stderr, "NOT YET IMPLEMENTED\n");
	return -1;
}




