#pragma once
#include "EarthMatrixExports.h"
#include "x3py/iobject.h"
#include "x3py/module/plugininc.h"
#include "x3py/observer/fireeventex.h"
#include "x3py/observer/fireobjeventex.h"
#include <string>
#include<algorithm>
#include <map>

BEGIN_NAMESPACE_EARTHMATRIX

#ifndef EVENT_NAMESPACE
#define EVENT_NAMESPACE "EarthMatrixEvent"
#endif

	X3DEFINE_OBJEVENT_4Break(EventMouseDown, long, long, long, long, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_4Break(EventMouseMove, long, long, long, long, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_4Break(EventMouseUp, long, long, long, long, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_4Break(EventButtonDblClk, long, long, long, long, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_5Break(EventMouseWheel, long, long, short, long, long, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_4Break(EventMouseHover, long, long, long, long, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_0Break(EventMouseLeave, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_3Break(EventContextMenu, HWND, long, long, EVENT_NAMESPACE);

class IUIMouseResponse : x3::ObserverObject
{	
public:
	IUIMouseResponse();
	virtual ~IUIMouseResponse();

public:
	virtual bool OnMouseDown(long button, long shift, long x, long y) { return true; }
	virtual bool OnMouseMove(long button, long shift, long x, long y) { return true; }
	virtual bool OnMouseUp(long button, long shift, long x, long y)   { return true; }
	virtual bool OnButtonDblClk(long button, long shift, long x, long y) { return true; }
	virtual bool OnMouseWheel(long button, long shift, short zDelta, long x, long y) { return true; }
	virtual bool OnMouseHover(long button, long shift, long x, long y) { return true; }
	virtual bool OnMouseLeave() { return true; }
	virtual bool OnContextMenu(HWND hWnd,long x, long y) { return true; }
};

X3DEFINE_OBJEVENT_2Break(EventKeyDown, long, long, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_2Break(EventKeyUp, long, long, EVENT_NAMESPACE);

class IUIKeyResponse : public x3::ObserverObject
{
public:
	IUIKeyResponse();
	virtual ~IUIKeyResponse();

public:
	virtual bool OnKeyDown(long keyCode, long shift) { return false; }
	virtual bool OnKeyUp(long keyCode, long shift) { return false; }
};


X3DEFINE_OBJEVENT_3Break(EventAnything, x3::IObject*, const std::string&, x3::IObject*, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_3Break(EventAnything2, x3::IObject*, const std::string&, const std::string&, EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_4Break(EventAnything3, x3::IObject*, const std::string&, long, long, EVENT_NAMESPACE);

class IAnythingEventObserver : public x3::ObserverObject
{
public:
	IAnythingEventObserver() 
	{
		m_bRegister = false;
	}

	virtual ~IAnythingEventObserver()
	{
		unregisterHandlers();
	}

public:
	// Event
	virtual void registerHandlers()
	{
		if( m_bRegister )
			return;

		X3_REGISTER_OBSERVER_OBJECT(EventAnything, &IAnythingEventObserver::_OnAnything);
		X3_REGISTER_OBSERVER_OBJECT(EventAnything2, &IAnythingEventObserver::_OnAnything2);
		X3_REGISTER_OBSERVER_OBJECT(EventAnything3, &IAnythingEventObserver::_OnAnything3);
		m_bRegister = true;
	}

	virtual void unregisterHandlers()
	{
		if( !m_bRegister )
			return;

		x3::unregisterObserver(this);
	}

	virtual void supportsEvent(const std::string& eventKey)
	{
		if( !eventKey.empty() )
		{
			std::string lowerKey = eventKey;
			std::transform(eventKey.begin(), eventKey.end(), lowerKey.begin(), tolower);
			m_supportedEvents.insert(std::map<std::string, std::string>::value_type(lowerKey, lowerKey));
		}
	}

	virtual bool acceptsEvent(const std::string& eventKey)
	{
		if( eventKey.empty() )
			return false;

		if( m_supportedEvents.size()<=0 )
			return true;

		std::string lowerKey = eventKey;
		std::transform(eventKey.begin(), eventKey.end(), lowerKey.begin(), tolower);
		return m_supportedEvents.find(lowerKey)!=m_supportedEvents.end();		
	}

	// Node Event
	virtual bool OnAnything(x3::IObject* sender, const std::string& eventKey, x3::IObject* param) { return true; }
	virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& param) { return true; }
	virtual bool OnAnything3(x3::IObject* sender, const std::string& eventKey, long wParam, long lParam) { return true; }

protected:
	bool _OnAnything(x3::IObject* sender, const std::string& eventKey, x3::IObject* param)
	{
		if( acceptsEvent(eventKey) )
			OnAnything(sender, eventKey, param);

		return true;
	}

	bool _OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& param)
	{
		if( acceptsEvent(eventKey) )
			OnAnything2(sender, eventKey, param);

		return true;
	}

	bool _OnAnything3(x3::IObject* sender, const std::string& eventKey, long wParam, long lParam)
	{
		if( acceptsEvent(eventKey) )
			OnAnything3(sender, eventKey, wParam, lParam);

		return true;
	}

protected:
	bool m_bRegister;
	std::map<std::string, std::string> m_supportedEvents;
};

END_NAMESPACE_EARTHMATRIX

