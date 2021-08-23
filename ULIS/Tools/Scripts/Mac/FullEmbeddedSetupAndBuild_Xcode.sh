set -e
cd "`dirname "$0"`"

## cd to root
cd ../../../../

## setup repository
if [ -f setup.sh ]; then
    sh setup.sh
fi

## set utility variables
ROOT=$PWD
EMB=$ROOT/ULIS/Tools/EmbeddedDependencies/
cd ULIS/Tools/

## make EmbeddedDependencies dir if not exist
if [ ! -d EmbeddedDependencies ]; then 
	mkdir EmbeddedDependencies
fi
cd EmbeddedDependencies

## clone dependencies
## clone dependencies
if [ ! -d preprocessor ]; then 
    git clone https://github.com/boostorg/preprocessor
fi

if [ ! -d glm ]; then 
    git clone https://github.com/g-truc/glm
fi

if [ ! -d Coal ]; then 
    git clone https://github.com/Robot-Fromage/Coal
fi

if [ ! -d Little-CMS ]; then 
    git clone https://github.com/Robot-Fromage/Little-CMS
fi

## build dependencies
if [ ! -d Little-CMS_Xcode ]; then 
    mkdir Little-CMS_Xcode
fi
cd Little-CMS_Xcode
cmake -G "Xcode" ../Little-CMS/.
cmake --build . --config Debug
cmake --build . --config Release

## cd to root again and write config for Xcode
cd $ROOT
rm Xcode_Config.cmake
echo "SET( ULIS_BOOST_INCLUDE_PATH        \"${EMB}preprocessor/include/\" )"                 >> Xcode_Config.cmake
echo "SET( ULIS_COAL_INCLUDE_PATH         \"${EMB}Coal/coal/Source/coal/Include/\" )"        >> Xcode_Config.cmake
echo "SET( ULIS_BUILD_SHARED              OFF )"                                           >> Xcode_Config.cmake
echo "SET( ULIS_BUILD_TESTS               OFF )"                                           >> Xcode_Config.cmake
echo "SET( ULIS_BUILD_GUIS                OFF )"                                           >> Xcode_Config.cmake
echo "SET( ULIS_QT_CMAKE_PATH             \"default\" )"                                   >> Xcode_Config.cmake
echo "SET( ULIS_GLM_INCLUDE_PATH          \"${EMB}glm/\" )"                                  >> Xcode_Config.cmake
echo "SET( ULIS_LCMS2_INCLUDE_PATH        \"${EMB}Little-CMS/include/\" )"                   >> Xcode_Config.cmake
echo "SET( ULIS_LCMS2_DEBUG_LIB_PATH      \"${EMB}Little-CMS_Xcode/Debug/lcms2d.a\" )"   >> Xcode_Config.cmake 
echo "SET( ULIS_LCMS2_RELEASE_LIB_PATH    \"${EMB}Little-CMS_Xcode/Release/lcms2.a\" )"  >> Xcode_Config.cmake 

## generate project files for Xcode
if [ -f Generate_Xcode.sh ]; then
    sh Generate_Xcode.sh
fi

## build Xcode
cd ULIS/Generated_Xcode
cmake --build . --config Debug
cmake --build . --config Release
