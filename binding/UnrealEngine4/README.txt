### ULIS for UE4
The **ULIS.Build.cs** file can be used to turn the **ULIS** library into an **UnrealEngine4** *third party library module*.  
Simply make a distribution with the *built binary files* and *header files* from the include folder, along with the required dependencies and copy the **ULIS.Build.cs** file to make a third party module.  

First, copy `ULIS_BuildThirdPartyModuleReleaseUE4.bat` next to the ULIS repository folder, then launch it.  

You should obtain this directory tree:
```
    •
    |_ ULIS
    |_ ULIS_BuildThirdPartyModuleReleaseUE4.bat
    |_ ULIS_ThirdPartyModuleReleaseUE4_WinMSVCx64_Redist
       |_ ULIS.Build.cs
       |_ redist
```

Copy the contents of **ULIS_ThirdPartyModuleReleaseUE4_WinMSVCx64_Redist** in a folder named **ULIS** under the third party directory for your C++ UE4 project. 

### Warning
The script builds a Release build by default, but can be configured to use a RelWithDebInfo build instead. The scripts also builds ULIS as a shared library right now. It is also possible to modify it to build a static library instead.
