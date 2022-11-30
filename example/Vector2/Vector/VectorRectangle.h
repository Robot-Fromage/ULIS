#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"

class FVectorRectangle : public FVectorObject
{
    private:
        void DrawShape( FRectD &iRoi, uint64 iFlags );
        FVectorObject* PickShape( double iX, double iY, double iRadius );
        FVectorObject* CopyShape();
        void UpdateShape() {};

    protected :
        double mWidth;
        double mHeight; 

    public:
        ~FVectorRectangle();
        FVectorRectangle( std::string iName );
        FVectorRectangle( std::string iName, double iWidth, double iHeight );
        void SetSize( double iWidth, double iHeight );
        double GetWidth();
        double GetHeight();

};
