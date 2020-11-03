// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        TriVignette.h
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include <QWidget>

namespace ULIS { class IBlock; }
class cVignette;

/////////////////////////////////////////////////////
// cTriVignette
class cTriVignette : public QWidget
{
    Q_OBJECT

    typedef QWidget     tSuperClass;
    typedef cTriVignette   tSelf;

public:
    // Construction / Destruction
    cTriVignette( ::ULIS::IBlock* iSurface, ::ULIS::IBlock* iComp, QWidget* iParent = nullptr );
    ~cTriVignette();

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
    ::ULIS::IBlock* mSurface;
    ::ULIS::IBlock* mComp;
    cVignette* mUlisVignette;
    cVignette* mPsVignette;
    cVignette* mDiffVignette;
};

