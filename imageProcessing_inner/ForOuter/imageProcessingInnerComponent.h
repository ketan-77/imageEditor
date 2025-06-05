#pragma once
#include <string>

class IDesaturation : public IUnknown
{
public:
    virtual HRESULT __stdcall destauration(COLORREF, COLORREF&) = 0;
};

class ISepia : public IUnknown
{
public:
    virtual HRESULT __stdcall sepia(COLORREF, COLORREF&) = 0;
};

class IInversion : public IUnknown
{
public:
    virtual HRESULT __stdcall inversion(COLORREF, COLORREF&) = 0;
};

//CLSID of image procesing {6AB99B8D-F77E-4F2B-ACA1-8EEFBD9C5B8F}
const CLSID CLSID_IMGPROCESSING = {0x6ab99b8d, 0xf77e, 0x4f2b, 0xac, 0xa1, 0x8e, 0xef, 0xbd, 0x9c, 0x5b, 0x8f};

// IID for IID_IDesaturation {CED1ED67-C536-4719-AFAE-589E13A786B1}
const IID IID_IDesaturation = { 0xced1ed67, 0xc536, 0x4719, 0xaf, 0xae, 0x58, 0x9e, 0x13, 0xa7, 0x86, 0xb1 };

// IID for IID_ISepia {9609F9C9-209C-469C-A42F-1BA43AE57F19}
const IID IID_ISepia = { 0x9609f9c9, 0x209c, 0x469c, 0xa4, 0x2f, 0x1b, 0xa4, 0x3a, 0xe5, 0x7f, 0x19 };

// IID for IID_IInversion {AE927B41-59D1-45B9-809B-7BF5DEF35F13}
const IID IID_IInversion = { 0xae927b41, 0x59d1, 0x45b9, 0x80, 0x9b, 0x7b, 0xf5, 0xde, 0xf3, 0x5f, 0x13 };
