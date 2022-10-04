// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Caption.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "Rivet/DefaultCaption.h"
class QLabel;

class SCaption
    : public FDefaultCaption
{
    Q_OBJECT

    typedef  SCaption           tSelf;
    typedef  FDefaultCaption    tSuperClass;

public:
    ~SCaption();
    SCaption();

protected:
    void  Compose() override;

private:
    QLabel* mTitle;
};

