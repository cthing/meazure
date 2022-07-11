# Building Meazure From Source

Meazure is a Microsoft Windows program and must be built on Windows. These
instructions describe how to build Meazure for 64-bits on Microsoft Windows
10 using Visual Studio Community 2022.

1. Download and install Visual Studio Community 2022 from <https://visualstudio.microsoft.com/>.
   Select the following components:
   * Workload - Python development
   * Workload - Desktop development with C++
   * C++ CMake tools for Windows
   * C++ MFC for latest v143 build tools (x86 & x64)

2. Install the Markdown Editor extension in Visual Studio
   <https://marketplace.visualstudio.com/items?itemName=MadsKristensen.MarkdownEditor2>

3. Meazure uses the [Conan C/C++ Package Manager](https://conan.io/) to install its library dependencies. Install
   Conan by performing the following steps:

   * In a browser go to <https://conan.io/downloads.html>
   * Download the Windows x86_64 package
   * Install the download package accepting all defaults

4. Download and install Python from <http://www.python.org/download/>. Include the following additional packages:

   * pip
   * Python launcher
   * Documentation

   Check the box to add Python to the environment variables.

5. Install the cmakelang package, which provide the cmake-format, cmake-lint and other CMake related tools:
   ```CMake
   pip install cmakelang
   ```

6. Download and install Inno Setup from <http://www.jrsoftware.org/isinfo.php>

7. Add `C:\Program Files (x86)\Inno Setup 6` to the user `Path`

8. Download and install Doxygen from <http://www.doxygen.org>

9. Download and install Graphviz from <http://www.graphviz.org/>

10. Download and install MiKTEX from <http://miktex.org/>

11. Download and install Ghostscript from <https://www.ghostscript.com/releases/gsdnld.html>

12. Open Visual Studio

13. From the `Start` menu, select `Visual Studio 2022 > x64 Native Tools Command Prompt for VS 2022`

14. In the command window change directory to the root of the Meazure source tree

15. The Meazure build uses the CMake program to configure the build environment.
    CMake can generate Makefiles and Visual Studio project files. To generate
    a command line build using Microsoft's NMake tool run the following commands at the
    root of the source tree:
    ```batchfile
    mkdir build-nmake
    cd build-nmake
    cmake -G "NMake Makefiles" ..
    ```
    To generate the solution and project files for Visual Studio, run the following commands
    at the root of the source tree:
    ```batchfile
    mkdir build-vs
    cd build-vs
    cmake -G "Visual Studio 17 2022" -A x64 ..
    ```

    **NOTE:** The first time the generator command is run, the Conan package manager will download and build the
    library dependencies (e.g. Boost). This can take a long time especially for the Visual Studio solution and will
    produce a significant amount of output.

16. To build Meazure using the command line, run the following command from the `build-nmake`
    directory:
    ```
    nmake
    ```
    The results are available in the `build-nmake/Release` directory.

17. To load Meazure into Visual Studio, open the `Meazure.sln` file
    in the `build-vs` directory.

18. To run the unit tests run the following command from the `build-nmake` directory:
    ```
    ctest
    ```
    To obtain more detail if a test fails, run:
    ```
    ctest -V
    ```

19. To create the Meazure installer, run the following command from the `build-nmake` directory:
    ```
    nmake dist
    ```

    The results are available in the `dist` top level directory.

20. To lint check the CMakeLists.txt file, run the following command from the `build-nmake` directory:
    ```
    nmake cmakelint
    ```

21. The Meazure source code is heavily commented using the Doxygen comment
    syntax. The documentation corresponding to these comments has been
    generated and checked into source control in the top level `apidocs` directory. To
    regenerate that documentation, change directories into the `build-nmake`
    directory and run the command:
    ```
    nmake apidocs
    ```

    Check in any changed documentation files.
  
