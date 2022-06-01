
add_compile_definitions(_AFXDLL)
set(CMAKE_MFC_FLAG 2)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /nologo")

set(TOP_SRCS
    App.cpp
    App.h
    AppFrame.cpp
    AppFrame.h
    CommandLineInfo.cpp
    CommandLineInfo.h
    Meazure.rc
    Messages.h
    resource.h
    VersionInfo.cpp
    VersionInfo.h
    VersionNumbers.h
)
source_group(App FILES ${TOP_SRCS})

set(UI_SRCS
    ui/AppView.cpp
    ui/AppView.h
    ui/ColorDialog.cpp
    ui/ColorDialog.h
    ui/DataDisplay.cpp
    ui/DataDisplay.h
    ui/DataFieldId.h
    ui/DataWin.cpp
    ui/DataWin.h
    ui/ImageButton.cpp
    ui/ImageButton.h
    ui/Label.cpp
    ui/Label.h
    ui/LayeredWindows.h
    ui/Layout.cpp
    ui/Layout.h
    ui/Magnifier.cpp
    ui/Magnifier.h
    ui/NumberField.cpp
    ui/NumberField.h
    ui/RulerSlider.cpp
    ui/RulerSlider.h
    ui/ScreenMgr.cpp
    ui/ScreenMgr.h
    ui/ScreenProvider.h
    ui/StatusBar.cpp
    ui/StatusBar.h
    ui/Swatch.cpp
    ui/Swatch.h
    ui/Themes.h
)
source_group(UI FILES ${UI_SRCS})

set(UNITS_SRCS
    units/Units.cpp
    units/Units.h
    units/UnitsLabels.cpp
    units/UnitsLabels.h
    units/UnitsMgr.cpp
    units/UnitsMgr.h
    units/UnitsProvider.h
)
source_group(Units FILES ${UNITS_SRCS})

set(POSITION_SRCS
    position/Position.cpp
    position/Position.h
    position/PositionCollection.cpp
    position/PositionCollection.h
    position/PositionDesktop.cpp
    position/PositionDesktop.h
    position/PositionLogDlg.cpp
    position/PositionLogDlg.h
    position/PositionLogMgr.cpp
    position/PositionLogMgr.h
    position/PositionLogObserver.h
    position/PositionSaveDlg.cpp
    position/PositionSaveDlg.h
    position/PositionScreen.cpp
    position/PositionScreen.h
    position/PositionLogWriter.h
)
source_group(Position FILES ${POSITION_SRCS})

set(GRAPHICS_SRCS
    graphics/Circle.cpp
    graphics/Circle.h
    graphics/Colors.cpp
    graphics/Colors.h
    graphics/CrossHair.cpp
    graphics/CrossHair.h
    graphics/Graphic.cpp
    graphics/Graphic.h
    graphics/Line.cpp
    graphics/Line.h
    graphics/Plotter.h
    graphics/Rectangle.cpp
    graphics/Rectangle.h
    graphics/Ruler.cpp
    graphics/Ruler.h
)
source_group(Graphics FILES ${GRAPHICS_SRCS})

set(PREFS_SRCS
    prefs/AdvancedPrefs.cpp
    prefs/AdvancedPrefs.h
    prefs/CalibrationPrefs.cpp
    prefs/CalibrationPrefs.h
    prefs/CustomUnitsPrefs.cpp
    prefs/CustomUnitsPrefs.h
    prefs/PrecisionPrefs.cpp
    prefs/PrecisionPrefs.h
    prefs/Preferences.cpp
    prefs/Preferences.h
    prefs/RulerPrefs.cpp
    prefs/RulerPrefs.h
    prefs/ToolsPrefs.cpp
    prefs/ToolsPrefs.h
)
source_group(Preferences FILES ${PREFS_SRCS})

set(PROFILE_SRCS
    profile/FileProfile.cpp
    profile/FileProfile.h
    profile/Profile.h
    profile/ProfileMgr.cpp
    profile/ProfileMgr.h
    profile/RegistryProfile.cpp
    profile/RegistryProfile.h
)
source_group(Profiles FILES ${PROFILE_SRCS})

set(TOOL_SRCS
    tools/AngleTool.cpp
    tools/AngleTool.h
    tools/CircleTool.cpp
    tools/CircleTool.h
    tools/CursorTool.cpp
    tools/CursorTool.h
    tools/GridTool.cpp
    tools/GridTool.h
    tools/LineTool.cpp
    tools/LineTool.h
    tools/OriginTool.cpp
    tools/OriginTool.h
    tools/PointTool.cpp
    tools/PointTool.h
    tools/RadioTool.cpp
    tools/RadioTool.h
    tools/RectTool.cpp
    tools/RectTool.h
    tools/RulerTool.cpp
    tools/RulerTool.h
    tools/ScreenTool.cpp
    tools/ScreenTool.h
    tools/Tool.cpp
    tools/Tool.h
    tools/ToolMgr.cpp
    tools/ToolMgr.h
    tools/WindowTool.cpp
    tools/WindowTool.h
)
source_group(Tools FILES ${TOOL_SRCS})

set(UTILITY_SRCS
    utilities/exval.cpp
    utilities/exval.h
    utilities/Geometry.h
    utilities/GUID.cpp
    utilities/GUID.h
    utilities/NumericUtils.h
    utilities/Registry.cpp
    utilities/Registry.h
    utilities/RegistryProvider.h
    utilities/Singleton.h
    utilities/StringUtils.cpp
    utilities/StringUtils.h
    utilities/Timer.cpp
    utilities/Timer.h
    utilities/TimeStamp.cpp
    utilities/TimeStamp.h
    utilities/XMLParser.cpp
    utilities/XMLParser.h
)
source_group(Utilities FILES ${UTILITY_SRCS})

set(MEAZURE_SRCS
    ${TOP_SRCS}
    ${UI_SRCS}
    ${GRAPHICS_SRCS}
    ${PREFS_SRCS}
    ${PROFILE_SRCS}
    ${TOOL_SRCS}
    ${UNITS_SRCS}
    ${POSITION_SRCS}
    ${UTILITY_SRCS}
)

# Replace the version variables with the actual values from the project version.
configure_file(VersionNumbers.h.in VersionNumbers.h @ONLY)

# Copy the DTD to the runtime directory so that position log loading will work when Meazure is run from
# that directory during development.
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    file(GENERATE OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/dtd/PositionLog1.dtd
                  INPUT ${SUPPORT_DIR}/dtd/PositionLog1.dtd)
else()
    configure_file(${SUPPORT_DIR}/dtd/PositionLog1.dtd ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dtd/PositionLog1.dtd COPYONLY)
endif()

add_executable(Meazure WIN32 ${MEAZURE_SRCS})
add_dependencies(Meazure apphelp libexpat hooks)
target_precompile_headers(Meazure PRIVATE pch.h)
target_link_libraries(Meazure PRIVATE libexpat hooks version.lib ${HTML_HELP_LIBRARY})
target_include_directories(Meazure PRIVATE
                           ${Boost_INCLUDE_DIRS}
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_CURRENT_BINARY_DIR}")
target_link_directories(Meazure PRIVATE ${Boost_LIBRARY_DIRS})
set_target_properties(Meazure PROPERTIES LINK_FLAGS "/MANIFEST")
