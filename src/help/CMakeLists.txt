
set(HELP_APP Meazure.hhp Meazure.hhc Meazure.hhk Meazure.css Meazure.js)
source_group("Help App" FILES ${HELP_APP})

set(IMAGES
    Graphics/AdvancedPref.png
    Graphics/Angle.gif
    Graphics/AngleDisplay.png
    Graphics/AngleIcon.gif
    Graphics/AngleMeasure.gif
    Graphics/AppIcon.gif
    Graphics/AppMinimal.png
    Graphics/AppNoMag.png
    Graphics/AppNoScreenInfo.png
    Graphics/AppNoStatus.png
    Graphics/AppNoToolbar.png
    Graphics/AppNoToolInfo.png
    Graphics/AppPositionInfo.png
    Graphics/ArbitraryAngleMeasure.gif
    Graphics/CalibrationPref.png
    Graphics/Circle.gif
    Graphics/CircleDisplay.png
    Graphics/CircleIcon.gif
    Graphics/ColorDisplay.png
    Graphics/CrossHair.gif
    Graphics/CrossHairFlash.gif
    Graphics/CrossHairSel.gif
    Graphics/Cursor.gif
    Graphics/CursorDisplay.png
    Graphics/CursorIcon.gif
    Graphics/CustomUnitsPref.png
    Graphics/DataWin.gif
    Graphics/DefaultCoords.gif
    Graphics/Grid.png
    Graphics/GridDialog.png
    Graphics/GridIcon.gif
    Graphics/HeaderLogo.gif
    Graphics/InvertedY.gif
    Graphics/KeyboardPosition.png
    Graphics/Line.gif
    Graphics/LineDisplay.png
    Graphics/LineIcon.gif
    Graphics/Magnifier.png
    Graphics/Menubar.gif
    Graphics/MoveOrigin.gif
    Graphics/MultipleScreens.gif
    Graphics/OriginMarkerDefault.gif
    Graphics/OriginMarkerInverted.gif
    Graphics/PointDisplay.png
    Graphics/PointIcon.gif
    Graphics/PositionManager.png
    Graphics/PositionSave.png
    Graphics/PrecisionPref.png
    Graphics/Rectangle.gif
    Graphics/RectDisplay.png
    Graphics/RectIcon.gif
    Graphics/RulerIcon.gif
    Graphics/Rulers.png
    Graphics/RulersPref.png
    Graphics/ScreenInfoCal.png
    Graphics/ScreenInfoSection.png
    Graphics/Sections.png
    Graphics/Sections.xcf
    Graphics/StatusBar.png
    Graphics/Toolbar.png
    Graphics/ToolInfoSection.png
    Graphics/ToolsPref.png
    Graphics/Window.png
    Graphics/WindowDisplay.png
    Graphics/WindowIcon.gif)
source_group(Images FILES ${IMAGES})

set(IMAGE_SRCS
    Graphics/CoordinateSystems.vsd)
source_group("Image Sources" FILES ${IMAGE_SRCS})
    
set(TOPICS
    Topics/AdvancedPrefs.htm
    Topics/AlwaysVisible.htm
    Topics/AngleTool.htm
    Topics/Calibration.htm
    Topics/CaptureTasks.htm
    Topics/ChangeCoordTask.htm
    Topics/CircleTool.htm
    Topics/ColorTask.htm
    Topics/Contents.htm
    Topics/Crosshair.htm
    Topics/CursorTool.htm
    Topics/CustomizationTasks.htm
    Topics/CustomLayout.htm
    Topics/CustomUnits.htm
    Topics/HideDataWinTask.htm
    Topics/HideSectionTask.htm
    Topics/InvertY.htm
    Topics/Launch.htm
    Topics/Layout.htm
    Topics/LineTool.htm
    Topics/Magnifier.htm
    Topics/MeasureAngleTask.htm
    Topics/MeasureDistanceTask.htm
    Topics/MeasureImageTask.htm
    Topics/MeasurementTasks.htm
    Topics/MeasurementToolTask.htm
    Topics/MeasurePointTask.htm
    Topics/MeasureWindowTask.htm
    Topics/MeazureOverview.htm
    Topics/Menus.htm
    Topics/MoveCrosshairTask.htm
    Topics/MoveOrigin.htm
    Topics/MultipleScreens.htm
    Topics/OnlineHelp.htm
    Topics/OriginMarker.htm
    Topics/PointTool.htm
    Topics/PositionLogging.htm
    Topics/PrecisionPrefs.htm
    Topics/Preferences.htm
    Topics/Profiles.htm
    Topics/RectTool.htm
    Topics/RulerPrefs.htm
    Topics/Rulers.htm
    Topics/SampleProfiles.htm
    Topics/SaveTask.htm
    Topics/ScreenCapture.htm
    Topics/ScreenGrid.htm
    Topics/ScreenGridSpacing.htm
    Topics/ScreenInfoSection.htm
    Topics/ScreenSizeTask.htm
    Topics/StatusBar.htm
    Topics/Toolbar.htm
    Topics/ToolInfoSection.htm
    Topics/ToolPrefs.htm
    Topics/ToolsOverview.htm
    Topics/Units.htm
    Topics/WindowTool.htm)
source_group(Topics FILES ${TOPICS})

set(TEMPLATES Templates/TopicTemplate.htm)
source_group(Templates FILES ${TEMPLATES})

if(CMAKE_BUILD_TOOL MATCHES "nmake")
    set(DEST_HELP_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
else()
    set(DEST_HELP_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")
endif()

set_source_files_properties(hid.h PROPERTIES GENERATED TRUE)

add_custom_target(apphelp
                  DEPENDS ${DEST_HELP_DIR}/Meazure.chm
                  SOURCES ${HELP_APP} ${TEMPLATES} ${TOPICS} ${IMAGES} ${IMAGE_SRCS})
add_custom_command(OUTPUT ${DEST_HELP_DIR}/Meazure.chm
                   COMMAND cmake -DHTML_HELP_COMPILER="${HTML_HELP_COMPILER}" -DHHP_FILE="Meazure.hhp" -P hhc.cmake
                   COMMAND move /Y Meazure.chm ${DEST_HELP_DIR}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   DEPENDS ${HELP_APP} ${TEMPLATES} ${TOPICS} ${IMAGES} ${CMAKE_CURRENT_SOURCE_DIR}/hid.h)
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/hid.h
                   COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/makehid/makehid.py "${PROJECT_SOURCE_DIR}/src/meazure/resource.h" hid.h
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   DEPENDS ${PROJECT_SOURCE_DIR}/src/meazure/resource.h)