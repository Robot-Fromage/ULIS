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
- Class declaration `class FClass`
- Class end: `}; // class FClass`

#### Namespaces
|Type           |Prefix                                 |
|---------------|---------------------------------------|
|Library        |5-char max name ( `ULIS, Rivet` )      |
|Application    |**n**Example (`nWindow, nApplication`) |
- Namespace begin: `namespace nExample {`
- Namespace end: `}; // namespace nExample`
- Explicit access from Global namespace: `::nApplication::nWindow:: ... `
- Never use the `using` directive, unless in a local function or function-like scope, especially for `std::`

#### Files
- Organisation:
    - One directory per namespace.
    - One class per file.
- Naming:
    - Library + Path + Class Name + Extension ( `ULIS.Base.AlignedMemory.h` )

### Include Directives
- Use `#pragma once`
- Include directives should be sorted first by type, then by name in alphanumeric order:
    - C++ Standard Library Headers
    - C Headers
    - Library1
    - Library2
    - Application
    - Platform-Specific
- Includes within Application files should use the full path:`#include "ULIS/Base/ULIS.Base.AlignedMemory.h"`
- Includes within Application files use double quotes:`""`
- Includes withing Library files use brackets:`<>`

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
- **`typedef`** on basic primitive type can be named however you like without prefix, but avoid clustering the namespace with too much semantic typedefs.
    ```
    typedef uint32_t    uint32;     // OK
    typedef uint8_t     byte;       // Clumsy...
    typedef uint8_t     small_size; // Not OK.
    ```

- **`typedef`** on template class can be named with uppercase prefix "**F**":
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
    - This is the reason ULIS2 was made in the first place: to get rid of too many template paths generating so much symbols in the resulting binary it could weight well over 1Gb on disk.

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