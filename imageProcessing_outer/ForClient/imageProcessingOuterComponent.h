#pragma once
#include <iostream>

class IUserHandler : public IUnknown
{
public:
	// ISum specific method declarations
	virtual HRESULT __stdcall RegisterUser(std::string userName, std::string middleName, std::string surName) = 0; //pure virtual
	virtual int createLogFile(const char *fileName) = 0;            
	virtual void addEntryInLogFile(const char *dataEntry) = 0;
	virtual void closeLogFile(void) = 0;
protected:
	FILE *logFile;
};


// CLSID of ImageProcessor comp {964866E5-7D21-4486-B628-99B212A06AE2}
const CLSID CLSID_ImageProcessor = { 0x964866e5, 0x7d21, 0x4486, 0xb6, 0x28, 0x99, 0xb2, 0x12, 0xa0, 0x6a, 0xe2 };

//IID of IUserHandler Interface {CA2B909B-1F04-4E41-A857-2B5B82BB3BBA}
const IID IID_IUserHandler = { 0xca2b909b, 0x1f04, 0x4e41, 0xa8, 0x57, 0x2b, 0x5b, 0x82, 0xbb, 0x3b, 0xba };

