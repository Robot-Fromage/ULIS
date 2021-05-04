// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainEditorWindow.h
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
#undef near
#undef far

class SWindow
    : public QOpenGLWidget
{
    Q_OBJECT

enum eAction {
      kMoveLeft
    , kMoveRight
    , kMoveForward
    , kMoveBackward
    , kMoveUp
    , kMoveDown
    , kYawRight
    , kYawLeft
    , kPitchUp
    , kPitchDown
    , kCenterCamera
    , kMaxActions
};

public:
    ~SWindow() override;
    SWindow();

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
    virtual void keyPressEvent( QKeyEvent* event ) override;
    virtual void keyReleaseEvent( QKeyEvent* event ) override;

private:
    void Update();
    void Render();

private:
    GLuint m_tex_id;
    GLuint m_fbo_id;
    uint8_t* m_bitmap;
    uint8_t* m_volume;
    QTimer* m_timer;
    bool m_actions[kMaxActions];
    Qt::Key m_keys[kMaxActions];
};

