#include "UIPluginsEvents.h"
#include "x3py/observer/observerobject.h"

BEGIN_NAMESPACE_EARTHMATRIX

//////////////////////////////////////////////////////////////////////////
// IUIMouseResponse
IUIMouseResponse::IUIMouseResponse()
{
	X3_REGISTER_OBSERVER_OBJECT(EventMouseDown, &IUIMouseResponse::OnMouseDown);
	X3_REGISTER_OBSERVER_OBJECT(EventMouseMove, &IUIMouseResponse::OnMouseMove);
	X3_REGISTER_OBSERVER_OBJECT(EventMouseUp, &IUIMouseResponse::OnMouseUp);
	X3_REGISTER_OBSERVER_OBJECT(EventButtonDblClk, &IUIMouseResponse::OnButtonDblClk);
	X3_REGISTER_OBSERVER_OBJECT(EventMouseWheel, &IUIMouseResponse::OnMouseWheel);
	X3_REGISTER_OBSERVER_OBJECT(EventMouseHover, &IUIMouseResponse::OnMouseHover);
	X3_REGISTER_OBSERVER_OBJECT(EventMouseLeave, &IUIMouseResponse::OnMouseLeave);
	X3_REGISTER_OBSERVER_OBJECT(EventContextMenu, &IUIMouseResponse::OnContextMenu);
}

IUIMouseResponse::~IUIMouseResponse()
{
	x3::unregisterObserver(this);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// IUIKeyResponse
IUIKeyResponse::IUIKeyResponse()
{
	X3_REGISTER_OBSERVER_OBJECT(EventKeyDown, &IUIKeyResponse::OnKeyDown);
	X3_REGISTER_OBSERVER_OBJECT(EventKeyUp, &IUIKeyResponse::OnKeyUp);
}

IUIKeyResponse::~IUIKeyResponse()
{
	x3::unregisterObserver(this);
}
//////////////////////////////////////////////////////////////////////////

END_NAMESPACE_EARTHMATRIX
