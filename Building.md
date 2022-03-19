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

3. Meazure uses the Boost library. It is highly recommended to build Boost from source. To build Boost
   perform the following steps:

   a. Download the latest Boost from <http://www.boost.org>
   b. Create the directory `C:\Program Files\boost`
   c. Unpack the Boost archive into the directory `C:\Program Files\boost`. This will result in the
      `C:\Program Files\boost\boot_<version>` being created.
   d. Set the `Users` persmission on the `C:\Program Files\boost` directory to `Full Control`
   e. Open a command window
   f. Build Boost by running the following commands
      ```
      cd C:\Program Files\boost\boost_<version>
      bootstrap.bat
      b2 --toolset=msvc --build-type=complete stage
      ```
   g. Wait a long time while Boost builds...

4. Edit Meazure's top level `CMakeLists.txt` file and ensure that the `BOOST_ROOT` variable is set to the
   Boost root directory. For exmample, if Boost is installed in `C:\Program Files\boost\boost_1_78_0`, the
   variable should be set to:
   ```
   set(BOOST_ROOT "C:/Program Files/boost/boost_1_78_0")
   ```

5. Download and install Python from <http://www.python.org/download/>

6. Download and install Inno Setup from <http://www.jrsoftware.org/isinfo.php>

7. Add `C:\Program Files (x86)\Inno Setup 6` to the user `Path`

8. Download and install Doxygen from <http://www.doxygen.org>

9. Download and install Graphviz from <http://www.graphviz.org/>

10. Download and install MiKTEX from <http://miktex.org/>

11. Open Visual Studio

12. From the `Start` menu, select `Visual Studio 2022 > x64 Native Tools Command Prompt for VS 2022`

13. In the command window change directory to the root of the Meazure source tree

14. The Meazure build uses the CMake program to configure the build environment.
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

15. To build Meazure using the command line, run the following command from the `build-nmake`
    directory:
    ```
    nmake
    ```
    The results are available in the `build-nmake/Release` directory.

16. To load Meazure into Visual Studio, open the `Meazure.sln` file
    in the `build-vs` directory.

17. To run the unit tests run the following command from the `build-nmake` directory:
    ```
    ctest
    ```
    To obtain more detail if a test fails, run:
    ```
    ctest -V
    ```

18. To create the Meazure installer, run the following command from the `build-nmake` directory:
    ```
    nmake dist
    ```

    The results are available in the `dist` top level directory.

19. The Meazure source code is heavily commented using the Doxygen comment
    syntax. The documentation corresponding to these comments has been
    generated and checked into source control in the top level `apidocs` directory. To
    regenerate that documentation, change directories into the `build-nmake`
    directory and run the command:
    ```
    nmake apidocs
    ```

    Check in any changed documentation files.
  