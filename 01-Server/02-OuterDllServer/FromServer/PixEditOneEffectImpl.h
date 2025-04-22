#pragma once
//Inner
class INegative : public IUnknown
{
public:
	// INegative specific method declarations
	virtual HRESULT __stdcall ApplyNegative(COLORREF, COLORREF*) = 0;	//pure virtual
};

// CLSID of Negative Component {CD79D980-0347-4FFD-AC0A-8E6CFF782702} --> this is stringised GUID
const CLSID CLSID_Negative = {0xcd79d980, 0x347, 0x4ffd, 0xac, 0xa, 0x8e, 0x6c, 0xff, 0x78, 0x27, 0x2};//Numeric Form of GUID

//IID of INegative Interface {99B96F39-6985-4132-B849-2C4669A0F7B6}
const IID IID_INegative = {0x99b96f39, 0x6985, 0x4132, 0xb8, 0x49, 0x2c, 0x46, 0x69, 0xa0, 0xf7, 0xb6};
