#ifndef _FVECTORROOT_H_
#define _FVECTORROOT_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include <Vector/FVectorObject.h>

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorRoot : public FVectorObject
{
    private:
        void RecursiveSelect( BLContext& iBLContext, FVectorObject& iChild, double x, double y );

    protected:
        std::list<FVectorObject*> mSelectedObjectList;

    public:
        ~FVectorRoot();
        FVectorRoot();
        void Select( BLContext& iBLContext, double x, double y );
        FVectorObject* GetLastSelected();
};

ULIS_NAMESPACE_END

#endif // _FVECTORROOT_H_