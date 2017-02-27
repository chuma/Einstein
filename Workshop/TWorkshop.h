//
//  TWorkshop.h
//  Einstein
//
//  Created by Matthias Melcher on 2/27/17.
//
//

#ifndef TWorkshop_h
#define TWorkshop_h


class TEmulator;


class TWorkshop
{
public:
	TWorkshop(TEmulator *emulator);
	~TWorkshop();
	void CompileAndRun();
protected:
	TEmulator*	pEmulator;
};


#endif /* TWorkshop_h */
