# Building Meazure From Source

Meazure is a Microsoft Windows program and must be built on Windows. These
instructions describe how to build Meazure for 64-bits on Microsoft Windows
10 using Visual Studio Community 2022.

1. Download and install Visual Studio Community 2022 from <https://visualstudio.microsoft.com/>.
   Select the following components:

   a. Workload - Python development
   b. Workload - Desktop development with C++
   c. C++ CMake tools for Windows
   d. C++ MFC for latest v143 build tools (x86 & x64)

2. Install the Markdown Editor extension in Visual Studio
   <https://marketplace.visualstudio.com/items?itemName=MadsKristensen.MarkdownEditor2>

3. Meazure uses the Boost library. To install Boost perform the following steps:

   a. Create the directory `C:\Program Files\boost`
   b. Set the `Users` persmission on the `C:\Program Files\boost` directory to `Full Control`
   c. Download <https://boostorg.jfrog.io/artifactory/main/release/1.79.0/binaries/boost_1_79_0-msvc-14.3-64.exe>
   d. Install the package into `C:\Program Files\boost\boost_<version>`

4. Edit Meazure's top level `CMakeLists.txt` file and ensure that the `BOOST_ROOT` variable is set to the
   Boost root directory. For exmample, if Boost is installed in `C:\Program Files\boost\boost_1_79_0`, the
   variable should be set to:
   ```
   set(BOOST_ROOT "C:/Program Files/boost/boost_1_79_0")
   ```

5. Download and install Python from <http://www.python.org/download/>. Include the following additional packages:

   a. pip
   b. Python launcher
   c. Documentation

   Check the box to add Python to the environment variables.

6. Install the cmakelang package, which provide the cmake-format, cmake-lint and other CMake related tools:
   ```
   pip install cmakelang
   ```

7. Download and install Inno Setup from <http://www.jrsoftware.org/isinfo.php>

8. Add `C:\Program Files (x86)\Inno Setup 6` to the user `Path`

9. Download and install Doxygen from <http://www.doxygen.org>

10. Download and install Graphviz from <http://www.graphviz.org/>

11. Download and install MiKTEX from <http://miktex.org/>

12. Download and install Ghostscript from <https://www.ghostscript.com/releases/gsdnld.html>

13. Open Visual Studio

14. From the `Start` menu, select `Visual Studio 2022 > x64 Native Tools Command Prompt for VS 2022`

15. In the command window change directory to the root of the Meazure source tree

16. The Meazure build uses the CMake program to configure the build environment.
    CMake can generate Makefiles and Visual Studio project files. To generate
    a command line build using Microsoft's NMake tool run the following commands at the
    root of the source tree:
    ```
    mkdir build-nmake
    cd build-nmake
    cmake -G "NMake Makefiles" ..
    ```
    To generate the solution and project files for Visual Studio, run the following commands
    at the root of the source tree:
    ```
    mkdir build-vs
    cd build-vs
    cmake -G "Visual Studio 17 2022" -A x64 ..
    ```

17. To build Meazure using the command line, run the following command from the `build-nmake`
    directory:
    ```
    nmake
    ```
    The results are available in the `build-nmake/Release` directory.

18. To load Meazure into Visual Studio, open the `Meazure.sln` file
    in the `build-vs` directory.

19. To run the unit tests run the following command from the `build-nmake` directory:
    ```
    ctest
    ```
    To obtain more detail if a test fails, run:
    ```
    ctest -V
    ```

20. To create the Meazure installer, run the following command from the `build-nmake` directory:
    ```
    nmake dist
    ```

    The results are available in the `dist` top level directory.

21. To lint check the CMakeLists.txt file, run the following command from the `build-nmake` directory:
    ```
    nmake cmakelint
    ```

22. The Meazure source code is heavily commented using the Doxygen comment
    syntax. The documentation corresponding to these comments has been
    generated and checked into source control in the top level `apidocs` directory. To
    regenerate that documentation, change directories into the `build-nmake`
    directory and run the command:
    ```
    nmake apidocs
    ```

    Check in any changed documentation files.
  