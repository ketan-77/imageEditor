#define UNICODE
#include <windows.h>
#include "imageProcessingInnerComponent.h"

interface INoAggregationIUnknown
{
	virtual HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void **) = 0;
	virtual ULONG __stdcall AddRef_NoAggregation(void) = 0;
	virtual ULONG __stdcall Release_NoAggregation(void) = 0;
};

class CImageProcessing : public INoAggregationIUnknown, IDesaturation, ISepia, IInversion
{
private:
	long m_cRef;
	IUnknown *m_pIUnknownOuter;

public:
	CImageProcessing(IUnknown *);
	~CImageProcessing(void);

	// Aggregation Nonsupported IUnknown specific method declaration (inherited)
	HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void **);
	ULONG __stdcall AddRef_NoAggregation(void);
	ULONG __stdcall Release_NoAggregation(void);

	// IUknown Aggregation supported specific method declarations (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	HRESULT __stdcall destauration(COLORREF, COLORREF &); // IDesaturation method
	HRESULT __stdcall sepia(COLORREF, COLORREF &);		  // ISepia method
	HRESULT __stdcall inversion(COLORREF, COLORREF &);	  // IInversion method
};

class CImageProcessingClassFactory : public IClassFactory
{
private:
	long m_cRef;

public:
	CImageProcessingClassFactory(void);
	~CImageProcessingClassFactory(void);

	// IUnknown specific method declarations (inherited)
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

// Implementation of CImageProcessing's constructor method
CImageProcessing::CImageProcessing(IUnknown *pIUnknownOuter)
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents); // increment global counter

	if (pIUnknownOuter)
	{
		m_pIUnknownOuter = pIUnknownOuter;
	}
	else
	{
		// you will come to this block when inner server directly called by client
		m_pIUnknownOuter = reinterpret_cast<IUnknown *>(static_cast<INoAggregationIUnknown *>(this));
	}
}

CImageProcessing::~CImageProcessing(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents); // decrement global count
}
// Aggreagation nonsupporting methods change 6/7/8
HRESULT CImageProcessing::QueryInterface_NoAggregation(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<INoAggregationIUnknown *>(this);
	else if (riid == IID_IDesaturation)
		*ppv = static_cast<IDesaturation *>(this);
	else if (riid == IID_ISepia)
		*ppv = static_cast<ISepia *>(this);
	else if (riid == IID_IInversion)
		*ppv = static_cast<IInversion *>(this);
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return (S_OK);
}
ULONG CImageProcessing::AddRef_NoAggregation(void)
{
	InterlockedIncrement(&m_cRef); // increment global counter
	return (m_cRef);
}

ULONG CImageProcessing::Release_NoAggregation(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete (this);
		return (0);
	}

	return (m_cRef);
}
// Aggregation supporting methods change 9/10/11
HRESULT CImageProcessing::QueryInterface(REFIID riid, void **ppv)
{
	return m_pIUnknownOuter->QueryInterface(riid, ppv);
}

ULONG CImageProcessing::AddRef(void)
{
	return m_pIUnknownOuter->AddRef();
}

ULONG CImageProcessing::Release(void)
{
	return m_pIUnknownOuter->Release();
}

// Implementation of IMultiplications,s method
HRESULT CImageProcessing::destauration(COLORREF originalPixelColor, COLORREF &dsPixelColor)
{
	unsigned int ogR = GetRValue(originalPixelColor);
	unsigned int ogG = GetGValue(originalPixelColor);
	unsigned int ogB = GetBValue(originalPixelColor);

	unsigned int dsR = (unsigned int)((float)(ogR * 0.3));
	unsigned int dsG = (unsigned int)((float)(ogG * 0.59));
	unsigned int dsB = (unsigned int)((float)(ogB * 0.11));

	unsigned int finalDsColor = dsR + dsG + dsB;

	dsPixelColor = RGB(finalDsColor, finalDsColor, finalDsColor);
	return (S_OK);
}

HRESULT CImageProcessing::sepia(COLORREF originalPixelColor, COLORREF &sepPixel)
{
	unsigned int ogR = GetRValue(originalPixelColor);
	unsigned int ogG = GetGValue(originalPixelColor);
	unsigned int ogB = GetBValue(originalPixelColor);

	unsigned int sepiaR = (unsigned int)(((float)ogR * 0.393f) + ((float)ogG * 0.769f) + ((float)ogB * 0.189f));
	sepiaR = (sepiaR > 255) ? 255 : sepiaR;

	unsigned int sepiaG = (unsigned int)(((float)ogR * 0.349f) + ((float)ogG * 0.686f) + ((float)ogB * 0.168f));
	sepiaG = (sepiaG > 255) ? 255 : sepiaG;

	unsigned int sepiaB = (unsigned int)(((float)ogR * 0.272f) + ((float)ogG * 0.534f) + ((float)ogB * 0.189f));
	sepiaB = (sepiaB > 255) ? 255 : sepiaB;

	sepPixel = RGB(sepiaR, sepiaG, sepiaB);

	return (S_OK);
}

HRESULT CImageProcessing::inversion(COLORREF originalPixelColor, COLORREF &invPixel)
{
	unsigned int ogR = GetRValue(originalPixelColor);
	unsigned int ogG = GetGValue(originalPixelColor);
	unsigned int ogB = GetBValue(originalPixelColor);

	unsigned int negativeR = 255 - ogR;
	negativeR < 255 ? 0 : negativeR;

	unsigned int negativeG = 255 - ogG;
	negativeG < 255 ? 0 : negativeG;

	unsigned int negativeB = 255 - ogB;
	negativeB < 255 ? 0 : negativeB;

	invPixel = RGB(negativeR, negativeB, negativeG);

	return (S_OK);
}

// implementation of CMulDivisionrClassFactory's constructor
CImageProcessingClassFactory::CImageProcessingClassFactory(void)
{
	m_cRef = 1; // hardcoded init to anticipate possible failure
}

CImageProcessingClassFactory::~CImageProcessingClassFactory(void)
{
	//
}

HRESULT CImageProcessingClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CImageProcessingClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef); // increment global counter
	return (m_cRef);
}

ULONG CImageProcessingClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete (this);
		return (0);
	}

	return (m_cRef);
}

// implementation of CImageProcessingClassFactory's IClassFactory's method
HRESULT CImageProcessingClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	// variable declaration
	CImageProcessing *pCImageProcessing = NULL;
	HRESULT hr;
	if ((pUnkOuter != NULL) && (riid != IID_IUnknown)) // change 12a - if condition
	{
		return (CLASS_E_NOAGGREGATION);
	}
	// create the instance of component i.e. of CImageProcessing
	pCImageProcessing = new CImageProcessing(pUnkOuter); // change 12b - param constructor
	if (pCImageProcessing == NULL)
	{
		return (E_OUTOFMEMORY);
	}

	hr = pCImageProcessing->QueryInterface_NoAggregation(riid, ppv);
	pCImageProcessing->Release_NoAggregation();
	return (hr);
}

HRESULT CImageProcessingClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&glNumberOfActiveComponents);
	else
		InterlockedDecrement(&glNumberOfActiveComponents);

	return (S_OK);
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	CImageProcessingClassFactory *pCMulDivisionClassFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_IMGPROCESSING)
		return (CLASS_E_CLASSNOTAVAILABLE);

	pCMulDivisionClassFactory = new CImageProcessingClassFactory;
	if (pCMulDivisionClassFactory == NULL)
		return (E_OUTOFMEMORY);

	hr = pCMulDivisionClassFactory->QueryInterface(riid, ppv);
	pCMulDivisionClassFactory->Release();

	return (hr);
}

extern "C" HRESULT __stdcall DLLCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return (S_OK);
	else
		return (S_FALSE);
}
