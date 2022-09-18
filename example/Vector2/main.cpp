#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <chrono>
#include <blend2d.h>
using namespace ::ULIS;

#include "Vector/FVectorObject.h"

/******************************************************************************/
class MyWidget: public QWidget
{

private:
    QImage* mQImage;
    /*FVectorPathCubic* mCubicPath;
    FVectorCircle* mCircle;*/
    BLImage* mBLImage;
    BLContext* mBLContext;
    FBlock* mCanvas;
    FThreadPool* mPool;
    FCommandQueue* mQueue;
    FContext* mContext;
    FVectorObject* mScene;

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

        /*mCubicPath->Draw( *mCanvas, *mBLContext );*/
/*
        mCubicPath->Translate( 200, 200 );
        mCubicPath->Rotate( 45 );
        mCubicPath->Scale( 1.0f, 1.0f );
        mCubicPath->UpdateMatrix( *mBLContext );
*/
        // mandatory for now
/*
        mCircle->UpdateMatrix( *mBLContext );
*/
        /*mContext->DrawVectorObject ( *mCanvas, *mCubicPath, *mBLContext );*/
        mContext->DrawVectorObject ( *mCanvas, *mScene, *mBLContext );
        mContext->Finish();

        p.drawImage( rect(), *mQImage );
    }

    virtual void keyPressEvent( QKeyEvent* event )
    {
        switch( event->key() )
        {
            case Qt::Key_C :
            {
                FVectorCircle* circle = new FVectorCircle( 20.0f );

                circle->Translate( mQImage->width() / 2, mQImage->height() / 2 );
                circle->UpdateMatrix( *mBLContext );

                mScene->AddChild( circle );
            } 
            break;

            case Qt::Key_R:
            {
                FVectorRectangle* rectangle = new FVectorRectangle( 50,80 );

                rectangle->Translate( mQImage->width() / 2, mQImage->height() / 2 );
                rectangle->UpdateMatrix( *mBLContext );

                mScene->AddChild( rectangle );
            }
            break;

            case Qt::Key_Plus:
            {
                FVectorRectangle* rectangle = new FVectorRectangle(50,80);

                mScene->Scale( mScene->GetScalingX() + 0.01f, mScene->GetScalingY() + 0.01f );
                mScene->UpdateMatrix(*mBLContext);
            }
            break;

            case Qt::Key_Minus:
            {
                FVectorRectangle* rectangle = new FVectorRectangle(50,80);

                mScene->Scale(mScene->GetScalingX() - 0.01f,mScene->GetScalingY() - 0.01f);
                mScene->UpdateMatrix(*mBLContext);
            }
            break;
        }

        update(); // redraw
    }

    virtual void mousePressEvent(QMouseEvent* event)
    {
/*
        mCubicPath->Unselect(nullptr);

        mCubicPath->Pick(event->x(),event->y(), 10.0f);
*/
/*
        const uint32_t someColor = rand();
        const size_t frameBufferSizeWords = frameBufferSizeBytes/sizeof(uint32_t);
        uint32* fb32 = (uint32* ) frameBuffer;
        for(size_t i=0; i<frameBufferSizeWords; i++) fb32[i] = someColor;
*/
        update(); // redraw
    }

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        if (event->buttons() == Qt::LeftButton )
        {
/*
            std::list<FVectorPoint*> selectedPointList = mCubicPath->GetSelectedPointList();

            for(std::list<FVectorPoint*>::iterator it = selectedPointList.begin(); it != selectedPointList.end(); ++it)
            {
                FVectorPoint *selectedPoint = *it;

                selectedPoint->SetX(event->x());
                selectedPoint->SetY(event->y());
            }
*/

            update();
        }
    }
};

MyWidget::MyWidget( uint32 iWidth, uint32 iHeight ) {

    BLImageData data;
    eFormat fmt = Format_RGBA8;

    // ULIS part
    mPool = new FThreadPool();
    mQueue = new FCommandQueue( *mPool );
    mContext = new FContext( *mQueue, Format_RGBA8 );
    mCanvas = new FBlock( iWidth, iHeight, fmt);

    // Blend2D part
    mBLImage = new BLImage(iWidth,iHeight,BL_FORMAT_PRGB32);
    mBLContext = new BLContext(*mBLImage);
    mBLImage->getData( &data );

    mQImage = new QImage( (uchar*) data.pixelData
                                 , data.size.w
                                 , data.size.h
                                 , data.stride
                                 , QImage::Format_ARGB32_Premultiplied );

    mScene = new FVectorObject();

    mScene->UpdateMatrix( *mBLContext );
/*
    mCubicPath = new FVectorPathCubic();
    mCircle = new FVectorCircle();

    FVectorPoint* point0 = new FVectorPoint( 26, 31 );
    FVectorPoint* point1 = new FVectorPoint( 25, 464 );
    FVectorPoint* point2 = new FVectorPoint( 200, 31 );

    FVectorSegmentCubic* segment;
    FVectorPoint* ctrlPoint0;
    FVectorPoint* ctrlPoint1;

    mCubicPath->AppendPoint( point0 );

    segment = static_cast<FVectorSegmentCubic*>( mCubicPath->AppendPoint( point1 ) );

    ctrlPoint0 = segment->GetControlPoint( 0 );
    ctrlPoint1 = segment->GetControlPoint( 1 );

    ctrlPoint0->SetX( 50 );
    ctrlPoint0->SetY( 50 );
    ctrlPoint1->SetX( 50 );
    ctrlPoint1->SetY( 400 );

    segment = static_cast<FVectorSegmentCubic*>( mCubicPath->AppendPoint( point2 ) );
    ctrlPoint0 = segment->GetControlPoint( 0 );
    ctrlPoint1 = segment->GetControlPoint( 1 );

    ctrlPoint0->SetX( 560 );
    ctrlPoint0->SetY( 404 );
    ctrlPoint1->SetX( 144 );
    ctrlPoint1->SetY( 267 );
*/
}

int
main(int argc, char* argv[])
{
    int width = 640;
    int height = 480;

    QApplication app(argc,argv);

    MyWidget* demoWidget = new MyWidget( width,height );

    demoWidget->resize( width, height );
    demoWidget->show();

    int exit_code = app.exec();

    delete  demoWidget;

    return  exit_code;
}
