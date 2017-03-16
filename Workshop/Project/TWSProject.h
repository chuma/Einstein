//
//  TWSProject.h
//  Einstein
//
//  Created by Matthias Melcher on 3/6/17.
//
//

#ifndef TWSProject_h
#define TWSProject_h

#include "Workshop/TWSProjectItem.h"
#include "Newt0/NewtCore.h"
#include <string>


void UpdateCString(char* &outStr, const char *inStr);


class TWSProject : public TWSProjectItemWithChildren
{
public:
	TWSProject(TWorkshop*);
	~TWSProject();
	virtual const char *GetIcon() { return "tree_app.png"; }
	virtual bool CreateEditor(void *inParentView);
	void AssociateFile(const char *inFilename);

	int ImportWinNTK(const char *inSourceFile, const char *inDestinationDirectory);
	void SetFromNewtFrame(newtRefArg inFrame, newtRefArg inSymbol, bool &outValue, bool dflt=false);
	void SetFromNewtFrame(newtRefArg inFrame, newtRefArg inSymbol, char *&outValue, const char *dflt=0);
	void SetFromNewtFrame(newtRefArg inFrame, newtRefArg inSymbol, int &outValue, const int dflt=0);
	newtRefVar GetFrameInFrame(newtRefArg inFrame, newtRefArg inSymbol);

	// -- project settings

	int GetPlatform() { return pPlatform; }
	void SetPlatform(char*);
	void SetPlatform(int v) { pPlatform = v; }

	const char *GetLanguage() { return pLanguage; }
	void SetLanguage(const char *inLanguage) { UpdateCString(pLanguage, inLanguage); }

	bool IsDebugBuild() { return pDebugBuild; }
	void SetDebugBuild(bool v) { pDebugBuild = v; }

	bool IgnoreNative() { return pIgnoreNative; }
	void IgnoreNative(bool v) { pIgnoreNative = v; }

	bool CheckGlobalFunctions() { return pCheckGlobalFunctions; }
	void CheckGlobalFunctions(bool v) { pCheckGlobalFunctions = v; }

	bool UseOldBuildRules() { return pOldBuildRules; }
	void UseOldBuildRules(bool v) { pOldBuildRules = v; }

	bool UseStepChildren() { return pUseStepChildren; }
	void UseStepChildren(bool v) { pUseStepChildren = v; }

	bool SuppressByteCodes() { return pSuppressByteCodes; }
	void SuppressByteCodes(bool v) { pSuppressByteCodes = v; }

	bool UseFasterFunctions() { return pFasterFunctions; }
	void SetFasterFunctions(bool v) { pFasterFunctions = v; }

	// -- output settings

	const char *GetApplicationName() { return pApplicationName; }
	void SetApplicationName(const char *inApplicationName) { UpdateCString(pApplicationName, inApplicationName); }

	const char *GetApplicationSymbol() { return pApplicationSymbol; }
	void SetApplicationSymbol(const char *inApplicationSymbol) { UpdateCString(pApplicationSymbol, inApplicationSymbol); }

	int GetPartType() { return pPartType; }
	void SetPartType(int v) { pPartType = v; }

	const char *GetTopFrameExpression() { return pTopFrameExpression; }
	void SetTopFrameExpression(const char *inTopFrameExpression) { UpdateCString(pTopFrameExpression, inTopFrameExpression); }

	bool IsAutoClose() { return pAutoClose; }
	void SetAutoClose(bool v) { pAutoClose = v; }

	const char *GetCustomPartType() { return pCustomPartType; }
	void SetCustomPartType(const char *inCustomPartType) { UpdateCString(pCustomPartType, inCustomPartType); }

	bool UseFasterSoups() { return pFasterSoups; }
	void SetFasterSoups(bool v) { pFasterSoups = v; }

	// -- package settings

	const char *GetPackageName() { return pPackageName; }
	void SetPackageName(const char *inPackageName) { UpdateCString(pPackageName, inPackageName); }

	int GetVersion() { return pVersion; }
	void SetVersion(int v) { pVersion = v; }

	const char *GetCopyright() { return pCopyright; }
	void SetCopyright(const char *inCopyright) { UpdateCString(pCopyright, inCopyright); }

	bool OptimizeSpeed() { return pOptimizeSpeed; }
	void SetOptimizeSpeed(bool v) { pOptimizeSpeed = v; }

	bool CopyProtect() { return pCopyProtect; }
	void SetCopyProtect(bool v) { pCopyProtect = v; }

	bool DeleteOnDownload() { return pDeleteOnDownload; }
	void SetDeleteOnDownload(bool v) { pDeleteOnDownload = v; }

	bool DispatchOnly() { return pDispatchOnly; }
	void SetDispatchOnly(bool v) { pDispatchOnly = v; }

	bool UseFourByteAlignment() { return pFourByteAlignment; }
	void SetFourByteAlignment(bool v) { pFourByteAlignment = v; }

	bool UseZippyCompression() { return pZippyCompression; }
	void SetZippyCompression(bool v) { pZippyCompression = v; }

	bool IsNewton20Only() { return pNewton20Only; }
	void SetNewton20Only(bool v) { pNewton20Only = v; }

	// -- profiler settings

	bool ProfileMemory() { return pProfileMemory; }
	void SetProfileMemory(bool v) { pProfileMemory = v; }

	bool GetProfilePercent() { return pProfilePercent; }
	void SetProfilePercent(bool v) { pProfilePercent = v; }

	bool CompileForProfiling() { return pCompileForProfiling; }
	void SetCompileForProfiling(bool v) { pCompileForProfiling = v; }

	bool CompileForSpeed() { return pCompileForSpeed; }
	void SetCompileForSpeed(bool v) { pCompileForSpeed = v; }

	bool UseDetailedSystemCalls() { return pDetailedSystemCalls; }
	void SetDetailedSystemCalls(bool v) { pDetailedSystemCalls = v; }

	bool UseDetailedUserFunctions() { return pDetailedUserFunctions; }
	void SetDetailedUserFunctions(bool v) { pDetailedUserFunctions = v; }

protected:
	char *pFilename;	// TODO: currently, this is the full path and name. We will need to differentiate this form the project relative filename

	// -- project settings
	int pPlatform;
	char *pLanguage;
	bool pDebugBuild;
	bool pIgnoreNative;
	bool pCheckGlobalFunctions;
	bool pOldBuildRules;
	bool pUseStepChildren;
	bool pSuppressByteCodes;
	bool pFasterFunctions;

	// -- output settings
	char *pApplicationName;
	char *pApplicationSymbol;
	int pPartType;
	char *pTopFrameExpression;
	bool pAutoClose;
	char *pCustomPartType;
	bool pFasterSoups;
	// TODO: icon

	// -- package settings
	char *pPackageName;
	int pVersion;
	char *pCopyright;
	bool pOptimizeSpeed;
	bool pCopyProtect;
	bool pDeleteOnDownload;
	bool pDispatchOnly;
	bool pFourByteAlignment;
	bool pZippyCompression;
	bool pNewton20Only;

	// -- profiler settings
	bool pProfileMemory;
	bool pProfilePercent;
	bool pCompileForProfiling;
	bool pCompileForSpeed;
	bool pDetailedSystemCalls;
	bool pDetailedUserFunctions;

};

// ---- the functions below provide a platform independent interface to create
//		and manage the UI for Workshop Projects.
//		These must be implemented in the platform specific UI file.

typedef void (*TWSProjectEditorCallback)(void *inEditor);

// create and destroy a Project editor interface
void *ProjectEditorCreate(TWSProject *inItem, void* inParentView);
void ProjectEditorDestroy(void *inEditor);

// Set and get the text in the editor
void ProjectEditorUpdateUI(void *inEditor);
void ProjectEditorUpdateData(void *inEditor);

// Receive messages from the UI
//void ProjectEditorChangedCallback(TWSProjectEditorCallback* inCallback);

#endif /* TWSProject_h */
