set -e
cd "`dirname "$0"`"
CURR=$PWD
## Call full setup & build for Xcode
if [ -f FullEmbeddedSetupAndBuild_Xcode.sh ]; then
    sh FullEmbeddedSetupAndBuild_Xcode.sh
fi

## cd to current script dir
cd $CURR

## cd to root
cd ../../../../

## set utility variables
ROOT=$PWD

## bake distro directories
mkdir -p Redist
cd Redist

mkdir -p Include
cd Include
mkdir -p ULIS
mkdir -p boost_preprocessor
mkdir -p Little-CMS
mkdir -p glm/glm
mkdir -p coal

cd ..
mkdir -p Lib
cd Lib
mkdir -p Mac
cd Mac
mkdir -p Xcode
cd Xcode
mkdir -p x64
cd x64
mkdir -p Release
mkdir -p Debug

cd ..
cd ..
cd ..
cd ..
cd ..

cp -a ${ROOT}/ULIS/Tools/EmbeddedDependencies/Coal/coal/Source/coal/Include/. ${ROOT}/Redist/Include/coal
cp -a ${ROOT}/ULIS/Tools/EmbeddedDependencies/glm/glm. ${ROOT}/Redist/Include/glm/glm
cp -a ${ROOT}/ULIS/Tools/EmbeddedDependencies/preprocessor/include/. ${ROOT}/Redist/Include/boost_preprocessor
cp -a ${ROOT}/ULIS/Tools/EmbeddedDependencies/Little-CMS/include/. ${ROOT}/Redist/Include/Little-CMS
cp -a ${ROOT}/ULIS/Source/ULIS/Include/. ${ROOT}/Redist/Include/ULIS

cp ${ROOT}/ULIS/Generated_Xcode/Debug/libULIS1.0d.a ${ROOT}/Redist/Lib/Mac/Xcode/x64/Debug/libULIS1.0d.a
cp ${ROOT}/ULIS/Generated_Xcode/Release/libULIS1.0.a ${ROOT}/Redist/Lib/Mac/Xcode/x64/Release/libULIS1.0.a
cp ${ROOT}/ULIS/Tools/EmbeddedDependencies/Little-CMS_Xcode/Debug/liblcms2d.a ${ROOT}/Redist/Lib/Mac/Xcode/x64/Debug/liblcms2d.a
cp ${ROOT}/ULIS/Tools/EmbeddedDependencies/Little-CMS_Xcode/Release/liblcms2.a ${ROOT}/Redist/Lib/Mac/Xcode/x64/Release/liblcms2.a

##cp ${ROOT}/ULIS/Tools/Scripts/Unreal/ULIS.Build.cs ${ROOT}/ULIS.Build.cs

