# ULIS2 Benchmark
Benchmark System for ULIS2

## How-to
Create a directory structure outside of this repository anywhere on your computer as follows and copy the indicated files:
```
ULIS2_Benchmark
├── run_benchmark.py
└── bin
    ├── clang
    │   ├── Benchmark.exe
    │   ├── CPUDeviceInfo.exe
    │   ├── Formats.exe
    │   ├── GPUDeviceInfo.exe
    │   └── ULIS2.0.dll
    └── msvc
        ├── Benchmark.exe
        ├── CPUDeviceInfo.exe
        ├── Formats.exe
        ├── GPUDeviceInfo.exe
        └── ULIS2.0.dll
```
Running the `run_benchmark.py` script with python3+ will open a local web browser page displaying feedback of the benchmark data in real time. Don't close the python instance while the benchmark is running. Once the benchmark is done you can keep the output `benchmark.html` file and share it or archive it.
Copy .exe file and dlls to the appropriate bin subfolders, files should match the appropriate compiler.