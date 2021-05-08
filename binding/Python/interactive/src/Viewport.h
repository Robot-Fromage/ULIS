// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Viewport.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <QOpenGLWidget>

class SViewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    ~SViewport();
    SViewport( QWidget* iParent = nullptr );

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
    void keyPressEvent( QKeyEvent* event ) override;
    void keyReleaseEvent( QKeyEvent* event ) override;

private:
    void Update();
    void Render();

private:
    GLuint m_tex_id;
    GLuint m_fbo_id;
    uint8_t* m_bitmap;
    QTimer* m_timer;
};

