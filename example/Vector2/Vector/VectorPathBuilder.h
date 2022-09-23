#ifndef _FVECTORPATHBUILDER_H_
#define _FVECTORPATHBUILDER_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorPath.h"

class FVectorPathBuilder : public FVectorPath
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );
        bool PickShape( BLContext& iBLContext, double iX, double iY ) { return false; };

    protected :
 
    public:
        ~FVectorPathBuilder();
         FVectorPathBuilder();
         FVectorSegment* AppendPoint(FVectorPoint* iPoint);
         void Pick(double iX,double iY,double iRadius);
         void Unselect(FVectorPoint* iPoint);
};

#endif // _FVECTORPATHBUILDER_H_