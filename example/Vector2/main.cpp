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
    uint32 mFillColor;

public:
    MyWidget::MyWidget(uint32 iWidth,uint32 iHeight);
    void MyWidget::CreatePath(QEvent *e);
    void MyWidget::CutPath(QEvent *e);
    void MyWidget::EditPath(QEvent *e);
    void MyWidget::PickObject(QEvent *e);
    void MyWidget::MoveObject(QEvent *e);
    void MyWidget::ScaleObject(QEvent *e);
    void MyWidget::RotateObject(QEvent *e);
    void MyWidget::CreateCircle(QEvent *e);
    void MyWidget::CreateRectangle(QEvent *e);
    void MyWidget::PanView(QEvent *e);
    void MyWidget::Bucket(QEvent *e);
    void MyWidget::MirrorX(QEvent *e);
    void MyWidget::MirrorY(QEvent *e);

    void MyWidget::SelectCreatePath( bool checked );
    void MyWidget::SelectCutPath( bool checked );
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
    void MyWidget::SelectBucket( bool checked );
    void MyWidget::SelectGroupObjects( bool checked );
    void MyWidget::SelectMirrorX( bool checked );
    void MyWidget::SelectMirrorY( bool checked );
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

        mBLContext->flush(BL_CONTEXT_FLUSH_SYNC);

        p.end();
    }

    virtual void keyReleaseEvent(QKeyEvent* event)
    {
        mAction = -1;
    }

    virtual void keyPressEvent( QKeyEvent* event )
    {
        static FVectorObject* copiedObject;

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

            case Qt::Key_Less:
            {
                FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

                if( selectedObject )
                {
                    selectedObject->MoveBack();
                }
            }
            break;

            case Qt::Key_Greater:
            {
                FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

                if( selectedObject )
                {
                    selectedObject->MoveFront();
                }
            }
            break;

            case Qt::Key_C :
            {
                if ( QApplication::keyboardModifiers().testFlag( Qt::ControlModifier ) == true )
                {
                    FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

                    if ( selectedObject )
                    {
                        if ( copiedObject )
                        {
                            delete copiedObject;
                        }

                        copiedObject = selectedObject->Copy();
                    }
                }
            } 
            break;

            case Qt::Key_V :
            {
                if ( QApplication::keyboardModifiers().testFlag( Qt::ControlModifier ) == true )
                {
                    if ( copiedObject )
                    {
                        FVectorObject* pastedObject =  copiedObject->Copy();

                        mVEngine.GetScene().AppendChild( pastedObject );
                        mVEngine.GetScene().Select( *mBLContext, *pastedObject );

                        pastedObject->UpdateMatrix( *mBLContext );
                    }
                }
            } 
            break;

            case Qt::Key_A:
            {
                mAction = 3;
            }
            break;

            case Qt::Key_R:
            {
                FVectorRectangle* rectangle = new FVectorRectangle( 50,80 );

                rectangle->Translate( mQImage->width() / 2, mQImage->height() / 2 );
                rectangle->UpdateMatrix( *mBLContext );

                mVEngine.GetScene().AppendChild( rectangle );
                mVEngine.GetScene().Select( *mBLContext, *rectangle );
            }
            break;

            case Qt::Key_Plus:
            {
                FVectorRoot& scene = mVEngine.GetScene();

                scene.Scale( scene.GetScalingX() + 0.01f, scene.GetScalingY() + 0.01f );
                scene.UpdateMatrix( *mBLContext );
            }
            break;

            case Qt::Key_Minus:
            {
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

     void wheelEvent( QWheelEvent *event )
     {
         QPoint numPixels = event->pixelDelta();
         QPoint numDegrees = event->angleDelta() / 8;

         if( !numPixels.isNull() ) {
             /*scrollWithPixels(numPixels);*/
         } else if( !numDegrees.isNull() ) {
             QPoint numSteps = numDegrees / 15;
             FVectorRoot& scene = mVEngine.GetScene();
             double step = ( double ) numSteps.y() * 0.05f;
             BLMatrix2D& worldMatrix = scene.GetWorldMatrix();
             BLMatrix2D& inverseWorldMatrix = scene.GetInverseWorldMatrix();
             BLPoint localCoords = inverseWorldMatrix.mapPoint( ( double ) event->x(), ( double ) event->y() );

             scene.Scale( scene.GetScalingX() + step, scene.GetScalingY() + step );
             scene.UpdateMatrix(*mBLContext);

             BLPoint worldCoords = worldMatrix.mapPoint( localCoords );

             scene.Translate( scene.GetTranslationX() - ( worldCoords.x - ( double ) event->x() )
                            , scene.GetTranslationY() - ( worldCoords.y - ( double ) event->y() ) );
             scene.UpdateMatrix(*mBLContext);

             /*scrollWithDegrees(numSteps);*/
         }

         event->accept();

        update();
     }
};

#define CREATE_PATH      "Create Path"
#define CUT_PATH         "Cut Path"
#define EDIT_PATH        "Edit Path"
#define PICK_OBJECT      "Pick"
#define MOVE_OBJECT      "Move"
#define SCALE_OBJECT     "Scale"
#define ROTATE_OBJECT    "Rotate"
#define CREATE_CIRCLE    "Circle"
#define CREATE_RECTANGLE "Rectangle"
#define STROKE_COLOR     "Stroke"
#define FILL_COLOR       "Fill"
#define DELETE_OBJECT    "Delete"
#define PAN_VIEW         "Pan"
#define BUCKET           "Bucket"
#define GROUP_OBJECTS    "Group"
#define MIRROR_X         "Mirror X"
#define MIRROR_Y         "Mirror Y"

class MyAction : public QAction {
    public :
        MyAction(){};
        void Triggered( bool checked );
};

void MyWidget::SelectCreatePath     ( bool checked ) { mTool = &MyWidget::CreatePath;     }
void MyWidget::SelectCutPath        ( bool checked ) { mTool = &MyWidget::CutPath;        }
void MyWidget::SelectEditPath       ( bool checked ) { mTool = &MyWidget::EditPath;       }
void MyWidget::SelectPickObject     ( bool checked ) { mTool = &MyWidget::PickObject;     }
void MyWidget::SelectMoveObject     ( bool checked ) { mTool = &MyWidget::MoveObject;     }
void MyWidget::SelectScaleObject    ( bool checked ) { mTool = &MyWidget::ScaleObject;    }
void MyWidget::SelectRotateObject   ( bool checked ) { mTool = &MyWidget::RotateObject;   }
void MyWidget::SelectCreateCircle   ( bool checked ) { mTool = &MyWidget::CreateCircle;   }
void MyWidget::SelectCreateRectangle( bool checked ) { mTool = &MyWidget::CreateRectangle;}
void MyWidget::SelectPanView        ( bool checked ) { mTool = &MyWidget::PanView        ;}
void MyWidget::SelectBucket         ( bool checked ) { mTool = &MyWidget::Bucket         ;}
void MyWidget::SelectMirrorX        ( bool checked )
{
    FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

    if ( selectedObject )
    {
        if( typeid ( *selectedObject ) == typeid ( FVectorPathCubic ) )
        {
            FVectorPathCubic *cubicPath = static_cast<FVectorPathCubic*>(selectedObject);

            cubicPath->Mirror( true, false );
            cubicPath->Update();
        }
    }
}

void MyWidget::SelectMirrorY        ( bool checked )
{
    FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

    if ( selectedObject )
    {
        if( typeid ( *selectedObject ) == typeid ( FVectorPathCubic ) )
        {
            FVectorPathCubic *cubicPath = static_cast<FVectorPathCubic*>(selectedObject);

            cubicPath->Mirror( false, true );
            cubicPath->Update();
        }
    }
}

void MyWidget::SelectGroupObjects   ( bool checked )
{
    FVectorGroup* group = mVEngine.GetScene().GroupSelectdObjects();

    mVEngine.GetScene().ClearSelection();
    mVEngine.GetScene().Select( mVEngine.GetBLContext(), *group );

    /*group->UpdateMatrix( *mBLContext );*/

    /*mVEngine.GetScene().AddChild( group );*/

    update();
}

void MyWidget::SelectStrokeColor    ( bool checked )
{
    QColor color = QColorDialog::getColor( Qt::white, this );
    if( color.isValid() )
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );

        if ( selectedObject )
        {
            selectedObject->SetStrokeColor( color.rgba() );
        }
    }
}

void MyWidget::SelectFillColor ( bool checked )
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if(color.isValid())
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected());

        if( selectedObject )
        {
            selectedObject->SetFilled(true);
            selectedObject->SetFillColor(color.rgba());
        }

        mFillColor = color.rgba(); // save for loop filling
    }
}

void MyWidget::SelectDeleteObject ( bool checked )
{
    FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected());

    mVEngine.GetScene().RemoveChild( selectedObject );

    update();
}

void
MyWidget::CutPath(QEvent *event)
{
    static BLMatrix2D inverseWorldMatrix;
    static FVec2D startCutAt;
    static FVec2D endCutAt;

    if( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

        if ( selectedObject )
        {
            BLPoint localCoords = selectedObject->GetInverseWorldMatrix().mapPoint( e->x(), e->y() );

            startCutAt.x = localCoords.x;
            startCutAt.y = localCoords.y;
        }
    }

    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

        if ( selectedObject )
        {
            BLPoint localCoords = selectedObject->GetInverseWorldMatrix().mapPoint( e->x(), e->y() );

            endCutAt.x = localCoords.x;
            endCutAt.y = localCoords.y;

            if( typeid ( *selectedObject ) == typeid ( FVectorPathCubic ) )
            {
                FVectorPathCubic *cubicPath = static_cast<FVectorPathCubic*>(selectedObject);

                cubicPath->Cut( startCutAt, endCutAt );

                cubicPath->Update();
            }
        }

        mVEngine.GetScene().Update();
    }

    update();
}

void
MyWidget::CreatePath(QEvent *event)
{
    static BLMatrix2D inverseWorldMatrix;

    if( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorPathCubic *cubicPath = nullptr;
        FVectorPathBuilder* builder;

        if ( QApplication::keyboardModifiers().testFlag( Qt::ControlModifier ) == true )
        {
            cubicPath =  static_cast<FVectorPathCubic*>( mVEngine.GetScene().GetLastSelected() );
        }

        if ( cubicPath == nullptr ) 
        {
            cubicPath = new FVectorPathCubic();

            mVEngine.GetScene().AppendChild( cubicPath );
        }

        builder = new FVectorPathBuilder( cubicPath );

        mVEngine.GetScene().AppendChild( builder );

        builder->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().ClearSelection();
        mVEngine.GetScene().Select( *mBLContext, *builder );

        BLMatrix2D::invert( inverseWorldMatrix, builder->GetWorldMatrix() );
    }

    if( event->type() == QEvent::MouseMove )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorPathBuilder *currentPathBuilder = static_cast<FVectorPathBuilder*>( mVEngine.GetScene().GetLastSelected() );
        FVectorPathCubic* cubicPath = currentPathBuilder->GetCubicPath();
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

        mVEngine.GetScene().Update();

       /* mVEngine.InvalidateRegion( finalRegion.x, finalRegion.y, finalRegion.w, finalRegion.h ); */

        /*mVEngine.InvalidateRegion(  );*/
    }

    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        FVectorPathBuilder *currentPathBuilder = static_cast<FVectorPathBuilder*>( mVEngine.GetScene().GetLastSelected());
        BLPoint localCoords = inverseWorldMatrix.mapPoint( e->x(), e->y() );

        if( QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == false )
        {
            currentPathBuilder->End( localCoords.x, localCoords.y, 1.0f, false );
        }
        else
        {
            currentPathBuilder->End( localCoords.x, localCoords.y, 1.0f, true );
        }

        FVectorPathCubic* cubicPath = currentPathBuilder->GetCubicPath();

        currentPathBuilder->CopyTransformation( *cubicPath );

        cubicPath->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().RemoveChild( currentPathBuilder );
        mVEngine.GetScene().ClearSelection ( );
        mVEngine.GetScene().Select( *mBLContext, *cubicPath );

        mVEngine.GetScene().Update();
    }

    update();
}

void
MyWidget::PickObject( QEvent *event )
{
    if( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>(event);

        if ( QApplication::keyboardModifiers().testFlag( Qt::ControlModifier ) == false )
        {
            mVEngine.GetScene().ClearSelection();
        }

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
        FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

        if ( selectedObject )
        {
            bool picked = false;

            if ( typeid ( *selectedObject ) == typeid ( FVectorPathCubic ) )
            {
                FVectorPathCubic *cubicPath = static_cast<FVectorPathCubic*>( selectedObject );
                BLPoint localCoords;
                BLPoint localSize;
                double localRadius;
                double radius = 10.0f;
                double x = e->x();
                double y = e->y();
                uint64 selectionFlags = 0;

                BLMatrix2D::invert( inverseWorldMatrix, cubicPath->GetWorldMatrix() );

                localCoords = inverseWorldMatrix.mapPoint( x, y );
                localSize   = inverseWorldMatrix.mapVector( radius, radius );

                localRadius = sqrt ( ( localSize.x * localSize.x ) + ( localSize.y * localSize.y ) );

                mMouseX = localCoords.x;
                mMouseY = localCoords.y;

                cubicPath->Unselect( nullptr );

                if ( QApplication::keyboardModifiers().testFlag( Qt::ControlModifier ) == true )
                {
                    selectionFlags = FVectorPath::PICK_HANDLE_POINT;
                }
                else
                {
                    selectionFlags = FVectorPath::PICK_HANDLE_SEGMENT | FVectorPath::PICK_POINT;
                }

                picked = cubicPath->PickPoint( localCoords.x, localCoords.y, localRadius, selectionFlags );

                if ( picked == false )
                {
                    PickObject (event);
                }
            }
        }
        else
        {
            PickObject ( event );
        }
    }

    if( event->type() == QEvent::MouseButtonRelease )
    {

    }

    if( event->type() == QEvent::MouseMove )
    {
        QMouseEvent *e = static_cast<QMouseEvent*>( event );
        FVectorObject* selectedObject = mVEngine.GetScene().GetLastSelected();

        if ( selectedObject && ( typeid ( *selectedObject ) == typeid ( FVectorPathCubic ) ) )
        {
            FVectorPathCubic *cubicPath = static_cast<FVectorPathCubic*>( selectedObject );
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
                    case FVectorPoint::POINT_TYPE_HANDLE_POINT :
                    {
                        FVectorHandlePoint* pointHandle = static_cast<FVectorHandlePoint*>( selectedPoint );
                        FVectorPointCubic& cubicPoint = static_cast<FVectorPointCubic&>(pointHandle->GetParent());
                        FVec2D dif = { cubicPoint.GetX() - localCoords.x, cubicPoint.GetY() - localCoords.y };

                        cubicPoint.SetRadius( dif.Distance(), true );
                    }
                    break;

                    case FVectorPoint::POINT_TYPE_HANDLE_SEGMENT :
                    {
                        FVectorHandleSegment* segmentHandle = static_cast<FVectorHandleSegment*>( selectedPoint );
                        FVectorSegmentCubic& cubicSegment = static_cast<FVectorSegmentCubic&>(segmentHandle->GetParent());

                        selectedPoint->Set( selectedPoint->GetX() + difx
                                          , selectedPoint->GetY() + dify );

                        /*cubicSegment.BuildVariable();*/
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
                            FVectorHandleSegment* ctrlPoint = ( cubicSegment->GetPoint(0) == selectedPoint ) ? static_cast<FVectorHandleSegment*>( &cubicSegment->GetControlPoint( 0 ) ) :
                                                                                                               static_cast<FVectorHandleSegment*>( &cubicSegment->GetControlPoint( 1 ) );

                            ctrlPoint->Set( ctrlPoint->GetX() + difx
                                          , ctrlPoint->GetY() + dify );
                        }

                        /*cubicPoint->BuildSegments();*/
                    }
                    break;
                }
            }

            mVEngine.GetScene().Update();

            mMouseX = localCoords.x;
            mMouseY = localCoords.y;
        }
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
        /*BLMatrix2D::invert( inverseWorldMatrix, sceneObject->GetWorldMatrix() );*/
        BLPoint localCoords = { (double) e->x(),  (double) e->y() };

        mMouseX = localCoords.x;
        mMouseY = localCoords.y;
    }

    if( event->type() == QEvent::MouseMove )
    {
        if( e->buttons() == Qt::LeftButton )
        {
            FVectorObject *sceneObject = static_cast<FVectorObject*>( &mVEngine.GetScene() );
            /*BLMatrix2D::invert( inverseWorldMatrix, sceneObject->GetWorldMatrix() );*/
            BLPoint localCoords = { (double) e->x(),  (double) e->y() };
            double difx = localCoords.x - mMouseX;
            double dify = localCoords.y - mMouseY;

            sceneObject->Translate( sceneObject->GetTranslationX() + difx
                                  , sceneObject->GetTranslationY() + dify );

            sceneObject->UpdateMatrix( *mBLContext );

            mMouseX = localCoords.x;
            mMouseY = localCoords.y;

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

                /*mVEngine.InvalidateRegion( beforeBBox | selectedObject->GetBBox( true ) );*/
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
MyWidget::Bucket(QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);

    if( event->type() == QEvent::MouseButtonPress )
    {
        FVec2D localCoordinates = mVEngine.GetScene().WorldCoordinatesToLocal( e->x(), e->y() );

        mVEngine.GetScene().Bucket( *mBLContext, localCoordinates.x, localCoordinates.y, mFillColor );
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

        mVEngine.GetScene().AppendChild( circle );

        circle->Translate( localCoordinates.x, localCoordinates.y );
        circle->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().ClearSelection();
        mVEngine.GetScene().Select( *mBLContext, *circle );
    }

    if( event->type() == QEvent::MouseMove )
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );

        if ( selectedObject )
        {
            FVectorCircle* circle = static_cast<FVectorCircle*>(selectedObject);
            FVec2D localCoordinates = selectedObject->WorldCoordinatesToLocal( e->x(),e->y() );
            FVec2D dif ={ localCoordinates.x,
                          localCoordinates.y };
            FRectD beforeBBox = selectedObject->GetBBox(true);

            circle->SetRadius( dif.Distance() );

            mVEngine.InvalidateRegion(beforeBBox | selectedObject->GetBBox(true));
        }
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

        mVEngine.GetScene().AppendChild( rectangle );

        rectangle->Translate( localCoordinates.x, localCoordinates.y );
        rectangle->UpdateMatrix( *mBLContext );

        mVEngine.GetScene().ClearSelection();
        mVEngine.GetScene().Select( *mBLContext, *rectangle );
    }

    if( event->type() == QEvent::MouseMove )
    {
        FVectorObject *selectedObject = static_cast<FVectorObject*>( mVEngine.GetScene().GetLastSelected() );

        if ( selectedObject )
        {
            FVectorRectangle* rectangle = static_cast<FVectorRectangle*>( selectedObject );
            FVec2D localCoordinates = selectedObject->WorldCoordinatesToLocal( e->x(), e->y() );
            FVec2D dif = { localCoordinates.x,
                           localCoordinates.y };
            FRectD beforeBBox = selectedObject->GetBBox( true );

            rectangle->SetSize( dif.x * 2.0f, dif.y * 2.0f );

            mVEngine.InvalidateRegion( beforeBBox | selectedObject->GetBBox( true ) );
        }
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
    MyAction *createBucketAction       = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), BUCKET           ) );
    MyAction *groupObjectsAction       = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), GROUP_OBJECTS    ) );
    MyAction *cutPathAction            = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), CUT_PATH         ) );
    MyAction *mirrorXAction            = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), MIRROR_X         ) );
    MyAction *mirrorYAction            = static_cast<MyAction*>( toolbar->addAction( QIcon( pathPix ), MIRROR_Y         ) );

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
    connect( createBucketAction      , &MyAction::triggered, this, &MyWidget::SelectBucket          );
    connect( groupObjectsAction      , &MyAction::triggered, this, &MyWidget::SelectGroupObjects    );
    connect( cutPathAction           , &MyAction::triggered, this, &MyWidget::SelectCutPath         );
    connect( mirrorXAction           , &MyAction::triggered, this, &MyWidget::SelectMirrorX         );
    connect( mirrorYAction           , &MyAction::triggered, this, &MyWidget::SelectMirrorY         );

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
   /* BLContextCreateInfo createInfo{};*/

    // Configure the number of threads to use.
    /*createInfo.threadCount = 1;*/

    mBLContext = &FVectorEngine::GetBLContext();
    mBLImage = new BLImage( iWidth,iHeight,BL_FORMAT_PRGB32 );
    mBLImage->getData( &data );

     BLResult result = mBLContext->begin(*mBLImage/*, createInfo*/);

    /*mBLContext->setRenderingQuality(0);*/

     if(result != BL_SUCCESS)
printf("failure\n");

    mQImage = new QImage( (uchar*) data.pixelData
                                 , data.size.w
                                 , data.size.h
                                 , data.stride
                                 , QImage::Format_ARGB32 );

    mVEngine.GetScene().UpdateMatrix( *mBLContext );

/*
    FVectorPathCubic* testPath = new FVectorPathCubic();
    FVectorPointCubic* cubicPoints[4] = { new FVectorPointCubic( 200, 300, 1 )
                                        , new FVectorPointCubic( 500, 250, 1 )
                                        , new FVectorPointCubic( 540, 220, 1 )
                                        , new FVectorPointCubic( 230, 270, 1 ) };

    FVectorSegmentCubic* cubicSegments[2];

                       testPath->AppendPoint( cubicPoints[0],  true, false );
    cubicSegments[0] = testPath->AppendPoint( cubicPoints[1],  true, false );
                       testPath->AppendPoint( cubicPoints[2], false, false );
    cubicSegments[1] = testPath->AppendPoint( cubicPoints[3],  true, false );

    cubicSegments[0]->GetControlPoint(0).Set( 230, 250 );
    cubicSegments[0]->GetControlPoint(1).Set( 470, 200 );
printf("FirstSegment = %d\n", cubicSegments[0] );
printf("SecondSegment = %d\n", cubicSegments[1] );
    cubicPoints[1]->BuildSegments();

    cubicSegments[1]->GetControlPoint(0).Set( 539, 220 );
    cubicSegments[1]->GetControlPoint(1).Set( 231, 270 );

    cubicPoints[3]->BuildSegments();

    mVEngine.GetScene().AddChild( testPath );

    testPath->UpdateMatrix( *mBLContext );

    cubicSegments[0]->IntersectPath(*testPath);
*/
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
