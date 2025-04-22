//Changes in AggregationOuter are with respect to ContainmentOuter
#include<windows.h>
#include"PixEditOneEffectImpl.h"
#include"PixEditTwoEffectsImpl.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//Outer co-class aggegation will not inherit from interfaces of inner
//class declaration
class CDesaturationSepia : public IDestauration, ISepia //this is co-class
{
private:
	long m_cRef;
	IUnknown* m_pIUnknownInner;
public:
	// constructor method declarations
	 CDesaturationSepia(void);
    // destructtor method declarations
	 ~CDesaturationSepia(void);

	 // IUnknown specific method declartions (inherited)
	 HRESULT __stdcall QueryInterface(REFIID, void**);
	 ULONG __stdcall AddRef(void);
	 ULONG __stdcall Release(void);
	 
	 // IDestauration specific method declarations (inherited)
	 HRESULT __stdcall ApplyDestauration(COLORREF , COLORREF*);
		
	 // ISepia specific method declarations (inherited)
	 HRESULT __stdcall ApplySepia(COLORREF , COLORREF*);
  
	 //custom method for inner component creation
	 HRESULT __stdcall InitializeInnerComponent(void);
};

class CDesaturationSepiaClassFactory : public IClassFactory //this is class factory
{
private:
	long m_cRef;
public:
	// constructor method declarations
	CDesaturationSepiaClassFactory(void);

	// destructor method declarations
	~CDesaturationSepiaClassFactory(void);

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

//	Implementation Of CDesaturationSepia's Constructor Method
CDesaturationSepia::CDesaturationSepia(void)
{
	// hardcoded initialization to anticipate possible failre of QueryInterface()
	m_pIUnknownInner = NULL;
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);// Increment global counter
}
 
//	Implementation Of CDesaturationSepia's Destructor Method
CDesaturationSepia::~CDesaturationSepia(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);// Decrement global counter
	
	if (m_pIUnknownInner)
	{
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
	}
}

//	Implementation Of CDesaturationSepia's IUnknown's Methods
HRESULT CDesaturationSepia::QueryInterface(REFIID riid, void** ppv)//co-class
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IDestauration*>(this);
	else if (riid == IID_IDestauration)
		*ppv = static_cast<IDestauration*>(this);
	else if (riid == IID_ISepia)
		*ppv = static_cast<ISepia*>(this);
	else if (riid == IID_INegative)
		return(m_pIUnknownInner->QueryInterface(riid, ppv));
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*> (*ppv)->AddRef();

	return(S_OK);
}

ULONG CDesaturationSepia::AddRef(void)//co-class
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CDesaturationSepia::Release(void)//co-class
{
	InterlockedDecrement(&m_cRef);
	
	if (m_cRef == 0)
	{
		delete(this);//no object remaining so return 0
		return(0);
	}

	return(m_cRef);
}

// Implementation Of IDestauration's Methods
HRESULT CDesaturationSepia::ApplyDestauration(COLORREF originalPixelColor, COLORREF * pDesaturation)
{
	// Variable Declarations 
    unsigned int originalR = GetRValue(originalPixelColor);
	unsigned int originalG = GetGValue(originalPixelColor);
	unsigned int originalB = GetBValue(originalPixelColor);

	unsigned int desaturatedR = (unsigned int)((float)originalR * 0.3f);
	unsigned int desaturatedG = (unsigned int)((float)originalG * 0.59f);
	unsigned int desaturatedB = (unsigned int)((float)originalB * 0.11f);

	unsigned int finalDesaturatedColor = desaturatedR + desaturatedG + desaturatedB;

	*pDesaturation = RGB(finalDesaturatedColor, finalDesaturatedColor, finalDesaturatedColor);
	
	return(S_OK);
}

// Implementation Of ISepia's Methods
HRESULT CDesaturationSepia::ApplySepia(COLORREF originalPixelColor, COLORREF * pSepia)
{
	// Variable Declarations 
	unsigned int originalR = GetRValue(originalPixelColor);
	unsigned int originalG = GetGValue(originalPixelColor);
	unsigned int originalB = GetBValue(originalPixelColor);

	unsigned int sepiaR = (unsigned int)(((float)originalR * 0.393f) + ((float)originalG * 0.769f) + ((float)originalB * 0.189f));

	//clamping
	if (sepiaR > 255)
	{
		sepiaR = 255;
	}
	unsigned int sepiaG = (unsigned int)(((float)originalR * 0.349f) + ((float)originalG * 0.686f) + ((float)originalB * 0.168f));
			
	sepiaG = (sepiaG > 255) ? 255 : sepiaG;
	unsigned int sepiaB = (unsigned int)(((float)originalR * 0.272f) + ((float)originalG * 0.534f) + ((float)originalB * 0.131f));

	sepiaB = (sepiaB > 255) ? 255 : sepiaB;

	*pSepia = RGB(sepiaR, sepiaG, sepiaB);//same color R,G,B la
	
	return(S_OK);
}

HRESULT CDesaturationSepia::InitializeInnerComponent(void)
{
	//variable declration
	HRESULT hr = S_OK;
    
	// Code
	hr = CoCreateInstance(CLSID_Negative, reinterpret_cast<IUnknown * >(this), CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pIUnknownInner);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("IUnknown Interface Cannot be Obtained From Inner Component"), TEXT("ERROR"), MB_ICONERROR | MB_OK);
		return(E_FAIL);
	}

	return(S_OK);
}

//Implementation of CDesaturationSepiaClassFactory's Constructor Method
CDesaturationSepiaClassFactory::CDesaturationSepiaClassFactory(void)
{
	m_cRef = 1; // Hard Coded initialization to anticipate possible failure of QueryInterface()  
}

// Implementation of CDesaturationSepiaClassFactory's Destructor Method
CDesaturationSepiaClassFactory::~CDesaturationSepiaClassFactory(void)
{}//no code 

// Implementation Of CDesaturationSepiaClassFactory's IClassFactory's IUnknown's Methods
HRESULT CDesaturationSepiaClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CDesaturationSepiaClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CDesaturationSepiaClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete(this);//no object remaining so return 0
		return(0);
	}
	return(m_cRef);
}
	
// Implementation Of CDesaturationSepiaClassFactory's IClassFactory's Methods
HRESULT CDesaturationSepiaClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)//Gives Implemented Co-Class 
{
	//variable declarations
	CDesaturationSepia *pCDesaurationSepia = NULL;
	HRESULT hr;

	//code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);
	
	// create the instance of component i.e. of CDesaturationSepia class
	pCDesaurationSepia = new CDesaturationSepia;//co-class constructor

	if (pCDesaurationSepia == NULL)
		return(E_OUTOFMEMORY);

	//Initialize the inner component
	hr = pCDesaurationSepia->InitializeInnerComponent();

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed To Initialize Inner Component"), TEXT("ERROR"), MB_ICONERROR | MB_OK);
		pCDesaurationSepia->Release();
		return(E_FAIL);
	}

	// get the requested interface
	hr = pCDesaurationSepia->QueryInterface(riid, ppv);

	//anticipate possible failure of QueryInterface
	pCDesaurationSepia->Release();
	return(hr);
}

HRESULT CDesaturationSepiaClassFactory::LockServer(BOOL fLock)
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
	CDesaturationSepiaClassFactory* pCDesaurationSepiaClassFactory = NULL;
	HRESULT hr;

	//code
	if (rclsid != CLSID_DesaturationSepia)
		return(CLASS_E_CLASSNOTAVAILABLE);
	
	pCDesaurationSepiaClassFactory = new CDesaturationSepiaClassFactory;

	if (pCDesaurationSepiaClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pCDesaurationSepiaClassFactory->QueryInterface(riid, ppv);

	//anticipate possible failure of QueryInterface
	pCDesaurationSepiaClassFactory->Release();

	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	else
		return(S_FALSE);
}
