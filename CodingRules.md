# Coding Rules
The following documents provides the **Coding Rules** for this project.

#### Classes
|Type                   |Prefix                 |
|-----------------------|-----------------------|
|Model / Utility class  |**F**Class             |
|Model / Utility struct |**F**Struct            |
|GUI class              |**S**Class             |
|Abstract class         |**I**Class             |
|Template class         |**T**Class             |
|Template Abstract class|**TAbstract**Class     |

#### Namespaces
|Type           |Prefix                                 |
|---------------|---------------------------------------|
|Library        |5-char max name ( `ULIS, Rivet` )      |
|Application    |**n**Example (`nWindow, nApplication`) |
- Namespace begin: `namespace nExample {`
- Namespace end: `}; // namespace nExample`
- Explicit access from Global namespace: `::nApplication::nWindow:: ... `
- Never use the `using` directive, unless in a local function or function-like scope, especially for `std::`

### Include Directives
- Use `#pragma once`
- Include directives should be sorted first by type, then by name in alphanumeric order:
    - Application
    - Library1
    - Library2
    - C++ Standard Library Headers
    - C Headers
    - Platform-Specific
- Includes of Application files use double quotes:`""`
- Includes of Library files use brackets:`<>`

### Member Variables
- **Member variables** of class should generally be private and start with lowercase prefix "**m**":
    ```
    uint32 mCount;
    ```
- **Member variables** of struct should generally be public and be named with a short explicit name, you can ommit any prefix:
    ```
    int width;
    ```

### Typedefs
- **`typedef`** on template class instanciations can be named with uppercase prefix "**F**":
    ```
    typedef TNode< int > FNodeInt; // OK.
    ```

- **`typedef`** of a semantic nature about inheritance and object identity inside classes and structs should be defined as private as such:
    ```
    template< typename T >
    class  TAbstractNode< T > : public INode
    {
    private:
        typedef  INode                  tSuperClass;
        typedef  TAbstractNode< T >     tSelf;
    };
    ``` 

### Forward Declarations
- You should use forward declarations as much as possible:
    ```
    namespace nApplication { namespace nWindow { class SCanvas; } }
    ```

### return
- You should leave two space after a `return` statement:
    ```
    return  nullptr;
    ```

### nullptr
- Prefere `nullptr` to `NULL` or `0`
    ```
    return  nullptr;
    ```

### delete / new
- You should leave two space after a `delete` or `new` statement:
    ```
    new  int[8];
    new  SCanvas();
    delete  []  mVar;
    delete  mVar;
    ```

### Ternary Operator
- Avoid clustering too much with ternary conditional operators ( 1 level max ):
    ```
    int var1 = condition_A ? 0 : 1;                     // Yes !
    int var2 = condition_A ? condition_B ? 0 : 1 : 2;   // No !
    ```

### Spacing
- Two spaces beetween a variable type and the variable name:
- Two spaces beetween a function type and the function name:
    ```
    int  var;
    void  Foo();
    ```
- Avoid trailing spaces.

### Templates
- As powerfull as it can be, use templates sparingly as it can lead to unmanageable code quickly.

### License Header
- Doxygen style comments:
```
/**
*
*   ULIS2
*__________________
*
* @file     ULIS.Interface.Example.h
* @author   Author
* @brief    This file provides the declarations for the FExample class.
*/
```

### CRT Objects Across DLL Boundaries, CRT Safety
- Keep in mind that the library or module could be linked dynamically against other modules, libraries or programs, where it is not always possible to ensure CRT compatibility.
Don't forget to add **XXX_API** in front of classes, structs and function declarations that are meant to be part of the public API. Avoid mixing inline functions in an exported class.
Extra care is needed when using templates, when possible use explicit template instanciations and export these instanciations.
Make sure heaps don't get mixed up, when allocating memory in an implementation file, it should be deleted in an implementation file too.
Don't allocate from an inline function in a header and delete in a destructor implemented in a source either.
Avoid using threads and std members as part of the public API.
Use the private implementation idiom if needed.
Not respecting this guideling might lead to bugs that are hard to spot, so keep that in mind.

### SIMD Optimization
- If you wish to implement an optimized version of an algorithm, always make sure that a generic non optimized version is available first.
When implementing such optimizations, enclose it in preprocessor directives to ensure the host that compiles the program has compile time support for the intrinsic you wish to use.
It may be the case on one computer, but they might be missing on another compiler or version.
Before calling an optimized version of a function, granted it was compiled on a computer that supported the instrinsics, make sure the computer that runs the code also supports the intrinsic with runtime checks.
Not respecting this guideling might lead to bugs that are hard to spot, so keep that in mind.

### Intermediate Results In Non-Blocking Operations
- If the API you expose performs a non-blocking operation and uses intermediate results, the function might return and the intermediate result that lives on the stack will get deleted before the workers finished processing the task.
In that case, you should use a shared pointer to ensure all workers keep a reference to the intermediate result, until all of them are finished.
In that respect, copying intermediate results by value might be valid most of the time for basic types, but is not guaranteed to work for intrinsics data structures as they might be stored in registers and not passed the same way as other variables.
Not respecting this guideling might lead to bugs that are hard to spot, so keep that in mind.

### Type Consistency And Possible Loss Of Data.
- Solve all warnings of the form "about C4244: conversion from 'X' to 'Y', possible loss of data".

### Casts
- Always use C++ style cast such as static_cast, dynamic_cast, and such. Never use c-style casts.

### Cross-Platform
- Always target the broadest set of platforms. Never make assumptions about the target OS ( the only exception is to always target 64-bit, we don't care about 32-bit ). If some feature need platform specific implementation, use the available macros to dispatch the appropriate implementation at compile time.

### Aim For Zero Warnings
- Aim for zero compile time warnings at all times.

### Lines Limit Per File
- Whenever possible, keep the line count per file as low as possible. A file that has over 500 lines of code is considered bloated.
