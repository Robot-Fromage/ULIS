// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Caption.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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

