//Changes in AggregationInner are with respect to ContainmentInner
#include<windows.h>
#include"PixEditOneEffectImpl.h"
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

// interface declaration ( for internal use only. i.e. not to be included in .h file ) 
interface INoAggregationIUnknown //new 
{
	virtual HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**) = 0;
	virtual ULONG __stdcall AddRef_NoAggregation(void) = 0;
	virtual ULONG __stdcall Release_NoAggregation(void) = 0;
};

//class declaration
class CNegative : public INoAggregationIUnknown, INegative //this is co-class
{
private:
	long m_cRef;
	IUnknown* m_pIUnknowOuter;
public:
	// constructor method declarations
	  CNegative(IUnknown*);
    // destructtor method declarations
	 ~CNegative(void);

	 //INoAggregationIUnknown specific method declartions (inherited)
	 HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**);
	 ULONG __stdcall AddRef_NoAggregation(void);
	 ULONG __stdcall Release_NoAggregation(void);

	 // IUnknown specific method declartions (inherited)
	 HRESULT __stdcall QueryInterface(REFIID, void**);
	 ULONG __stdcall AddRef(void);
	 ULONG __stdcall Release(void);

	 // INegative specific method declarations (inherited)
	 HRESULT __stdcall ApplyNegative(COLORREF, COLORREF*);
};

class CNegativeClassFactory : public IClassFactory //this is class factory
{
private:
	long m_cRef;
	
public:
	// constructor method declarations
	CNegativeClassFactory(void);

	// destructor method declarations
	~CNegativeClassFactory(void);

	// IUnknown specific method declartions (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific method declarations (inherited)
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

//global variable declarations
long glNumberOfActiveComponents = 0; //Number of active components
long glNumberOfServerLocks      = 0; //Number of locks on this dll

// DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)//hDll == adress retured by LoadLibrary(LPSCTSTR)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return(TRUE);
}

//	Implementation Of CNegative's Constructor Method
CNegative::CNegative(IUnknown *pUnkOuter)
{
	// hardcoded initialization to anticipate possible failre of QueryInterface()
	m_cRef = 1;
	
	InterlockedIncrement(&glNumberOfActiveComponents);// Increment global counter

	if (pUnkOuter != NULL)
		m_pIUnknowOuter = pUnkOuter;//inner kade outer cha IUnknown ala
	else
		m_pIUnknowOuter = reinterpret_cast<IUnknown*> (static_cast<INoAggregationIUnknown*>(this));
}
 
//	Implementation Of CNegative's Destructor Method
CNegative::~CNegative(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);// Decrement global counter
}

//	Implementation Of CNegative's IUnknown's Methods
HRESULT CNegative::QueryInterface_NoAggregation(REFIID riid, void** ppv)//co-class//ppv == m_pIUnknownInner   //method coloring starts here
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<INoAggregationIUnknown *>(this);
	else if (riid == IID_INegative)
		*ppv = static_cast<INegative *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*> (*ppv)->AddRef();//ha call AddRef_NoAggregation la jatoy in first flow 

	return(S_OK);
}

ULONG CNegative::AddRef_NoAggregation(void)//co-class
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CNegative::Release_NoAggregation(void)//co-class
{
	InterlockedDecrement(&m_cRef);
	
	if (m_cRef == 0)
	{
		delete(this);//no object remaining so return 0
		return(0);
	}

	return(m_cRef);
}

HRESULT CNegative::QueryInterface(REFIID riid, void** ppv)
{
	return (m_pIUnknowOuter->QueryInterface(riid, ppv));
}

ULONG CNegative::AddRef(void)
{
	return (m_pIUnknowOuter->AddRef());
}

ULONG CNegative::Release(void)
{
	return (m_pIUnknowOuter->Release());
}

// Implementation Of INegative's Methods
HRESULT CNegative::ApplyNegative(COLORREF originalPixelColor, COLORREF* pNegative)
{
	// Variable Declarations
    unsigned int originalR = GetRValue(originalPixelColor);
	unsigned int originalG = GetGValue(originalPixelColor);
	unsigned int originalB = GetBValue(originalPixelColor);
	unsigned int negativeR = 255 - originalR;

			//clamping
	negativeR = (negativeR < 0) ? 0 : negativeR;
			
	unsigned int negativeG = 255 - originalG;

	//clamping
	negativeG = (negativeG < 0) ? 0 : negativeG;

	unsigned int negativeB = 255 - originalB;

	//clamping
	negativeB = (negativeB < 0) ? 0 : negativeB;

	*pNegative = RGB(negativeR, negativeG, negativeB);//same color R,G,B la	

	return(S_OK);
}

//Implementation of CNegativeClassFactory's Constructor Method
CNegativeClassFactory::CNegativeClassFactory(void)
{
	m_cRef = 1; // Hard Coded initialization to anticipate possible failure of QueryInterface()  
}

// Implementation of CNegativeClassFactory's Destructor Method
CNegativeClassFactory::~CNegativeClassFactory(void){}//no code 

// Implementation Of CSumSubtractClassFactory's IClassFactory's IUnknown's Methods
HRESULT CNegativeClassFactory::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory *>(this);//CSumSubtractClassFactory
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();//tu konihi asa tuzyatlya IUnknown madhe typecast ho, why?
	return(S_OK);
}

ULONG CNegativeClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CNegativeClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete(this);//no object remaining so return 0
		return(0);
	}
	return(m_cRef);
}
	
// Implementation Of CNegativeClassFactory's IClassFactory's Methods
HRESULT CNegativeClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)//Gives Implemented Co-Class 
{
	//variable declarations
	CNegative *pCnegative = NULL;
	HRESULT hr;

	//code
	if ((pUnkOuter != NULL) && (riid != IID_IUnknown))//pUnkOuter is not null now 
		return(CLASS_E_NOAGGREGATION);
	
	pCnegative = new CNegative(pUnkOuter);//co-class constructor

	if (pCnegative == NULL)
		return(E_OUTOFMEMORY);

	hr = pCnegative->QueryInterface_NoAggregation(riid, ppv);

	pCnegative->Release_NoAggregation();
	return(hr);
}

HRESULT CNegativeClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);

	return(S_OK);
}

// Implementation Of Exported Functions From This Dll
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)//This gives implemented ClassFactory
{
	//variable declarations
	CNegativeClassFactory* pCnegativeClassFactory = NULL;
	HRESULT hr;

	//code
	if (rclsid != CLSID_Negative)
		return(CLASS_E_CLASSNOTAVAILABLE);
	
	pCnegativeClassFactory = new CNegativeClassFactory;

	if (pCnegativeClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pCnegativeClassFactory->QueryInterface(riid, ppv);

	pCnegativeClassFactory->Release();

	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}
