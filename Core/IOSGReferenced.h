#pragma once
#include "EarthMatrixExports.h"
#include "x3py/module/plugininc.h"
#include "x3py/objptr.h"
#include "osg/Referenced"
#include "osg/ref_ptr"

BEGIN_NAMESPACE_EARTHMATRIX
const char* const clsidOSGReferenced = "027DCBD6-CF4B-46E1-8F6B-127A3E19424D";
class IOSGReferenced : public x3::IObject
{
	X3DEFINE_IID(IOSGReferenced);

public:
	virtual osg::Referenced* value() = 0;
	virtual void value(osg::Referenced* v) = 0;
};

template<class T>
T* fromOSGReferencend(x3::IObject* pObj)
{
	x3::Object<IOSGReferenced> spOSGReferenced = pObj;
	if( !spOSGReferenced.valid() )
		return nullptr;

	return dynamic_cast<T*>(spOSGReferenced->value());
}

inline x3::IObject* toOSGReferencend(osg::Referenced* val)
{
	if( val==nullptr )
		return nullptr;

	x3::Object<IOSGReferenced> spOSGReferenced(clsidOSGReferenced);
	if( !spOSGReferenced.valid() )
		return nullptr;

	spOSGReferenced->value(val);
	spOSGReferenced->retainObject();
	return spOSGReferenced.p();
}
END_NAMESPACE_EARTHMATRIX