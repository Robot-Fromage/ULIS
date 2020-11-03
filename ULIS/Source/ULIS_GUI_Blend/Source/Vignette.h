// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Vignette.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include <QWidget>

namespace ULIS { class IBlock; }
class QImage;
class QPixmap;
class QLabel;

/////////////////////////////////////////////////////
// cVignette
class cVignette : public QWidget
{
    Q_OBJECT

    typedef QWidget     tSuperClass;
    typedef cVignette   tSelf;

public:
    // Construction / Destruction
    cVignette( ::ULIS::IBlock* iSurface, QWidget* iParent = nullptr );
    ~cVignette();

protected:
    // Protected Qt event overrides
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    // GUI utilities
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

private:
    // Data
    ::ULIS::IBlock*         mSurface;
    QImage*                 mDisplay;
    QPixmap*                mPixmap;
    QLabel*                 mLabel;
};

