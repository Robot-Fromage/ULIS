// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Viewport.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
// The inclusion constraints here are rather complicated:
// We should define GLEW_STATIC here if it is not defined BEFORE including glew.
// We should include glew BEFORE including QOpenGL-related stuff.
// pybind11 includes python files, which use the "slots" name at times, which is
// also a macro used by Qt. We should also include pybind BEFORE any Qt stuff,
// but it is not always possible, so we have to temporarily disable the macro.
// Also, QOpenGLWidget indirectly includes GDI stuff on windows that define the
// RGB macro, which causes issues in ULIS for the same reason, so we simply
// undef it here.
#pragma push_macro("slots")
#undef slots
#include <pybind11/embed.h>
namespace py = pybind11;
using namespace py::literals;
#pragma pop_macro("slots")

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>

#include <QOpenGLWidget>
#undef RGB

// Forward Declaration
namespace ULIS { class FBlock; }
class SCode;
class SConsole;
class FOutputRedirectionWrapper; // FDecl stdoutput redirection wrapper for py

class SViewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    ~SViewport();
    SViewport( QWidget* iParent, SCode* iCode, SConsole* iConsole );

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
    ::ULIS::FBlock* mBitmap;
    QTimer* m_timer;
    SCode* mCode;
    SConsole* mConsole;
    FOutputRedirectionWrapper* mRedirect;
    bool mLaunched;
};

