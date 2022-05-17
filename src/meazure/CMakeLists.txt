
add_compile_definitions(_AFXDLL)
set(CMAKE_MFC_FLAG 2)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /nologo")

set(top_SRCS
    MeaAssert.h
    ChildView.cpp
    ChildView.h
    Colors.cpp
    Colors.h
    CommandLineInfo.cpp
    CommandLineInfo.h
    DataDisplay.cpp
    DataDisplay.h
    DataWin.cpp
    DataWin.h
    Magnifier.cpp
    Magnifier.h
    MainFrm.cpp
    MainFrm.h
    Meazure.cpp
    Meazure.h
    Meazure.rc
    Messages.h
    PositionLogDlg.cpp
    PositionLogDlg.h
    PositionLogObserver.h
    PositionSaveDlg.cpp
    PositionSaveDlg.h
    resource.h
    Units.cpp
    Units.h
    UnitsLabels.cpp
    UnitsLabels.h
)
source_group(App FILES ${top_SRCS})

set(exval_SRCS
    exval.cpp
    exval.h
)
source_group(Exval FILES ${exval_SRCS})

set(graphic_SRCS
    Circle.cpp
    Circle.h
    CrossHair.cpp
    CrossHair.h
    Graphic.cpp
    Graphic.h
    Line.cpp
    Line.h
    Rectangle.cpp
    Rectangle.h
    Ruler.cpp
    Ruler.h
)
source_group(Graphics FILES ${graphic_SRCS})

set(interface_SRCS
    ScreenProvider.h
    UnitsProvider.h
)
source_group(Interfaces FILES ${interface_SRCS})

set(manager_SRCS
    PositionLogMgr.cpp
    PositionLogMgr.h
    ProfileMgr.cpp
    ProfileMgr.h
    ScreenMgr.cpp
    ScreenMgr.h
    ToolMgr.cpp
    ToolMgr.h
    UnitsMgr.cpp
    UnitsMgr.h
)
source_group(Managers FILES ${manager_SRCS})

set(pref_SRCS
    AdvancedPrefs.cpp
    AdvancedPrefs.h
    CalibrationPrefs.cpp
    CalibrationPrefs.h
    CustomUnitsPrefs.cpp
    CustomUnitsPrefs.h
    PrecisionPrefs.cpp
    PrecisionPrefs.h
    Preferences.cpp
    Preferences.h
    RulerPrefs.cpp
    RulerPrefs.h
    ToolsPrefs.cpp
    ToolsPrefs.h
)
source_group(Preferences FILES ${pref_SRCS})

set(profile_SRCS
    FileProfile.cpp
    FileProfile.h
    Profile.cpp
    Profile.h
    RegistryProfile.cpp
    RegistryProfile.h
)
source_group(Profiles FILES ${profile_SRCS})

set(tool_SRCS
    AngleTool.cpp
    AngleTool.h
    CircleTool.cpp
    CircleTool.h
    CursorTool.cpp
    CursorTool.h
    GridTool.cpp
    GridTool.h
    LineTool.cpp
    LineTool.h
    OriginTool.cpp
    OriginTool.h
    PointTool.cpp
    PointTool.h
    RadioTool.cpp
    RadioTool.h
    RectTool.cpp
    RectTool.h
    RulerTool.cpp
    RulerTool.h
    ScreenTool.cpp
    ScreenTool.h
    Tool.cpp
    Tool.h
    WindowTool.cpp
    WindowTool.h
)
source_group(Tools FILES ${tool_SRCS})

set(utility_SRCS
    ColorDialog.cpp
    ColorDialog.h
    GUID.cpp
    GUID.h
    ImageButton.cpp
    ImageButton.h
    Label.cpp
    Label.h
    LayeredWindows.h
    Layout.cpp
    Layout.h
    NumberField.cpp
    NumberField.h
    RulerSlider.cpp
    RulerSlider.h
    Singleton.h
    StatusBar.cpp
    StatusBar.h
    Swatch.cpp
    Swatch.h
    Themes.h
    Timer.cpp
    Timer.h
    TimeStamp.cpp
    TimeStamp.h
    Utils.cpp
    Utils.h
    VersionInfo.cpp
    VersionInfo.h
    VersionNumbers.h
    XMLParser.cpp
    XMLParser.h
)
source_group(Utilities FILES ${utility_SRCS})

set(meazure_SRCS
    ${top_SRCS}
    ${exval_SRCS}
    ${graphic_SRCS}
    ${interface_SRCS}
    ${manager_SRCS}
    ${pref_SRCS}
    ${profile_SRCS}
    ${tool_SRCS}
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
