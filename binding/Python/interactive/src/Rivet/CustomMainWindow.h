// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CustomMainWindow.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "CustomMainWindowBase.h"
#include "CustomCaption.h"
#include <QWidget>

/////////////////////////////////////////////////////
/// @class      FCustomMainWindow
/// @brief      The FCustomMainWindow class provides a custom window widget.
/// @details    Tertiary level for custom windows.
///             Handles Custom events and provides Custom API calls
///             Users can derive this class for more customization
class  FCustomMainWindow
    : public FCustomMainWindowBase
{
    Q_OBJECT

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  FCustomMainWindow      tSelf;
    typedef  FCustomMainWindowBase  tSuperClass;

public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~FCustomMainWindow()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~FCustomMainWindow();


    /// @fn         FCustomMainWindow( QWidget* iParent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init data members.
    FCustomMainWindow( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Center Caption API
    /// @fn         FCustomCaption*  CaptionWidget()  const
    /// @brief      Getter for custom caption widget
    /// @return     A pointer to the owned member mCaptionWidget, can be null.
    FCustomCaption*     CaptionWidget()  const;


    /// @fn         QWidget*  CenterWidget()  const
    /// @brief      Getter for custom center widget
    /// @return     A pointer to the owned  member mCenterWidget, can be null.
    QWidget*            CenterWidget()  const;


    /// @fn         void  SetCaptionWidget( FCustomCaption* iCaptionWidget )
    /// @brief      Setter for the custom caption widget.
    /// @param      iCaptionWidget      The FCustomCaption* to take ownership over and place in the window.
    /// @details    The object takes ownership over the passed parameter. Can be null.
    void                SetCaptionWidget( FCustomCaption* iCaptionWidget );


    /// @fn         void  SetCenterWidget( QWidget* iCenterWidget )
    /// @brief      Setter for the custom center widget.
    /// @param      iCenterWidget       The QWidget* to take ownership over and place in the window.
    /// @details    The object takes ownership over the passed parameter. Can be null.
    void                SetCenterWidget( QWidget* iCenterWidget );


    /// @fn         void  Recompose()
    /// @brief      Recompose internal geometry.
    /// @details    Direct call to private Compose method.
    void                Recompose();


//--------------------------------------------------------------------------------------
//----------------------------------------------- Protected Non-Client OS event handling
    /// @fn         virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override
    /// @brief      Override of the Hit-Test to check if the cursor hits the caption.
    /// @details    This implementation delegates the call to the caption widget if elligible.
    /// @return     A boolean value stating the behaviour to take on caption events.
    virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override;


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
    virtual  void  resizeEvent( QResizeEvent*  event )  override;


private  slots:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- Private Qt Slots
    void  ProcessCloseClicked();
    void  ProcessMaximizeClicked();
    void  ProcessMinimizeClicked();


private:
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
    void  Compose();


    /// @fn         void  Destroy()
    /// @brief      Delete GUI members.
    /// @details    Called only once by destructor.
    void  Destroy();


private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    FCustomCaption* mCaptionWidget; ///< The FCustomCaption widget, owned but set via the accessors, can be null.
    QWidget*        mCenterWidget;  ///< The center QWidget, owned but set via the accessors, can be null.


};

