#pragma once

class IDestauration : public IUnknown
{
public:
	// IDestauration specific method declarations
	virtual HRESULT __stdcall ApplyDestauration(COLORREF, COLORREF*) = 0;	//pure virtual
};

class ISepia : public IUnknown
{
public:
	// ISepia specific method declarations
	virtual HRESULT __stdcall ApplySepia(COLORREF, COLORREF*) = 0;	//pure virtual 
};

class INegative : public IUnknown
{
public:
	// INegative specific method declarations
	virtual HRESULT __stdcall ApplyNegative(COLORREF, COLORREF*) = 0;	//pure virtual
};

// CLSID of DesaturationSepia Component {7AD35FE0-BB59-4773-9BDB-75C47AFF32DC} --> this is stringised GUID
const CLSID CLSID_DesaturationSepia = { 0x7ad35fe0, 0xbb59, 0x4773, 0x9b, 0xdb, 0x75, 0xc4, 0x7a, 0xff, 0x32, 0xdc };//Numeric Form of GUID

//IID of IDestauration Interface {0312B3D7-E187-4D35-A81F-321A271AA41E}
const IID IID_IDestauration = { 0x312b3d7, 0xe187, 0x4d35, 0xa8, 0x1f, 0x32, 0x1a, 0x27, 0x1a, 0xa4, 0x1e };

//IID of ISepia Interface {2F276B4A-AFE5-4497-AB0A-AD3A8FE920C0}
const IID IID_ISepia = { 0x2f276b4a, 0xafe5, 0x4497, 0xab, 0xa, 0xad, 0x3a, 0x8f, 0xe9, 0x20, 0xc0 };

//IID of INegative Interface {99B96F39-6985-4132-B849-2C4669A0F7B6}
const IID IID_INegative = { 0x99b96f39, 0x6985, 0x4132, 0xb8, 0x49, 0x2c, 0x46, 0x69, 0xa0, 0xf7, 0xb6 };
