# ULIS3 Benchmark
Benchmark System for ULIS3

## How-to
Create a directory structure outside of this repository anywhere on your computer as follows and copy the indicated files:
```
ULIS3_Benchmark
├── run_benchmark.py
└── bin
    ├── clang
    │   ├── Benchmark.exe
    │   ├── CPUDeviceInfo.exe
    │   ├── Formats.exe
    │   ├── GPUDeviceInfo.exe
    │   └── ULIS3.dll
    ├── msvc
    │   ├── Benchmark.exe
    │   ├── CPUDeviceInfo.exe
    │   ├── Formats.exe
    │   ├── GPUDeviceInfo.exe
    │   └── ULIS3.dll
    └── gcc
        ├── Benchmark.exe
        ├── CPUDeviceInfo.exe
        ├── Formats.exe
        ├── GPUDeviceInfo.exe
        └── ULIS3.dll
```
Running the `run_benchmark.py` script with python3+ will open a local web browser page displaying feedback of the benchmark data in real time.  
Don't close the python instance while the benchmark is running. Once the benchmark is done you can keep the output `benchmark.html` file and share it or archive it.  
Copy .exe file and dlls to the appropriate bin subfolders, files should match the appropriate compiler.  
The way it works is a bit weird: the Benchmark executable is launched as a subprocess from python with arguments combinations computed by the script, and passed through command the line. The Benchmark program executes itself and performs a benchmark according to the arguments, then returns the time it took to proceed as milliseconds in the integer error code.