
add_compile_definitions(_AFXDLL)
set(CMAKE_MFC_FLAG 2)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /nologo")

set(top_SRCS
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
source_group(App FILES ${top_SRCS})

set(ui_SRCS
    ui/AppView.cpp
    ui/AppView.h
    ui/ColorDialog.cpp
    ui/ColorDialog.h
    ui/DataDisplay.cpp
    ui/DataDisplay.h
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
    ui/StatusBar.cpp
    ui/StatusBar.h
    ui/Swatch.cpp
    ui/Swatch.h
    ui/Themes.h
)
source_group(UI FILES ${iu_SRCS})

set(units_SRCS
    units/Units.cpp
    units/Units.h
    units/UnitsLabels.cpp
    units/UnitsLabels.h    
)
source_group(Units FILES ${units_SRCS})

set(position_SRCS
    position/PositionLogDlg.cpp
    position/PositionLogDlg.h
    position/PositionLogObserver.h
    position/PositionSaveDlg.cpp
    position/PositionSaveDlg.h
)
source_group(Position FILES ${position_SRCS})

set(graphic_SRCS
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
    graphics/Rectangle.cpp
    graphics/Rectangle.h
    graphics/Ruler.cpp
    graphics/Ruler.h
)
source_group(Graphics FILES ${graphic_SRCS})

set(interface_SRCS
    ui/ScreenProvider.h
    units/UnitsProvider.h
)
source_group(Interfaces FILES ${interface_SRCS})

set(manager_SRCS
    position/PositionLogMgr.cpp
    position/PositionLogMgr.h
    profile/ProfileMgr.cpp
    profile/ProfileMgr.h
    ui/ScreenMgr.cpp
    ui/ScreenMgr.h
    tools/ToolMgr.cpp
    tools/ToolMgr.h
    units/UnitsMgr.cpp
    units/UnitsMgr.h
)
source_group(Managers FILES ${manager_SRCS})

set(pref_SRCS
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
source_group(Preferences FILES ${pref_SRCS})

set(profile_SRCS
    profile/FileProfile.cpp
    profile/FileProfile.h
    profile/Profile.cpp
    profile/Profile.h
    profile/RegistryProfile.cpp
    profile/RegistryProfile.h
)
source_group(Profiles FILES ${profile_SRCS})

set(tool_SRCS
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
    tools/WindowTool.cpp
    tools/WindowTool.h
)
source_group(Tools FILES ${tool_SRCS})

set(utility_SRCS
    utilities/exval.cpp
    utilities/exval.h
    utilities/Geometry.h
    utilities/GUID.cpp
    utilities/GUID.h
    utilities/MeaAssert.h
    utilities/NumericUtils.h
    utilities/Singleton.h
    utilities/StringUtils.cpp
    utilities/StringUtils.h
    utilities/Timer.cpp
    utilities/Timer.h
    utilities/TimeStamp.cpp
    utilities/TimeStamp.h
    utilities/Utils.cpp
    utilities/Utils.h
    utilities/XMLParser.cpp
    utilities/XMLParser.h
)
source_group(Utilities FILES ${utility_SRCS})

set(meazure_SRCS
    ${top_SRCS}
    ${ui_SRCS}
    ${graphic_SRCS}
    ${interface_SRCS}
    ${manager_SRCS}
    ${pref_SRCS}
    ${profile_SRCS}
    ${tool_SRCS}
    ${units_SRCS}
    ${position_SRCS}
    ${utility_SRCS}
)

configure_file(VersionNumbers.h.in VersionNumbers.h @ONLY)

add_executable(Meazure WIN32 ${meazure_SRCS})
add_dependencies(Meazure apphelp libexpat hooks)
target_precompile_headers(Meazure PRIVATE pch.h)
target_link_libraries(Meazure PRIVATE libexpat hooks Version.lib ${HTML_HELP_LIBRARY})
target_include_directories(Meazure PRIVATE ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}")
target_link_directories(Meazure PRIVATE ${Boost_LIBRARY_DIRS})
set_target_properties(Meazure PROPERTIES LINK_FLAGS "/MANIFEST")
