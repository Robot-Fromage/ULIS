###ULIS3 for UE4:
This ULIS.Build.cs file can be used to turn the ULIS3 library into an UnrealEngine4 third party library Module. Simply make a distribution with the built binaries and includes, along with the required dependencies and copy the build.cs file to make a ThirdParty module.  

First, copy `ULIS3_UE4_build_redist.bat` next to the root of the ULIS3 repository, then launch it.  

You should obtain this directory tree:
```
    •
    |_ ULIS3
    |_ ULIS3_UE4_build_redist.bat
    |_ ULIS3_FullBuildUE4_WinMSVCx64_Redist
       |_ Generated_VisualStudio_Solution_MSVC
       |_ ULIS.Build.cs
       |_ redist
```

Copy both redist folder and ULIS.Build.cs script in a folder named `ULIS` under the third party for your C++ UE4 project. 

###Warning:
The script builds a RelWithDebInfo build, not an optimal Release nor a fully qualified Debug build. That's the only way to have some amount of debug info for the third party dll withing UE4. The scripts also builds ULIS3 as a shared library only as it is. It is also possible to modify it to build a static library instead.
