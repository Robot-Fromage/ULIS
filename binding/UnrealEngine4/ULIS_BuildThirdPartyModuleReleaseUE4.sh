## Put this file next to ULIS3 directory and launch to trigger full build and Redist
set -e
cd "`dirname "$0"`"
CURR=$PWD

## Check if we're next to ULIS3 directory
if [ ! -d ULIS3 ]; then 
    exit 1
fi

## Make dir for build if not exist
if [ ! -d ULIS3_FullBuildUE4_MacOsXCodex64_Redist ]; then 
    mkdir ULIS3_FullBuildUE4_MacOsXCodex64_Redist
fi

## Step in build dir
cd ULIS3_FullBuildUE4_MacOsXCodex64_Redist

## Make Generated dir for generated project if not exist
if [ ! -d Generated_XCode_Solution ]; then 
    mkdir Generated_XCode_Solution
fi

## Step in Generated dir
cd Generated_XCode_Solution

## Clean cmake garbage if there
if [ -d CMakeFiles ]; then 
	rm -r CMakeFiles 
fi
if [ -f cmake_install.cmake ]; then 
	rm cmake_install.cmake 
fi
if [ -f CMakeCache.txt ]; then 
	rm CMakeCache.txt 
fi

## Rebuild Project
cmake -G "Xcode" -DULIS3_BUILD_SHARED=ON -DULIS3_FORCE_ASSERTS=ON ../../ULIS3

## Replace RelWithDebInfo with Release if needed
cmake --build . --config RelWithDebInfo

## go up to build dir
cd ..

## Make Redist dir for generated project if not exist
if [ -d redist ]; then 
    rmdir -r redist
fi

mkdir redist
cd redist
mkdir include
cd include
mkdir ULIS3
cd ..
mkdir bin

## CD to root and copy
cd ../../
cp -a ./ULIS3/include/.                                                                         ULIS3_FullBuildUE4_MacOsXCodex64_Redist/redist/include/ULIS3
cp ULIS3_FullBuildUE4_MacOsXCodex64_Redist/Generated_XCode_Solution/RelWithDebInfo/ULIS3.dylib  ULIS3_FullBuildUE4_MacOsXCodex64_Redist/redist/bin/ULIS3.dylib
cp ULIS3/scripts/UnrealEngine4/ULIS.Build.cs                                                    ULIS3_FullBuildUE4_MacOsXCodex64_Redist/ULIS.Build.cs