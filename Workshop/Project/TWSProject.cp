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
	pDetailedUserFunctions(true)
	// FIXME: find the defualt values from NTK and put those in here!
{
}


TWSProject::~TWSProject()
{
	if (pFilename) { free(pFilename); pFilename = 0L; }
	if (pLanguage) { free(pLanguage); pLanguage = 0L; }
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
	if (!NewtRefIsFrame(slot))
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
			newtRef items = GetFrameInFrame(newtProject, NSSYM(items));
			if (items!=kNewtRefNIL)
			{
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
