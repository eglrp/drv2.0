#pragma once
#include "CbersExports.h"
#include "x3py/module/plugininc.h"
#include "x3py/objptr.h"

BEGIN_NAMESPACE_Cbers3D

class ICbersView3D : public x3::IObject
{
	X3DEFINE_IID(ICbersView3D);

public:
	virtual x3::IObject* getViewer() = 0;
	virtual bool openFile(const std::string& file) { return false; }
};

END_NAMESPACE_Cbers3D