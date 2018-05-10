#pragma once
#include <string>
#include <vector>
#include "EarthMatrixExports.h"
#include "UIPluginsEvents.h"
#include "x3py/module/plugininc.h"
#include "x3py/objptr.h"
#include "x3py/utilfunc/lockrw.h"

BEGIN_NAMESPACE_EARTHMATRIX
const char* const clsidUICore = "506707AE-9827-439A-ADF9-9CDD86262603";
const char* const clsidLanguages = "A87F1E31-085C-4FE1-8F46-2295BFF5ED21";
class IUICore : public x3::IObject
{
	X3DEFINE_IID(IUICore);

public:
	virtual long size() = 0;
	virtual x3::IObject* Find(const char* key) = 0;
	virtual bool Add(const char* key, x3::IObject* obj) = 0;
	virtual bool Remove(const char* key) = 0;
	virtual bool Remove(x3::IObject* obj) = 0;
	virtual void clear() = 0;
};

class IUICommon : public x3::IObject
{
	X3DEFINE_IID(IUICommon);

public:
	IUICommon() { m_spBuddy = NULL; }

	const wchar_t* Name() { return m_strName.c_str(); }
	void Name(const wchar_t* val) { m_strName = val; }

	const wchar_t* Caption() { return m_strCaption.c_str(); }
	void Caption(const wchar_t* val) { m_strCaption = val; }

	const wchar_t* Category() { return m_strCategory.c_str(); }
	void Category(const wchar_t* val) { m_strCategory = val; }

	const wchar_t* Tooltip() { return m_strTooltip.c_str(); }
	void Tooltip(const wchar_t* val) { m_strTooltip = val; }

	const wchar_t* Description() { return m_strDescription.c_str(); }
	void Description(const wchar_t* val) { m_strDescription = val; }

	const wchar_t* BitmapName() { return m_strBitmapName.c_str(); }
	void BitmapName(const wchar_t* val) { m_strBitmapName = val; }

	virtual bool Initialize() = 0;
	virtual bool UnInitialize() = 0;
	virtual bool SetBuddy(x3::IObject* val) = 0;// { m_spBuddy = val; return true; }

protected:
	std::wstring m_strName;
	std::wstring m_strCaption;
	std::wstring m_strCategory;
	std::wstring m_strTooltip;
	std::wstring m_strDescription;
	std::wstring m_strBitmapName;
	x3::AnyObject m_spBuddy;
};

class IUICommand : public x3::IObject
{
	X3DEFINE_IID(IUICommand);

public:
	IUICommand() { m_bChecked = false; m_bEnable = true; }	

	virtual bool Checked() { return m_bChecked; }
	virtual void Checked(bool val) 
	{ 
		if( m_bEnable ) 
			m_bChecked = val; 
	}

	virtual bool Enable() { return m_bEnable; }
	virtual void Enable(bool val) { m_bEnable = val; }

	virtual bool OnClick() 
	{ 
		if( !m_bEnable ) 
			return false; 

		return true;
	}

protected:
	bool m_bChecked;
	bool m_bEnable;
};

class IUITool : public x3::IObject
{
	X3DEFINE_IID(IUITool);

public:	
	virtual bool Activate() { return false; }
	virtual bool Deactivate() { return false; }
};

class IUIWnd : public x3::IObject
{
	X3DEFINE_IID(IUIWnd);

public:
	virtual bool createWnd(size_t hParent, long id) = 0;
	virtual size_t getHandle() = 0;
	virtual bool setParent(size_t hWnd) = 0;	
	virtual DWORD getStyle() { return WS_CHILD | WS_VISIBLE; }	
};

class IUIViewer3D : public x3::IObject
{
	X3DEFINE_IID(IUIViewer3D);
};

class IUILanguages : public x3::IObject
{
	X3DEFINE_IID(IUILanguages);

public:
	virtual int size() = 0;
	virtual const wchar_t* getName(int index) = 0;
	virtual bool select(int index) = 0;
	virtual const wchar_t* GetString(const wchar_t* app, const wchar_t* stringId, const wchar_t* defualt = NULL) = 0;
	virtual const wchar_t* GetString(const wchar_t* app, const int id, const wchar_t* defualt = NULL) = 0;
	virtual bool LocalizeDialog(size_t hwnd, const wchar_t* app, const wchar_t *stringId) = 0;
	virtual bool LocalizeDialog(size_t hwnd, const wchar_t* app, const int id) = 0;
};

END_NAMESPACE_EARTHMATRIX
