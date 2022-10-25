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
#include <QColorDialog>
#include <chrono>

#include <blend2d.h>
#include "Vector/Vector.h"

using namespace ::ULIS;

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
    FVectorEngine mVEngine;
    double mMouseX, mMouseY;
    int32 mAction;
    void (MyWidget::*mTool)(QEvent*);
    FRectD mScreen;

public:
    MyWidget::MyWidget(uint32 iWidth,uint32 iHeight);
    void MyWidget::CreatePath(QEvent *e);
    void MyWidget::EditPath(QEvent *e);
    void MyWidget::PickObject(QEvent *e);
    void MyWidget::MoveObject(QEvent *e);
    void MyWidget::ScaleObject(QEvent *e);
    void MyWidget::RotateObject(QEvent *e);
    void MyWidget::CreateCircle(QEvent *e);
    void MyWidget::CreateRectangle(QEvent *e);
    void MyWidget::PanView(QEvent *e);

    void MyWidget::SelectCreatePath( bool checked );
    void MyWidget::SelectEditPath( bool checked );
    void MyWidget::SelectPickObject( bool checked );
    void MyWidget::SelectMoveObject( bool checked );
    void MyWidget::SelectScaleObject( bool checked );
    void MyWidget::SelectRotateObject( bool checked );
    void MyWidget::SelectCreateCircle( bool checked );
    void MyWidget::SelectCreateRectangle( bool checked );
    void MyWidget::SelectStrokeColor( bool checked );
    void MyWidget::SelectFillColor( bool checked );
    void MyWidget::SelectDeleteObject( bool checked );
    void MyWidget::SelectPanView( bool checked );

    virtual ~MyWidget() {
        delete mQImage; mBLContext->end();
    }

    virtual void paintEvent( QPaintEvent* e )
    {
        QPainter p(this);
        QRect region;
        FRectD& invalidateRegion = mVEngine.GetInvalidateRegion();

        mContext->Clear(*mCanvas);
        mContext->Finish();

        mBLContext->setFillStyle(BLRgba32(0xFFFFFFFF));
        mBLContext->setCompOp(BL_COMP_OP_SRC_COPY);



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
        /*mContext->DrawVectorObject ( *mCanvas, *mScene, *mBLContext );*/

        region.setRect( invalidateRegion.x
                      , invalidateRegion.y
                      , invalidateRegion.w
                      , invalidateRegion.h );

        mVEngine.Draw( *mCanvas, *mBLContext );

        mContext->Finish();

        /*p.drawImage( region, *mQImage, region );*/
        p.drawImage( rect(), *mQImage );

        p.end();
    }

    virtual void keyReleaseEvent(QKeyEvent* event)
    {
        mAction = -1;
    }

    virtual void keyPressEvent( QKeyEvent* event )
    {
        switch( event->key() )
        {
            case Qt::Key_T:
            {
                mAction = 0;
            }
            break;
            case Qt::Key_O:
            {
                mAction = 1;
            }
            break;

            case Qt::Key_X :
            {
                mAction = 2;
            }
            break;

            case Qt::Key_C :
            {
                FVectorCircle* circle = new FVectorCircle( 20.0f );

                circle->Translate( mQImage->width() / 2, mQImage->height() / 2 );
                circle->UpdateMatrix( *mBLContext );

                mVEngine.GetScene().AddChild( circle );
                mVEngine.GetScene().Select( *mBLContext, *circle );
            } 
            break;

            case Qt::Key_A:
            {
                mAction = 3;
            }
            break;

            case Qt::Key_P:
            {
                FVectorPathBuilder* builder = new FVectorPathBuilder();

                /*builder->Translate(mQImage->width() / 2,mQImage->height() / 2);*/
                builder->UpdateMatrix(*mBLContext);

                mVEngine.GetScene().AddChild(builder);
                mVEngine.GetScene().Select( *mBLContext, *builder );
            }
            break;

            case Qt::Key_R:
            {
                FVectorRectangle* rectangle = new FVectorRectangle( 50,80 );

                rectangle->Translate( mQImage->width() / 2, mQImage->height() / 2 );
                rectangle->UpdateMatrix( *mBLContext );

                mVEngine.GetScene().AddChild( rectangle );
                mVEngine.GetScene().Select( *mBLContext, *rectangle );
            }
            break;

            case Qt::Key_Plus:
            {
                FVectorRectangle* rectangle = new FVectorRectangle(50,80);
                FVectorRoot& scene = mVEngine.GetScene();

                scene.Scale( scene.GetScalingX() + 0.01f, scene.GetScalingY() + 0.01f );
                scene.UpdateMatrix( *mBLContext );
            }
            break;

            case Qt::Key_Minus:
            {
                FVectorRectangle* rectangle = new FVectorRectangle(50,80);
                FVectorRoot& scene = mVEngine.GetScene();

                scene.Scale( scene.GetScalingX() - 0.01f, scene.GetScalingY() - 0.01f );
                scene.UpdateMatrix( *mBLContext );
            }
            break;
        }

        update(); // redraw
    }


    virtual void mousePressEvent(QMouseEvent* event)
    {
        if ( mTool ) ((*this).*(mTool))( event );
    }

    virtual void mouseReleaseEvent( QMouseEvent* event )
    {
        if ( mTool ) ((*this).*(mTool))( event );
    }


     virtual void mouseMoveEvent(QMouseEvent* event)
     {
        if ( mTool ) ((*this).*(mTool))( event );
     }
};

#define CREATE_PATH      "Create Path"
#define EDIT_PATH        "Edit Path"
#define PICK_OBJECT      "Pick Object"
#define MOVE_OBJECT      "Move Object"
#define SCALE_OBJECT     "Scale Object"
#define ROTATE_OBJECT    "Rotate Object"
#define CREATE_CIRCLE    "Circle"
#define CREATE_RECTANGLE "Rectangle"
#define STROKE_COLOR     "Stroke Color"
#define FILL_COLOR       "Fill Color"
#define DELETE_OBJECT    "Delete Object"
#define PAN_VIEW         "Pan View"

class MyAction : public QAction {
    public :
        MyAction(){};
        void Triggered( bool checked );
};

void MyWidget::SelectCreatePath     ( bool checked ) { mTool = &MyWidget::CreatePath;     }
void MyWidget::SelectEditPath       ( bool checked ) { mTool = &MyWidget::EditPath;       }
void MyWidget::SelectPickObject     ( bool checked ) { mTool = &MyWidget::PickObject;     }
void MyWidget::SelectMoveObject     ( bool checked ) { mTool = &MyWidget::MoveObject;     }
void MyWidget::SelectScaleObject    ( bool checked ) { mTool = &MyWidget::ScaleObject;    }
void MyWidget::SelectRotateObject   ( bool checked ) { mTool = &MyWidget::RotateObject;   }
void MyWidget::SelectCreateCircle   ( bool checked ) { mTool = &MyWidget::CreateCircle;   }
void MyWidget::SelectCreateRectangle( bool checked ) { mTool = &MyWidget::CreateRectangle;}
void MyWidget::SelectPanView        ( bool checked ) { mTool = &MyWidget::PanView        ;}
void MyWidget::SelectStrokeColor    ( bool checked )
{
    QColor color = QColorDialog::getColor( Qt::white, this );
    if( color.isValid() )
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );

        selectedObject->SetStrokeColor( color.rgba() );
    }
}

void MyWidget::SelectFillColor ( bool checked )
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid())
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected());

        selectedObject->SetFilled(true);
        selectedObject->SetFillColor(color.rgba());
    }
}

void MyWidget::SelectDeleteObject ( bool checked )
{
    FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected());

    mVEngine.GetScene().RemoveChild( selectedObject );

    update();
}

void
MyWidget::CreatePath(QEvent *event)
{
    static BLMatrix2D inverseWorldMatrix;

    if( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorPathBuilder* builder = new FVectorPathBuilder();

        mVEngine.GetScene().AddChild( builder );

        builder->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().ClearSelection();
        mVEngine.GetScene().Select( *mBLContext, *builder );

        BLMatrix2D::invert( inverseWorldMatrix, builder->GetWorldMatrix() );
    }

    if( event->type() == QEvent::MouseMove )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorPathBuilder *currentPathBuilder = static_cast<FVectorPathBuilder*>( mVEngine.GetScene().GetLastSelected() );
        FVectorPathCubic* cubicPath = currentPathBuilder->GetSmoothedPath();
        BLPoint localCoords = inverseWorldMatrix.mapPoint( e->x(), e->y() );
        FRectD invalidateRegion = { e->x() - 100.0f, e->y() - 100.0f, 200.0f, 200.0f };
        FRectD finalRegion = invalidateRegion;

        currentPathBuilder->AppendPoint( localCoords.x, localCoords.y, 4.0f );
        FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(cubicPath->GetLastSegment());

        if ( cubicSegment )
        {
            FRectD& bbox = cubicSegment->GetBoundingBox();
            BLPoint localBBoxPoint = currentPathBuilder->GetWorldMatrix().mapPoint( bbox.x, bbox.y );
            BLPoint localBBoxSize  = currentPathBuilder->GetWorldMatrix().mapVector( bbox.w, bbox.h );
            FRectD worldBBox = { localBBoxPoint.x
                               , localBBoxPoint.y
                               , localBBoxSize.x
                               , localBBoxSize.y };

            /*worldBBox.Sanitize();*/

            invalidateRegion = finalRegion | worldBBox;
        } 

        invalidateRegion.Sanitize();

        finalRegion = invalidateRegion & mScreen;

        mVEngine.InvalidateRegion( finalRegion.x, finalRegion.y, finalRegion.w, finalRegion.h );

        /*mVEngine.InvalidateRegion(  );*/
    }

    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorPathBuilder *currentPathBuilder = static_cast<FVectorPathBuilder*>( mVEngine.GetScene().GetLastSelected());
        BLPoint localCoords = inverseWorldMatrix.mapPoint( e->x(), e->y() );

        currentPathBuilder->End( localCoords.x, localCoords.y, 1.0f );

        FVectorPathCubic* cubicPath = currentPathBuilder->GetSmoothedPath();

        currentPathBuilder->CopyTransformation( *cubicPath );

        cubicPath->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().AddChild( cubicPath );
        mVEngine.GetScene().RemoveChild( currentPathBuilder );
        mVEngine.GetScene().Select( *mBLContext, *cubicPath );
    }

    update();
}

void
MyWidget::PickObject( QEvent *event )
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);

        mVEngine.GetScene().ClearSelection();
        mVEngine.GetScene().Select( *mBLContext, e->x(), e->y(), 10.0f );

        update();
    }
}

void
MyWidget::EditPath( QEvent *event )
{
    static BLMatrix2D inverseWorldMatrix;

    if( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>( event );
        FVectorPathCubic *cubicPath = static_cast<FVectorPathCubic*>( mVEngine.GetScene().GetLastSelected() );

        if ( cubicPath )
        {

            BLPoint localCoords;
            BLPoint localSize;
            double localRadius;
            double radius = 10.0f;
            double x = e->x();
            double y = e->y();

            BLMatrix2D::invert( inverseWorldMatrix, cubicPath->GetWorldMatrix() );

            localCoords = inverseWorldMatrix.mapPoint( x, y );
            localSize   = inverseWorldMatrix.mapPoint( x + radius, 0.0f );

            localRadius = localSize.x - localCoords.x;

            mMouseX = localCoords.x;
            mMouseY = localCoords.y;

            cubicPath->Unselect( nullptr );

            cubicPath->PickPoint( localCoords.x, localCoords.y, localRadius );
        }
    }

    if( event->type() == QEvent::MouseMove )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>( event );
        FVectorPathCubic *cubicPath = static_cast<FVectorPathCubic*>( mVEngine.GetScene().GetLastSelected() );
        double radius = 10.0f;
        double x = e->x();
        double y = e->y();
        BLPoint localCoords = inverseWorldMatrix.mapPoint( x, y );
        double difx = localCoords.x - mMouseX;
        double dify = localCoords.y - mMouseY;

        std::list<FVectorPoint*> selectedPointList = cubicPath->GetSelectedPointList();

        for( std::list<FVectorPoint*>::iterator it = selectedPointList.begin(); it != selectedPointList.end(); ++it )
        {
            FVectorPoint *selectedPoint = *it;
            std::list<FVectorSegment*> segmentList = selectedPoint->GetSegmentList();

            switch ( selectedPoint->GetType() )
            {
                case FVectorHandlePoint::HANDLE_TYPE_POINT :
                {
                    FVectorHandlePoint* pointHandle = static_cast<FVectorHandlePoint*>( selectedPoint );
                    FVectorPointCubic* cubicPoint = static_cast<FVectorPointCubic*>( pointHandle->GetParent() );
                    FVec2D dif = { cubicPoint->GetX() - localCoords.x, cubicPoint->GetY() - localCoords.y };

                    cubicPoint->SetRadius( dif.Distance(), true );
                }
                break;

                case FVectorHandleSegment::HANDLE_TYPE_SEGMENT :
                {
                    FVectorHandleSegment* segmentHandle = static_cast<FVectorHandleSegment*>( selectedPoint );
                    FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(segmentHandle->GetParent());

                    selectedPoint->Set( selectedPoint->GetX() + difx
                                      , selectedPoint->GetY() + dify );

                    cubicSegment->BuildVariable();
                }
                break;;

                default :
                {
                    FVectorPointCubic* cubicPoint = static_cast<FVectorPointCubic*>( selectedPoint );

                    cubicPoint->Set( selectedPoint->GetX() + difx
                                   , selectedPoint->GetY() + dify, false );

                    for( std::list<FVectorSegment*>::iterator segit = segmentList.begin(); segit != segmentList.end(); ++segit )
                    {
                        FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(*segit);
                        FVectorHandleSegment* ctrlPoint = ( &cubicSegment->GetPoint(0) == selectedPoint ) ? static_cast<FVectorHandleSegment*>( &cubicSegment->GetControlPoint( 0 ) ) :
                                                                                                            static_cast<FVectorHandleSegment*>( &cubicSegment->GetControlPoint( 1 ) );

                        ctrlPoint->Set( ctrlPoint->GetX() + difx
                                      , ctrlPoint->GetY() + dify );
                    }

                    cubicPoint->BuildSegments();
                }
                break;
            }
        }

        mMouseX = localCoords.x;
        mMouseY = localCoords.y;
    }

    update();
}

void
MyWidget::PanView(QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);
    static BLMatrix2D inverseWorldMatrix;

    if( event->type() == QEvent::MouseButtonPress )
    {
        FVectorObject* sceneObject = static_cast<FVectorObject*>( &mVEngine.GetScene() );
        BLMatrix2D::invert( inverseWorldMatrix, sceneObject->GetWorldMatrix() );
        BLPoint localCoords = inverseWorldMatrix.mapPoint(  e->x(),  e->y() );

        mMouseX = localCoords.x;
        mMouseY = localCoords.y;
    }

    if( event->type() == QEvent::MouseMove )
    {
        if( e->buttons() == Qt::LeftButton )
        {
            FVectorObject *sceneObject = static_cast<FVectorObject*>( &mVEngine.GetScene() );

            BLMatrix2D::invert( inverseWorldMatrix, sceneObject->GetWorldMatrix() );
            BLPoint localCoords = inverseWorldMatrix.mapPoint(  e->x(),  e->y() );
            double difx = localCoords.x - mMouseX;
            double dify = localCoords.y - mMouseY;

            sceneObject->Translate( /*sceneObject->GetTranslationX() +*/ difx
                                  , /*sceneObject->GetTranslationY() +*/ dify );

            sceneObject->UpdateMatrix( *mBLContext );

           /* mMouseX = localCoords.x;
            mMouseY = localCoords.y;*/

            /*mVEngine.InvalidateRegion( beforeBBox | selectedObject->GetBBox( true ) );*/
        }
    }

    update();
}

void
MyWidget::MoveObject(QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);
    static BLMatrix2D inverseWorldMatrix;

    if( event->type() == QEvent::MouseButtonPress )
    {
        PickObject( event );

        if ( mVEngine.GetScene().GetLastSelected() )
        {
            FVectorObject* parentObject = mVEngine.GetScene().GetLastSelected()->GetParent();
            BLMatrix2D::invert( inverseWorldMatrix, parentObject->GetWorldMatrix() );
            BLPoint localCoords = inverseWorldMatrix.mapPoint(  e->x(),  e->y() );

            mMouseX = localCoords.x;
            mMouseY = localCoords.y;
        }
    }

    if( event->type() == QEvent::MouseMove )
    {
        if( e->buttons() == Qt::LeftButton )
        {
            FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );

            if( selectedObject )
            {
                FVectorObject* parentObject = mVEngine.GetScene().GetLastSelected()->GetParent();
                BLMatrix2D::invert( inverseWorldMatrix, parentObject->GetWorldMatrix() );
                BLPoint localCoords = inverseWorldMatrix.mapPoint(  e->x(),  e->y() );
                double difx = localCoords.x - mMouseX;
                double dify = localCoords.y - mMouseY;
                FRectD beforeBBox = selectedObject->GetBBox( true );

                selectedObject->Translate( selectedObject->GetTranslationX() + difx
                                         , selectedObject->GetTranslationY() + dify );

                selectedObject->UpdateMatrix( *mBLContext );

                mMouseX = localCoords.x;
                mMouseY = localCoords.y;

                mVEngine.InvalidateRegion( beforeBBox | selectedObject->GetBBox( true ) );
            }
        }
    }

    update();
}

void
MyWidget::ScaleObject(QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);

    if( event->type() == QEvent::MouseButtonPress )
    {
        PickObject( event );

        mMouseX = e->x();
        mMouseY = e->y();
    }

    if( event->type() == QEvent::MouseMove )
    {
        double difx = e->x() - mMouseX;
        double dify = e->y() - mMouseY;

        if( e->buttons() == Qt::LeftButton )
        {
            FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );

            if( selectedObject )
            {
                selectedObject->Scale( selectedObject->GetScalingX() + difx * 0.01f
                                     , selectedObject->GetScalingY() + dify * 0.01f );

                selectedObject->UpdateMatrix(*mBLContext);
            }
        }

        mMouseX = e->x();
        mMouseY = e->y();
    }

    update();
}

void
MyWidget::RotateObject(QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);

    if( event->type() == QEvent::MouseButtonPress )
    {
        PickObject( event );

        mMouseX = e->x();
    }

    if( event->type() == QEvent::MouseMove )
    {
        double difx = e->x() - mMouseX;

        if(e->buttons() == Qt::LeftButton)
        {
            FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );

            if( selectedObject )
            {
                selectedObject->Rotate( selectedObject->GetRotation() + difx * 0.01f );

                selectedObject->UpdateMatrix( *mBLContext );
            }
        }

        mMouseX = e->x();
    }

    update();
}

void
MyWidget::CreateCircle( QEvent *event )
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);
    static int mouseX, mouseY;

    if( event->type() == QEvent::MouseButtonPress )
    {
        FVectorCircle* circle = new FVectorCircle( 0.0f );
        FVec2D localCoordinates = mVEngine.GetScene().WorldCoordinatesToLocal( e->x(), e->y() );

        mouseX = e->x();
        mouseY = e->y();

        mVEngine.GetScene().AddChild( circle );

        circle->Translate( localCoordinates.x, localCoordinates.y );
        circle->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().Select( *mBLContext, *circle );
    }

    if( event->type() == QEvent::MouseMove )
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );
        FVectorCircle* circle = static_cast<FVectorCircle*>( selectedObject );
        FVec2D localCoordinates = mVEngine.GetScene().WorldCoordinatesToLocal( e->x(), e->y() );
        FVec2D dif = { localCoordinates.x - circle->GetTranslationX(),
                       localCoordinates.y - circle->GetTranslationY() };
        FRectD circleRegion = circle->GetBBox( true );

        circle->SetRadius( dif.Distance() );

        mVEngine.InvalidateRegion( circleRegion );
    }

    update();
}

void
MyWidget::CreateRectangle(QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);

    if( event->type() == QEvent::MouseButtonPress )
    {
        FVectorRectangle* rectangle = new FVectorRectangle( 0.0f, 0.0f );
        FVec2D localCoordinates = mVEngine.GetScene().WorldCoordinatesToLocal( e->x(), e->y() );

        rectangle->Translate( localCoordinates.x, localCoordinates.y );
        rectangle->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().AddChild( rectangle );
        mVEngine.GetScene().Select( *mBLContext, *rectangle );
    }

    if( event->type() == QEvent::MouseMove )
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );
        FVectorRectangle* rectangle = static_cast<FVectorRectangle*>( selectedObject );
        FVec2D localCoordinates = mVEngine.GetScene().WorldCoordinatesToLocal( e->x(), e->y() );
        FVec2D dif = { localCoordinates.x - rectangle->GetTranslationX(),
                       localCoordinates.y - rectangle->GetTranslationY() };
        FRectD rectangleRegion = rectangle->GetBBox( true );

        rectangle->SetSize( dif.x * 2.0f, dif.y * 2.0f );

        mVEngine.InvalidateRegion( rectangleRegion );
    }

    update();
}

MyWidget::MyWidget( uint32 iWidth, uint32 iHeight ) {
    QToolBar *toolbar = new QToolBar(this);
    QPixmap pathPix("new.png");

    MyAction *createPathAction         = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), CREATE_PATH      ) );
    MyAction *editPathAction           = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), EDIT_PATH        ) );
    MyAction *pickObjectAction         = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), PICK_OBJECT      ) );
    MyAction *moveObjectAction         = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), MOVE_OBJECT      ) );
    MyAction *scaleObjectAction        = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), SCALE_OBJECT     ) );
    MyAction *rotateObjectAction       = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), ROTATE_OBJECT    ) );
    MyAction *createCircleAction       = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), CREATE_CIRCLE    ) );
    MyAction *createRectangleAction    = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), CREATE_RECTANGLE ) );
    MyAction *createStrokeColorAction  = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), STROKE_COLOR     ) );
    MyAction *createFillColorAction    = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), FILL_COLOR       ) );
    MyAction *createDeleteObjectAction = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), DELETE_OBJECT    ) );
    MyAction *createPanViewAction      = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), PAN_VIEW         ) );

    connect( createPathAction        , &MyAction::triggered, this, &MyWidget::SelectCreatePath      );
    connect( editPathAction          , &MyAction::triggered, this, &MyWidget::SelectEditPath        );
    connect( pickObjectAction        , &MyAction::triggered, this, &MyWidget::SelectPickObject      );
    connect( moveObjectAction        , &MyAction::triggered, this, &MyWidget::SelectMoveObject      );
    connect( scaleObjectAction       , &MyAction::triggered, this, &MyWidget::SelectScaleObject     );
    connect( rotateObjectAction      , &MyAction::triggered, this, &MyWidget::SelectRotateObject    );
    connect( createCircleAction      , &MyAction::triggered, this, &MyWidget::SelectCreateCircle    );
    connect( createRectangleAction   , &MyAction::triggered, this, &MyWidget::SelectCreateRectangle );
    connect( createStrokeColorAction , &MyAction::triggered, this, &MyWidget::SelectStrokeColor     );
    connect( createFillColorAction   , &MyAction::triggered, this, &MyWidget::SelectFillColor       );
    connect( createDeleteObjectAction, &MyAction::triggered, this, &MyWidget::SelectDeleteObject    );
    connect( createPanViewAction     , &MyAction::triggered, this, &MyWidget::SelectPanView         );

    /*setAttribute(Qt::WA_OpaquePaintEvent);*/

    toolbar->move( 0, 0 );

    BLImageData data;
    eFormat fmt = Format_RGBA8;

    mScreen.x = 0;
    mScreen.y = 0;
    mScreen.w = iWidth;
    mScreen.h = iHeight;

    mAction = -1;
    mTool = nullptr;

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
                                 , QImage::Format_ARGB32 );

    mVEngine.GetScene().UpdateMatrix( *mBLContext );

}

int
main(int argc, char* argv[])
{
    int width = 1024;
    int height = 768;

// DEBUG
srand(time(0));
// END DEBUG

    QApplication app(argc,argv);

    MyWidget* demoWidget = new MyWidget( width,height );

    demoWidget->resize( width, height );
    demoWidget->show();

    int exit_code = app.exec();

    delete  demoWidget;

    return  exit_code;
}
