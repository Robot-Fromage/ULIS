## Build And Install Step By Step
This page explains how to build and make a release package of ULIS from source.  

### TL;DR
```sh
cd C:/Users/PRAXINOS/dev/
git clone https://github.com/Praxinos/ULIS
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ../ULIS
cmake --build . --config Release --target ULIS
cmake --build . --config Release --target install
```

### Steps
Create a working directory somewhere on your system ( here "C:/Users/PRAXINOS/dev/" ).
![](documentation/manual/resource/image/build_install_step/0.png)

---

Start cloning the repository, we recommend doing so from the command line, in the working directory you chosed.
```sh
git clone https://github.com/Praxinos/ULIS
```
![](documentation/manual/resource/image/build_install_step/1.png)

---

The repository appears in the working directory location.
![](documentation/manual/resource/image/build_install_step/2.png)

---

You can observe the contents of the repository, it will look something like this:
![](documentation/manual/resource/image/build_install_step/3.png)

---

Now create a directory to host the generated project and build files called "build".
```sh
mkdir build
cd build
```
![](documentation/manual/resource/image/build_install_step/4.png)

---

Step in, it's empty.
![](documentation/manual/resource/image/build_install_step/5.png)

---

Start writing the command line to generate the project with cmake.
```sh
cmake -G "Visual Studio 15 2017 Win64" ../ULIS
```
![](documentation/manual/resource/image/build_install_step/6.png)

---

You can see a bunch of files were generated.
![](documentation/manual/resource/image/build_install_step/7.png)

---

You can trigger a build directly from the command line once the files are generated.
```sh
cmake --build . --config Release --target ULIS
```
![](documentation/manual/resource/image/build_install_step/8.png)

---

More files have been created if the build was succesful.
![](documentation/manual/resource/image/build_install_step/9.png)

---

In the "Release" directory, you can see the output binaries.
![](documentation/manual/resource/image/build_install_step/10.png)

---

Now we can proceed to the installation, making a release package of the lib.
```sh
cmake --build . --config Release --target install
```
![](documentation/manual/resource/image/build_install_step/11.png)

---

Back at the root of the working directory, we can see an "instal_" folder has been generated.
![](documentation/manual/resource/image/build_install_step/12.png)

---

You can see its contents to make sure it looks alright.
![](documentation/manual/resource/image/build_install_step/13.png)

---

It's rather light.
![](documentation/manual/resource/image/build_install_step/14.png)
