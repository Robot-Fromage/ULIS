#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <chrono>
#include <blend2d.h>
using namespace ::ULIS;

class FVectorObject
{
    public:
        ~FVectorObject();
        FVectorObject();
        virtual void Draw(FBlock& iBlock,BLContext& blctx) = 0;
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
        double mX;
        double mY;

        std::vector<FVectorSegment*> mSegmentList;
    public:
        ~FVectorPoint();
        FVectorPoint(double iX,double iY);
        void AddSegment(FVectorSegment* iSegment);
        double GetX();
        double GetY();
        void SetX(double iX);
        void SetY(double iY);

};

FVectorPoint::~FVectorPoint()
{
}

FVectorPoint::FVectorPoint( double iX, double iY )
{
    mX = iX;
    mY = iY;
}

double FVectorPoint::GetX()
{
    return mX;
}

double FVectorPoint::GetY()
{
    return mY;
}

void FVectorPoint::SetX( double iX )
{
    mX = iX;
}

void FVectorPoint::SetY(double iY)
{
    mY = iY;
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
        FVectorPoint* mLastPoint;

    protected :
        std::list<FVectorPoint*> mPointList;
        std::list<FVectorSegment*> mSegmentList;
        std::list<FVectorPoint*> mSelectedPointList;
        void AddSegment( FVectorSegment* iSegment );
  
    public:
        ~FVectorPath();
        FVectorPath();
        virtual FVectorSegment* AppendPoint(FVectorPoint* iPoint);
        /*virtual void InsertPoint( FVectorSegment* iSegment, FVectorPoint* iPoint );*/
        FVectorPoint *GetLastPoint();
        void SetLastPoint(FVectorPoint* iLastPoint);
        std::list<FVectorPoint*> GetSelectedPointList();
        virtual void Pick(double iX,double iY,double iRadius) = 0;
        virtual void Unselect(FVectorPoint* iPoint) = 0;
};

FVectorPath::~FVectorPath()
{
}

FVectorPath::FVectorPath()
{
    mLastPoint = NULL;
}

std::list<FVectorPoint*> 
FVectorPath::GetSelectedPointList()
{
    return mSelectedPointList;
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
        void Draw(FBlock& iBlock,BLContext& iBLContext);
        void Pick(double iX,double iY,double iRadius);
        void Unselect(FVectorPoint* iPoint);
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

        return segment;
    }

    return NULL;
}


void
FVectorCubicPath::Draw( FBlock& iBlock, BLContext& iBLContext )
{
    BLPath path;
    BLPath ctrlPath0;
    BLPath ctrlPath1;

    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorCubicSegment *segment = static_cast<FVectorCubicSegment*>(*it);
        BLPoint point0;
        BLPoint point1;
        BLPoint ctrlPoint0;
        BLPoint ctrlPoint1;

        point0.x = segment->GetPoint(0)->GetX();
        point0.y = segment->GetPoint(0)->GetY();

        ctrlPoint0.x = segment->GetControlPoint(0)->GetX();
        ctrlPoint0.y = segment->GetControlPoint(0)->GetY();

        ctrlPoint1.x = segment->GetControlPoint(1)->GetX();
        ctrlPoint1.y = segment->GetControlPoint(1)->GetY();

        point1.x = segment->GetPoint(1)->GetX();
        point1.y = segment->GetPoint(1)->GetY();

        iBLContext.setStrokeStyle(BLRgba32(0xFFFF0000));
        ctrlPath0.moveTo(point0.x,point0.y);
        ctrlPath0.lineTo(ctrlPoint0.x
                     ,ctrlPoint0.y );
        iBLContext.strokePath(ctrlPath0);

        iBLContext.setStrokeStyle(BLRgba32(0xFFFF0000));
        ctrlPath1.moveTo(point1.x,point1.y);
        ctrlPath1.lineTo(ctrlPoint1.x
                     ,ctrlPoint1.y);
        iBLContext.strokePath(ctrlPath1);

        iBLContext.setStrokeStyle(BLRgba32(0xFF000000));
        path.moveTo( point0.x, point0.y);
        path.cubicTo( ctrlPoint0.x
                    , ctrlPoint0.y
                    , ctrlPoint1.x
                    , ctrlPoint1.y
                    , point1.x
                    , point1.y );
        iBLContext.strokePath(path);
    }

    /*iBLContext->fillPath(mPath);*/
    /*iBLContext.strokePath(path);*/

}

void
FVectorCubicPath::Pick(double iX,double iY,double iRadius)
{
    for(std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it)
    {
        FVectorPoint* point = *it;

        if((fabs(point->GetX() - iX) <= iRadius) &&
           (fabs(point->GetY() - iY) <= iRadius))
        {
            mSelectedPointList.push_back(point);
        }
    }

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorCubicSegment* segment = static_cast<FVectorCubicSegment*>(*it);
        FVectorPoint* ctrlPoint0 = segment->GetControlPoint(0);
        FVectorPoint* ctrlPoint1 = segment->GetControlPoint(1);

        if((fabs(ctrlPoint0->GetX() - iX) <= iRadius) &&
           (fabs(ctrlPoint0->GetY() - iY) <= iRadius))
        {
            mSelectedPointList.push_back(ctrlPoint0);
        }

        if((fabs(ctrlPoint1->GetX() - iX) <= iRadius) &&
           (fabs(ctrlPoint1->GetY() - iY) <= iRadius))
        {
            mSelectedPointList.push_back(ctrlPoint1);
        }
    }
}

void
FVectorCubicPath::Unselect(FVectorPoint *iPoint)
{
    if ( iPoint == NULL )
    {
        mSelectedPointList.clear();
    }
    else
    {
        mSelectedPointList.remove(iPoint);
    }
}

/******************************************************************************/
class MyWidget: public QWidget
{
public:
    MyWidget::MyWidget(uint32 iWidth,uint32 iHeight);

    virtual ~MyWidget() {
        delete mQImage; mBLContext->end();
    }

    virtual void paintEvent( QPaintEvent* e )
    {
        QPainter p(this);

        mContext->Clear(*mCanvas);
        mContext->Finish();

        mBLContext->setFillStyle(BLRgba32(0xFFFFFFFF));
        mBLContext->setCompOp(BL_COMP_OP_SRC_COPY);
        mBLContext->fillAll();

        cubicPath->Draw(*mCanvas,*mBLContext);

        p.drawImage(rect(),*mQImage);
    }

    virtual void mousePressEvent(QMouseEvent* event)
    {
        cubicPath->Unselect(nullptr);

        cubicPath->Pick(event->x(),event->y(), 10.0f);

/*
        const uint32_t someColor = rand();
        const size_t frameBufferSizeWords = frameBufferSizeBytes/sizeof(uint32_t);
        uint32* fb32 = (uint32* ) frameBuffer;
        for(size_t i=0; i<frameBufferSizeWords; i++) fb32[i] = someColor;
*/
        update();
    }

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        if (event->buttons() == Qt::LeftButton )
        {
            std::list<FVectorPoint*> selectedPointList = cubicPath->GetSelectedPointList();

            for(std::list<FVectorPoint*>::iterator it = selectedPointList.begin(); it != selectedPointList.end(); ++it)
            {
                FVectorPoint *selectedPoint = *it;

                selectedPoint->SetX(event->x());
                selectedPoint->SetY(event->y());
            }

            update();
        }

    }

private:
    QImage* mQImage;
    FVectorCubicPath* cubicPath;
    BLImage* mBLImage;
    BLContext* mBLContext;
    FBlock* mCanvas;
    FThreadPool* mPool;
    FCommandQueue* mQueue;
    FContext* mContext;
};

MyWidget::MyWidget( uint32 iWidth, uint32 iHeight) {

    BLImageData data;
    eFormat fmt = Format_RGBA8;

    // ULIS part
    mPool = new FThreadPool();
    mQueue = new FCommandQueue(*mPool);
    mContext = new FContext(*mQueue,Format_RGBA8);
    mCanvas = new FBlock(iWidth,iHeight,fmt);

    // Blend2D part
    mBLImage = new BLImage(iWidth,iHeight,BL_FORMAT_PRGB32);
    mBLContext = new BLContext(*mBLImage);
    mBLImage->getData( &data );

    mQImage = new QImage( (uchar*)data.pixelData
                              ,data.size.w
                              ,data.size.h
                              ,data.stride
                              ,QImage::Format_ARGB32_Premultiplied);

    cubicPath = new FVectorCubicPath();

    FVectorPoint* point0 = new FVectorPoint(26,31);
    FVectorPoint* point1 = new FVectorPoint(25,464);
    FVectorPoint* point2 = new FVectorPoint(27,31);

    FVectorCubicSegment* segment;
    FVectorPoint* ctrlPoint0;
    FVectorPoint* ctrlPoint1;

    cubicPath->AppendPoint(point0);

    segment = static_cast<FVectorCubicSegment*>(cubicPath->AppendPoint(point1));

    ctrlPoint0 = segment->GetControlPoint(0);
    ctrlPoint1 = segment->GetControlPoint(1);

    ctrlPoint0->SetX(300);
    ctrlPoint0->SetY(132);
    ctrlPoint1->SetX(270);
    ctrlPoint1->SetY(136);

    segment = static_cast<FVectorCubicSegment*>(cubicPath->AppendPoint(point2));
    ctrlPoint0 = segment->GetControlPoint(0);
    ctrlPoint1 = segment->GetControlPoint(1);

    ctrlPoint0->SetX(560);
    ctrlPoint0->SetY(404);
    ctrlPoint1->SetX(144);
    ctrlPoint1->SetY(267);

}

int
main(int argc, char* argv[])
{
    /*FThreadPool pool;
    FCommandQueue queue(pool);
    eFormat fmt = Format_RGBA8;
    FContext ctx(queue, fmt);*/


    int width = 640;
    int height = 480;

    /*FBlock canvas(width, height, Format_RGBA8);
    ctx.Clear(canvas);
    ctx.Finish();*/

    // BL_FORMAT_PRGB32: 32-bit premultiplied ARGB pixel format (8-bit components). 
    /*BLImage img(width,height,BL_FORMAT_PRGB32);
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

        BLPath path;*/
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



        /*blctx.end();*/
    /*}*/

    /*BLImageData data;
    BLResult err = img.getData(&data);*/

    QApplication app(argc,argv);
    /*QWidget* widget = new QWidget();*/

    MyWidget* demoWidget = new MyWidget(width,height);

    //QImage* image = new QImage( canvas.Bits(), canvas.Width(), canvas.Height(), canvas.BytesPerScanLine(), QImage::Format_RGBA8888 );
    // Format_ARGB32: (0xAARRGGBB)
    // Format_ARGB32_Premultiplied: (0xAARRGGBB) prem, only compatible with bl2d
    // Format_RGB32: 32-bit RGB format (0xffRRGGBB)
    /*QImage* image = new QImage((uint8*)data.pixelData,data.size.w,data.size.h,data.stride,QImage::Format_ARGB32_Premultiplied);
    QPixmap pixmap = QPixmap::fromImage(*image);
    QLabel* label = new QLabel(widget);
    label->setPixmap(pixmap);*/
    /*widget->resize(pixmap.size());*/
    /*widget->show();*/

    demoWidget->resize(width,height);
    demoWidget->show();

    int exit_code = app.exec();
    /*delete  label;*/
    /*delete  image;*/
    /*delete  widget;*/
    delete  demoWidget;

    return  exit_code;
}
