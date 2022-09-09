#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <chrono>
#include <blend2d.h>
using namespace ::ULIS;

class FVectorObject
{
    public:
        ~FVectorObject();
        FVectorObject();
        virtual void Draw(FBlock& iBlock,BLContext *blctx) = 0;
};

FVectorObject::~FVectorObject()
{
}

FVectorObject::FVectorObject()
{
}

class FVectorSegment;

class FVectorPoint
{
    private:

    protected:
        BLPoint mCore;

        std::vector<FVectorSegment*> mSegmentList;
    public:
        ~FVectorPoint();
        FVectorPoint(double iX,double iY);
        void AddSegment(FVectorSegment* iSegment);
        double GetX();
        double GetY();
        void SetX(double iX);
        void SetY(double iY);
        BLPoint* GetBLPoint();

};

FVectorPoint::~FVectorPoint()
{
}

FVectorPoint::FVectorPoint( double iX, double iY )
{
    mCore.x = iX;
    mCore.y = iY;
}

BLPoint* FVectorPoint::GetBLPoint()
{
    return &mCore;
}

double FVectorPoint::GetX()
{
    return mCore.x;
}

double FVectorPoint::GetY()
{
    return mCore.y;
}

void FVectorPoint::SetX( double iX )
{
    mCore.x = iX;
}

void FVectorPoint::SetY(double iY)
{
    mCore.y = iY;
}

void
FVectorPoint::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );
}

/******************************************************************************/

class FVectorSegment
{
protected:
    FVectorPoint *mPoint[2];

public:
    ~FVectorSegment();
    FVectorSegment();
    FVectorSegment(FVectorPoint* iPoint0,FVectorPoint* iPoint1);
    FVectorPoint* GetPoint( int iPointNum );
};

FVectorSegment::~FVectorSegment()
{
}

FVectorSegment::FVectorSegment()
{
}

FVectorSegment::FVectorSegment( FVectorPoint* iPoint0, FVectorPoint* iPoint1 )
{
    mPoint[0] = iPoint0;
    mPoint[1] = iPoint1;

    mPoint[0]->AddSegment( this );
    mPoint[1]->AddSegment( this );
}

FVectorPoint* FVectorSegment::GetPoint( int iPointNum )
{
    return mPoint[iPointNum];
}

/******************************************************************************/

class FVectorCubicSegment : public FVectorSegment
{
    private:
        FVectorPoint *mCtrlPoint[2];
    public:
        ~FVectorCubicSegment();
        FVectorCubicSegment(FVectorPoint* iPoint0,FVectorPoint* iPoint1);
        FVectorCubicSegment(FVectorPoint* iPoint0,FVectorPoint* iCtrlPoint0,FVectorPoint* iCtrlPoint1,FVectorPoint* iPoint1);
        FVectorPoint* GetControlPoint( int iCtrlPointNum );
};

FVectorCubicSegment::FVectorCubicSegment( FVectorPoint* iPoint0
                                        , FVectorPoint* iPoint1 )
   : FVectorSegment( iPoint0, iPoint1 )
{
    mCtrlPoint[0] = new FVectorPoint( iPoint0->GetX(), iPoint0->GetY() );
    mCtrlPoint[1] = new FVectorPoint( iPoint1->GetX(), iPoint1->GetY() );
}

FVectorPoint*
FVectorCubicSegment::GetControlPoint( int iCtrlPointNum )
{
    return mCtrlPoint[iCtrlPointNum];
}

FVectorCubicSegment::FVectorCubicSegment( FVectorPoint* iPoint0
                                        , FVectorPoint* iCtrlPoint0
                                        , FVectorPoint* iCtrlPoint1
                                        , FVectorPoint* iPoint1 )
    : FVectorSegment( iPoint0, iPoint1 )
{
    mCtrlPoint[0] = iCtrlPoint0;
    mCtrlPoint[1] = iCtrlPoint1;
}

/******************************************************************************/

class FVectorQuadraticSegment : public FVectorSegment
{
    private:
        FVectorPoint *mCtrlPoint;

    public:
        ~FVectorQuadraticSegment();
        FVectorQuadraticSegment();
        FVectorQuadraticSegment( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorQuadraticSegment( FVectorPoint* iPoint0, FVectorPoint* iCtrl, FVectorPoint* iPoint1 );
};

FVectorQuadraticSegment::~FVectorQuadraticSegment()
{
}

FVectorQuadraticSegment::FVectorQuadraticSegment( FVectorPoint* iPoint0
                                                , FVectorPoint* iCtrl
                                                , FVectorPoint* iPoint1 )
    : FVectorSegment( iPoint0, iPoint1 )
    ,mCtrlPoint( iCtrl )
{

}

FVectorQuadraticSegment::FVectorQuadraticSegment( FVectorPoint* iPoint0
                                                , FVectorPoint* iPoint1 )
    : FVectorSegment( iPoint0, iPoint1 )
{

}

/******************************************************************************/

class FVectorPath : public FVectorObject
{
    private:
        std::vector<FVectorPoint*> mPointList;
        std::vector<FVectorSegment*> mSegmentList;
        FVectorPoint* mLastPoint;

    protected :
            BLPath mPath;
            void AddSegment( FVectorSegment* iSegment );
  
    public:
        ~FVectorPath();
        FVectorPath();
        virtual FVectorSegment* AppendPoint(FVectorPoint* iPoint);
        /*virtual void InsertPoint( FVectorSegment* iSegment, FVectorPoint* iPoint );*/
        FVectorPoint *GetLastPoint();
        void SetLastPoint(FVectorPoint* iLastPoint);
};

FVectorPath::~FVectorPath()
{
}

FVectorPath::FVectorPath()
{
    mLastPoint = NULL;
}

FVectorSegment*
FVectorPath::AppendPoint( FVectorPoint* iPoint )
{
    mPointList.push_back( iPoint );
    mLastPoint = iPoint;

    return NULL;
}

void
FVectorPath::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );
}

FVectorPoint*
FVectorPath::GetLastPoint()
{
    return mLastPoint;
}

void
FVectorPath::SetLastPoint(FVectorPoint* iLastPoint)
{
    mLastPoint = iLastPoint;
}

/******************************************************************************/

class FVectorCubicPath : public FVectorPath
{
    public:
        FVectorCubicPath();
        FVectorSegment* AppendPoint( FVectorPoint* iPoint );
        FVectorSegment* AppendPoint(FVectorPoint* iCtrlPoint0,FVectorPoint* iCtrlPoint1,FVectorPoint* iPoint1);
        void Draw( FBlock& iBlock,BLContext *blctx);
};

FVectorCubicPath::FVectorCubicPath()
    : FVectorPath()
{

}

FVectorSegment*
FVectorCubicPath::AppendPoint(FVectorPoint* iPoint)
{
    FVectorPoint* lastPoint = GetLastPoint();

    FVectorPath::AppendPoint(iPoint);

    // lastPoint is NULL if this is the first point added
    if(lastPoint)
    {
        FVectorCubicSegment* segment = new FVectorCubicSegment( lastPoint, iPoint );


        AddSegment(segment);

        mPath.cubicTo(*segment->GetControlPoint(0)->GetBLPoint()
                     ,*segment->GetControlPoint(1)->GetBLPoint()
                     ,*segment->GetPoint(1)->GetBLPoint());

        return segment;
    } else {
        mPath.moveTo(*iPoint->GetBLPoint());
    }

    return NULL;
}

FVectorSegment*
FVectorCubicPath::AppendPoint(FVectorPoint* iCtrlPoint0,FVectorPoint* iCtrlPoint1,FVectorPoint* iPoint1)
{
    FVectorPoint* lastPoint = GetLastPoint();

    FVectorPath::AppendPoint(iPoint1);

    // lastPoint is NULL if this is the first point added
    if(lastPoint)
    {
        FVectorCubicSegment* segment = new FVectorCubicSegment(lastPoint,iPoint1);


        AddSegment(segment);

        mPath.cubicTo(*iCtrlPoint0->GetBLPoint()
                     ,*iCtrlPoint1->GetBLPoint()
                     ,*iPoint1->GetBLPoint());

        return segment;
    } else {
        mPath.moveTo(*iPoint1->GetBLPoint());
    }

    return NULL;
}

void
FVectorCubicPath::Draw(FBlock& iBlock, BLContext *blctx )
{
printf("test\n");
blctx->setCompOp(BL_COMP_OP_SRC_OVER);
blctx->setFillStyle(BLRgba32(0xFFFFFFFF));
blctx->setStrokeStyle(BLRgba32(0xFF000000));
blctx->fillPath(mPath);
blctx->strokePath(mPath);
blctx->end();

    printf("test0\n");
}


/******************************************************************************/

int
main(int argc, char* argv[])
{
    FThreadPool pool;
    FCommandQueue queue(pool);
    eFormat fmt = Format_RGBA8;
    FContext ctx(queue, fmt);


    int width = 640;
    int height = 480;

    FBlock canvas(width, height, Format_RGBA8);
    ctx.Clear(canvas);
    ctx.Finish();

    // BL_FORMAT_PRGB32: 32-bit premultiplied ARGB pixel format (8-bit components). 
    BLImage img(width,height,BL_FORMAT_PRGB32);
    {
        BLContext blctx(img);
        auto a = blctx.fillRule();
        BLStyle strokeStyle;
        BLStyle fillStyle;
        blctx.getStrokeStyle(strokeStyle);
        blctx.getStrokeStyle(fillStyle);

        auto dummy = 0;

        blctx.setFillStyle(BLRgba32(0xFF0100FF));
        blctx.setCompOp(BL_COMP_OP_SRC_COPY);
        blctx.fillAll();

        BLPath path;
/*
        path.moveTo(26,31);
        path.cubicTo(642,132,587,-136,25,464);
        path.cubicTo(882,404,144,267,27,31);
*/
/*
        blctx.setCompOp(BL_COMP_OP_SRC_OVER);
        blctx.setFillStyle(BLRgba32(0xFFFFFFFF));
        blctx.setStrokeStyle(BLRgba32(0xFF000000));
*/
/*
        blctx.fillPath(path);
*/

        FVectorPoint point0(26,31);
        FVectorPoint point1(25,464);
        FVectorPoint point2(27,31);

        FVectorPoint ctrl0(642,132);
        FVectorPoint ctrl1(587,136);

        FVectorCubicPath cubicPath;

        FVectorCubicSegment *segment;
        FVectorPoint* ctrlPoint0;
        FVectorPoint* ctrlPoint1;

        cubicPath.AppendPoint( &point0 );

        segment = static_cast<FVectorCubicSegment*>(cubicPath.AppendPoint(&ctrl0,&ctrl1, &point1));

        /*ctrlPoint0 = segment->GetControlPoint(0);
        ctrlPoint1 = segment->GetControlPoint(1);

        ctrlPoint0->SetX(642);
        ctrlPoint0->SetY(132);
        ctrlPoint1->SetX(587);
        ctrlPoint1->SetY(136);*/

        segment = static_cast<FVectorCubicSegment*>(cubicPath.AppendPoint(&point2));
        ctrlPoint0 = segment->GetControlPoint(0);
        ctrlPoint1 = segment->GetControlPoint(1);

        ctrlPoint0->SetX(882);
        ctrlPoint0->SetY(404);
        ctrlPoint1->SetX(144);
        ctrlPoint1->SetY(267);

        cubicPath.Draw(canvas, &blctx);
    }

    BLImageData data;
    BLResult err = img.getData(&data);

    QApplication app(argc,argv);
    QWidget* widget = new QWidget();
    //QImage* image = new QImage( canvas.Bits(), canvas.Width(), canvas.Height(), canvas.BytesPerScanLine(), QImage::Format_RGBA8888 );
    // Format_ARGB32: (0xAARRGGBB)
    // Format_ARGB32_Premultiplied: (0xAARRGGBB) prem, only compatible with bl2d
    // Format_RGB32: 32-bit RGB format (0xffRRGGBB)
    QImage* image = new QImage((uint8*)data.pixelData,data.size.w,data.size.h,data.stride,QImage::Format_ARGB32_Premultiplied);
    QPixmap pixmap = QPixmap::fromImage(*image);
    QLabel* label = new QLabel(widget);
    label->setPixmap(pixmap);
    widget->resize(pixmap.size());
    widget->show();

    int exit_code = app.exec();
    delete  label;
    delete  image;
    delete  widget;

    return  exit_code;
}
