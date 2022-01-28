// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CustomCaption.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QObject>
#include <QWidget>

class  FCustomCaption
    : public QWidget
{
    Q_OBJECT

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  FCustomCaption     tSelf;
    typedef  QWidget            tSuperClass;

public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~FCustomCaption()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~FCustomCaption();


    /// @fn         FCustomCaption( QWidget *parent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init member data with default values.
    FCustomCaption( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//----------------------------------------- Custom NC Handling implementation in caption
    /// @fn         virtual  bool  HitEmptySpace( long iX, long iY )
    /// @brief      Check if the cursor hit empty space to allow for NC events.
    /// @details    Handles internal geometry tests.
    /// @param      iX      the x coordinate in parent space.
    /// @param      iY      the y coordinate in parent space.
    /// @return     A boolean flag stating if the hit succeeded or not.
    virtual  bool  HitEmptySpace( long iX, long iY );


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
    virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override;


public slots:
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- Public Qt Slots
    void  ProcessCloseClicked();
    void  ProcessMaximizeClicked();
    void  ProcessMinimizeClicked();


signals:
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Qt Signals
    void  CloseClicked();
    void  MaximizeClicked();
    void  MinimizeClicked();


};

