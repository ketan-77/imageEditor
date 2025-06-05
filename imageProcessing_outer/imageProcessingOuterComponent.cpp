#define UNICODE
#include <windows.h>
#include "imageProcessingOuterComponent.h"
#include "FromInner/imageProcessingInnerComponent.h"

class CImageProcessor : public IUserHandler
{
private:
	long m_cRef;
	IUnknown *m_pIUnknownInner;

public:
	CImageProcessor(void);
	~CImageProcessor(void);

	// IUknown specific method declarations (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	HRESULT __stdcall RegisterUser(std::string userName, std::string middleName, std::string surName); // IUserHandler method

	HRESULT __stdcall destauration(COLORREF, COLORREF &);
	HRESULT __stdcall sepia(COLORREF, COLORREF &);
	HRESULT __stdcall inversion(COLORREF, COLORREF &);

	HRESULT __stdcall InitializeInnerComponent(void);
	int createLogFile(const char *fileName);
	void addEntryInLogFile(const char *dataEntry);
	void closeLogFile(void);
};

class CImageProcessorClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	CImageProcessorClassFactory(void);
	~CImageProcessorClassFactory(void);

	// IUknown specific method declarations (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific method declaration
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void **);
	HRESULT __stdcall LockServer(BOOL);
};

// global declarations
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

// DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}

	return (TRUE);
}

// Implementation of CImageProcessor's constructor method
CImageProcessor::CImageProcessor(void)
{
	// init private data member
	m_cRef = 1;
	m_pIUnknownInner = NULL;

	InterlockedIncrement(&glNumberOfActiveComponents); // increment global counter
}

CImageProcessor::~CImageProcessor(void)
{
	if (logFile)
	{
		fprintf(logFile, "\n--------------------------------------------------------\n");
		fclose(logFile);
		logFile = NULL;
	}
	if (m_pIUnknownInner)
	{
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
	}
	InterlockedDecrement(&glNumberOfActiveComponents); // decrement global count
}

int CImageProcessor::createLogFile(const char *fileName)
{
	logFile = fopen(fileName, "w");
	if (logFile)
	{
		return 0;
	}
	return 1;
}
void CImageProcessor::addEntryInLogFile(const char *dataEntry)
{
	fprintf(logFile, dataEntry);
}

void CImageProcessor::closeLogFile(void)
{
	if (logFile)
	{
		fprintf(logFile, "\n------------------------------------------------------\n");
		fclose(logFile);
		logFile = NULL;
	}
}

HRESULT CImageProcessor::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IUserHandler *>(this);
	else if (riid == IID_IUserHandler)
		*ppv = static_cast<IUserHandler *>(this);
	else if (riid == IID_IDesaturation)
		return m_pIUnknownInner->QueryInterface(riid, ppv);
	else if (riid == IID_ISepia)
		return m_pIUnknownInner->QueryInterface(riid, ppv);
	else if (riid == IID_IInversion)
		return m_pIUnknownInner->QueryInterface(riid, ppv); // method coloring
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return (S_OK);
}
ULONG CImageProcessor::AddRef(void)
{
	InterlockedIncrement(&m_cRef); // increment global counter
	return (m_cRef);
}

ULONG CImageProcessor::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete (this);
		return (0);
	}

	return (m_cRef);
}

// Implementation of ISum,s method
HRESULT CImageProcessor::RegisterUser(std::string userName, std::string middleName, std::string surName)
{
	return (S_OK);
}

HRESULT __stdcall CImageProcessor::destauration(COLORREF ogPixel, COLORREF &dsPixel)
{
	return S_OK;
}

HRESULT __stdcall CImageProcessor::sepia(COLORREF ofPixel, COLORREF &sepPixel)
{
	return S_OK;
}

HRESULT __stdcall CImageProcessor::inversion(COLORREF ogPixel, COLORREF &invPixel)
{
	return S_OK;
}

HRESULT __stdcall CImageProcessor::InitializeInnerComponent(void)
{
	HRESULT hr;
	hr = CoCreateInstance(CLSID_IMGPROCESSING, reinterpret_cast<IUnknown *>(this), CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)&m_pIUnknownInner);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("IUnknown interface can not be obtained from inner component"), TEXT("Inner Server Error"), MB_ICONERROR);
		return E_FAIL;
	}
	// outer think he will get IUnkwnon ptr but he get INoAggregationIUnknown
	return S_OK;
}

// implementation of CImageProcessorrClassFactory's constructor
CImageProcessorClassFactory::CImageProcessorClassFactory(void)
{
	m_cRef = 1; // hardcoded init to anticipate possible failure
}

CImageProcessorClassFactory::~CImageProcessorClassFactory(void)
{
	//
}

HRESULT CImageProcessorClassFactory::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IClassFactory *>(this);
	}
	else if (riid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory *>(this);
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return (S_OK);
}

ULONG CImageProcessorClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef); // increment global counter
	return (m_cRef);
}

ULONG CImageProcessorClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete (this);
		return (0);
	}

	return (m_cRef);
}

// implementation of CSumSubstractClassFactory's IClassFactory's method
HRESULT CImageProcessorClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// variable declaration
	CImageProcessor *pCImageProcessor = NULL;
	HRESULT hr;
	if (pUnkOuter != NULL)
	{
		return (CLASS_E_NOAGGREGATION);
	}
	// create the instance of component i.e. of CImageProcessor
	pCImageProcessor = new CImageProcessor;
	if (pCImageProcessor == NULL)
	{
		return (E_OUTOFMEMORY);
	}

	hr = pCImageProcessor->InitializeInnerComponent();
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed to Init Inner Component"), TEXT("Inner Server Error"), MB_ICONERROR);
		pCImageProcessor->Release();
		return hr;
	}

	hr = pCImageProcessor->QueryInterface(riid, ppv);
	pCImageProcessor->Release();
	pCImageProcessor = NULL;
	return (hr);
}

HRESULT CImageProcessorClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&glNumberOfActiveComponents);
	else
		InterlockedDecrement(&glNumberOfActiveComponents);

	return (S_OK);
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	CImageProcessorClassFactory *pCImageProcessorClassFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_ImageProcessor)
		return (CLASS_E_CLASSNOTAVAILABLE);

	pCImageProcessorClassFactory = new CImageProcessorClassFactory;
	if (pCImageProcessorClassFactory == NULL)
		return (E_OUTOFMEMORY);

	hr = pCImageProcessorClassFactory->QueryInterface(riid, ppv);
	pCImageProcessorClassFactory->Release();

	return (hr);
}

extern "C" HRESULT __stdcall DLLCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return (S_OK);
	else
		return (S_FALSE);
}
