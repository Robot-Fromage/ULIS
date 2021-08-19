# Build the doc
You can build the docs by running the build_documentation.bat script in ULIS/documentation. This is an example using Windows, but the same process applies for Linux or macOS.

```d
    @echo off

    :: Ensure local scope
    setlocal

    :: Ensure current dir
    pushd %~dp0

    :: CD to root
    cd ..

    doxygen %~dp0Doxyfile

    cd documentation
    python postprocess_doc.py
```

You will need these softwares available from PATH:
    - python3
    - graphviz
    - doxygen
    
    