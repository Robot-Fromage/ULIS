// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HardwareMetrics.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QTableView>

class SHardwareMetrics : public QTableView
{
    Q_OBJECT

public:
    ~SHardwareMetrics();
    SHardwareMetrics( QWidget* iParent = nullptr );

private:

};

