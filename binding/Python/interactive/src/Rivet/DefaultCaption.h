// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DefaultCaption.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "CustomCaption.h"
#include "CustomButton.h"
#include <QObject>

/////////////////////////////////////////////////////
/// @class      FDefaultCaption
/// @brief      The FDefaultCaption class provides a default custom caption widget.
/// @details    It provides window snap / close / minimize / maximize mechanisms and buttons.
class  FDefaultCaption
    : public  FCustomCaption
{
    Q_OBJECT

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  FDefaultCaption    tSelf;
    typedef  FCustomCaption     tSuperClass;


public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~FDefaultCaption()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~FDefaultCaption();


    /// @fn         FDefaultCaption( QWidget* iParent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init data members.
    FDefaultCaption( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Public Geometry API
    QRect  ButtonsGeometry();


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------- Public customization utilities
    void  ActiveBackgroundColor( const  QColor& iColor );
    const  QColor&  ActiveBackgroundColor();
    void  InactiveBackgroundColor( const  QColor& iColor );
    const  QColor&  InactiveBackgroundColor();
    void  SetBlackControls();
    void  SetWhiteControls();


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
    virtual  void  paintEvent(  QPaintEvent*    event )     override;
    virtual  void  resizeEvent( QResizeEvent*   event )     override;


protected:
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------- Internal GUI management
    /// @fn         void  Init()
    /// @brief      Init GUI members.
    /// @details    Called only once by constructor.
    void  Init();

    /// @fn         void  Build()
    /// @brief      Setup GUI members flags, styles, properties.
    /// @details    Called only once by constructor.
    void  Build();


    /// @fn         void  Compose()
    /// @brief      Compose geometry of members in GUI.
    /// @details    Called repeatedly on resize or recompose.
    virtual void  Compose();


    /// @fn         void  Destroy()
    /// @brief      Delete GUI members.
    /// @details    Called only once by destructor.
    void  Destroy();


private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    FCustomButton*  mCloseButton;       ///< The close button.
    FCustomButton*  mMaximizeButton;    ///< The maximize button.
    FCustomButton*  mMinimizeButton;    ///< The minimize button.
    QColor  mActiveBackgroundColor;     ///< The active caption color.
    QColor  mInactiveBackgroundColor;   ///< The inactive caption color.


};

