set -e
cd "`dirname "$0"`"

## CD 3 steps up to root/ULIS
cd ../../../

## Make Generated dir for generated project if not exist
if [ ! -d Generated_Ubuntu ]; then 
	mkdir Generated_Ubuntu
fi

## Step in Generated dir
cd Generated_Ubuntu

## Make Release dir for generated project if not exist
if [ ! -d Release ]; then 
	mkdir Release
fi

cd Release

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
cmake -G "Sublime Text 2 - Unix Makefiles" -DULIS_USE_CONFIG:BOOL=ON -DCMAKE_BUILD_TYPE=Release -DULIS_EXPLICIT_COMPILER_ID:STRING="GCC" ../../Source

cd ..
## Make Debug dir for generated project if not exist
if [ ! -d Debug ]; then 
	mkdir Debug
fi
cd Debug

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
cmake -G "Sublime Text 2 - Unix Makefiles" -DULIS_USE_CONFIG:BOOL=ON -DCMAKE_BUILD_TYPE=Debug -DULIS_EXPLICIT_COMPILER_ID:STRING="GCC" ../../Source