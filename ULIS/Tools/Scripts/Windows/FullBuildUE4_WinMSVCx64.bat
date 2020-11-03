@echo off
setlocal
pushd %~dp0

:: Call full setup & build for msvc
call FullEmbeddedSetupAndBuild_MSVC.bat

:: cd to current script dir
cd %~dp0

:: cd to root
cd ../../../../

:: set utility variables
set ROOT=%CD%

:: bake distro directories
mkdir Redist
cd Redist

mkdir Include
cd Include
mkdir ULIS
mkdir boost_preprocessor
mkdir Little-CMS
mkdir glm
mkdir glm/glm
mkdir coal

cd ..
mkdir Lib   & cd Lib
mkdir Win   & cd Win
mkdir MSVC  & cd MSVC
mkdir x64   &  cd x64
mkdir Release
mkdir Debug

cd ..
cd ..
cd ..
cd ..
cd ..
XCOPY /E /C /I /Q /H /K /Y /B "%ROOT%\ULIS\Tools\EmbeddedDependencies\Coal\coal\Source\coal\Include" "%ROOT%\Redist\Include\coal"
XCOPY /E /C /I /Q /H /K /Y /B "%ROOT%\ULIS\Tools\EmbeddedDependencies\glm\glm" "%ROOT%\Redist\Include\glm\glm"
XCOPY /E /C /I /Q /H /K /Y /B "%ROOT%\ULIS\Tools\EmbeddedDependencies\preprocessor\include" "%ROOT%\Redist\Include\boost_preprocessor"
XCOPY /E /C /I /Q /H /K /Y /B "%ROOT%\ULIS\Tools\EmbeddedDependencies\Little-CMS\include" "%ROOT%\Redist\Include\Little-CMS"
XCOPY /E /C /I /Q /H /K /Y /B "%ROOT%\ULIS\Source\ULIS\Include" "%ROOT%\Redist\Include\ULIS"

COPY /Y "%ROOT%\ULIS\Generated_VisualStudio_Solution_MSVC\Debug\ULIS1.0d.lib" "%ROOT%\Redist\Lib\Win\MSVC\x64\Debug\ULIS1.0d.lib"
COPY /Y "%ROOT%\ULIS\Generated_VisualStudio_Solution_MSVC\Release\ULIS1.0.lib" "%ROOT%\Redist\Lib\Win\MSVC\x64\Release\ULIS1.0.lib"
COPY /Y "%ROOT%\ULIS\Tools\EmbeddedDependencies\Little-CMS_VisualStudio_Solution_MSVC\Debug\lcms2d.lib" "%ROOT%\Redist\Lib\Win\MSVC\x64\Debug\lcms2d.lib"
COPY /Y "%ROOT%\ULIS\Tools\EmbeddedDependencies\Little-CMS_VisualStudio_Solution_MSVC\Release\lcms2.lib" "%ROOT%\Redist\Lib\Win\MSVC\x64\Release\lcms2.lib"

::COPY /Y "%ROOT%\ULIS\Tools\Scripts\Unreal\ULIS.Build.cs" "%ROOT%\ULIS.Build.cs"

