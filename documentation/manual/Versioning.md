# Versioning

Starting from ULIS version 4.0.0, versioning follows this pattern:

    major.minor.patch

For an extensive discussion about APIs and ABIs compatibility, check out these guidelines: [Guidelines for compatibility policies in C++](https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C++)

## Source and Binary Compatibility
##### Source Compatibility  
A library is source compatible with another version if, by switching the version, it does not introduce compile-time errors, but a recompile is necessary.
> Example: Application retains source compatibility beetween two ULIS library versions if, by switching from version 4.1.3 to 4.2.0, no compilation errors occur, and no code change is required within the application.

##### Binary Compatibility  
A library is binary compatible with another version if, by switching the version, it does not introduce link-time errors. Ultimately, if the application links dynamically to a version of the library that is binary compatible with the expected version, it is not required to recompile the application at all.
> Example: Application retains binary compatibility beetween two ULIS library versions if, by switching from version 4.1.3 to 4.2.0, no linker or runtime error occurs, once could just replace the dll in place without any issue if binary compatibility is ensured.

## Backward and Forward Compatibility
##### Backward Compatibility  
A library is backward compatible when a new version can be used in a setup where an older version is expected.
> Example: Application expects to link with ULIS4.1.3, but ULIS4.2.0 can be distributed in place.

##### Forward Compatibility  
A library is forward compatible when an old version can be used in a setup where a newer version is expected.
> Example: Application expects to link with ULIS4.1.3, but ULIS4.1.0 can be distributed in place.

## Specification of versioning increments
##### major
A major increment occurs when a change occured, and binary or source compatibility is broken.
> Example1: A class layout changed: here source compatibility is probably retained but binary compatibility is lost.

> Example2: A publicly visible class or function is removed: here both source and binary compatibility are gone.

##### minor
A minor increment occurs when a change occured, that retains backward binary compatibility and backward source compatibility.
> Example: A new publicly visible exported class is added: here binary compatibility is ensured, and source compatibility is ensured too, although a recompile might be necessary.

##### patch
A patch increment occurs when a change occured, that retains backward and forward binary compatibility, and backward and forward source compatibility.
> Example: The body of a function in a private translation unit changed: here both binary and source compatibility are ensured.

## Library Suffix
Although the versioning convention is useful for an efficient organization of the library cycles, the produced library object suffix only shows the major version number, like so:

    ULIS<major>.dll

This is because backward binary compatibility is retained with the minor increments anyway, and application developers are not supposed to seek forward compatibility anyway, as both the library and the application should go forward ideally.

## Library Details At Runtime
During runtime, wihtin the application, it can be useful to know details about the library version that is linked, since this can be different from the initial assumption.
There are several functions that are guaranteed to be available as long as binary compatibility is preserved, which are listed here:

    VersionString();        // "4.1.5"
    VersionMajorString();   // "4"
    VersionMinorString();   // "1"
    VersionPatchString();   // "5"
    VersionMajor();         // 4
    VersionMinor();         // 1
    VersionPatch();         // 5

Additionaly, more information is available about the compiled library using these functions:

    CompilerInformationString();    //
    CompilerNameString();           // "MSVC"
    CompilerVersionString();        // "1912"
    CompiledFor64Bit()              // true
    CompiledWithAVX2();             // true
    CompiledWithSSE42();            // true
    CompilationTimeStamp();         // "Aug 15 2020 15:12:04"
    FullLibraryInformationString(); // "ULIS 4.1.5 (Aug 15 2020 15:12:04) [MSVC 1912]"

## Versioning Checklist
For each increment in major, minor or patch, the appropriate value should be updated in the CMakeLists.txt at the root of the repository. It should also be updated for the documentation, the documentation should be regenerated and uploaded to the appropriate host.

