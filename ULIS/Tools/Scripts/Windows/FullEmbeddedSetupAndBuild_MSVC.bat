@echo off
setlocal
pushd %~dp0

:: diagnosis
call diagnosis.bat

:: cd to root
cd ../../../../

:: setup repository
call setup.bat

:: set utility variables
set ROOT=%CD%
set EMB=%ROOT%\ULIS\Tools\EmbeddedDependencies\

:: convert backslash to forward slash for cmake
set "EMB=%EMB:\=/%"

:: cd to Tools and make EmbeddedDependencies dir
cd ULIS/Tools/
mkdir EmbeddedDependencies
cd EmbeddedDependencies

:: clone dependencies
git clone https://github.com/boostorg/preprocessor
git clone https://github.com/g-truc/glm
git clone https://github.com/Robot-Fromage/Coal
git clone https://github.com/Robot-Fromage/Little-CMS

:: build dependencies
mkdir Little-CMS_VisualStudio_Solution_MSVC
cd Little-CMS_VisualStudio_Solution_MSVC
cmake -G "Visual Studio 15 2017 Win64" ../Little-CMS/.
cmake --build . --config Debug
cmake --build . --config Release

:: cd to root again and write config for msvc
cd %ROOT%
del VisualStudio_Solution_MSVC_Config.cmake
call :writeConfig >VisualStudio_Solution_MSVC_Config.cmake

:: generate project files for msvc
call Generate_VisualStudio_Solution_MSVC.bat

:: build msvc
cd ULIS/Generated_VisualStudio_Solution_MSVC
cmake --build . --config Debug
cmake --build . --config Release

:: end of script
exit /B

:: utility to write config
:writeConfig
echo SET( ULIS_BOOST_INCLUDE_PATH        "%EMB%preprocessor/include/" )
echo SET( ULIS_COAL_INCLUDE_PATH         "%EMB%coal/coal/Source/coal/Include/" )
echo SET( ULIS_BUILD_SHARED              OFF )
echo SET( ULIS_BUILD_TESTS               OFF )
echo SET( ULIS_BUILD_GUIS                OFF )
echo SET( ULIS_QT_CMAKE_PATH             "default" )
echo SET( ULIS_GLM_INCLUDE_PATH          "%EMB%glm/" )
echo SET( ULIS_LCMS2_INCLUDE_PATH        "%EMB%Little-CMS/include/" )
echo SET( ULIS_LCMS2_DEBUG_LIB_PATH      "%EMB%Little-CMS_VisualStudio_Solution_MSVC/Debug/lcms2d.lib" )
echo SET( ULIS_LCMS2_RELEASE_LIB_PATH    "%EMB%Little-CMS_VisualStudio_Solution_MSVC/Release/lcms2.lib" )
exit /B 0
