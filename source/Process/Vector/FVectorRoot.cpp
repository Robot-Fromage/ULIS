#include <ULIS>
#include <blend2d.h>

ULIS_NAMESPACE_BEGIN

FVectorRoot::~FVectorRoot()
{
}

FVectorRoot::FVectorRoot()
    : FVectorObject()
{
}

void
FVectorRoot::Select(BLContext& iBLContext,double x,double y)
{
    mSelectedObjectList.clear();

    RecursiveSelect( iBLContext, *this, x, y );
}

FVectorObject*
FVectorRoot::GetLastSelected()
{
    if ( mSelectedObjectList.empty() == true )
    {
        return nullptr;
    }

    return mSelectedObjectList.back();
}

void
FVectorRoot::RecursiveSelect( BLContext& iBLContext, FVectorObject& iChild, double x, double y )
{
    for( std::list<FVectorObject*>::iterator it = iChild.GetChildrenList().begin(); it != iChild.GetChildrenList().end(); ++it )
    {
        FVectorObject *obj = (*it);

        if( obj->Pick( iBLContext, x, y ) )
        {
            mSelectedObjectList.push_back( obj );
        }

        RecursiveSelect( iBLContext, *obj, x, y );
    }
}

ULIS_NAMESPACE_END
