# Meazure Test Plan

## Clean Installation and Startup

1. Using the ***Apps*** settings panel, ensure that the Meazure program is not installed
1. Using the ***Registry Editor***, delete the ***HKCU/SOFTWARE/C Thing Software*** key, if it exists
1. Install Meazure accepting all default settings
1. After installation is complete, verify the following:
   1. Meazure entry appears on Start menu
   1. Verify that the Meazure Start menu folder contains the following items:
      1. Meazure
      1. Meazure Help
      1. Meazure License
      1. Meazure Readme
   1. Select each Meazure Start menu item and verify that the appropriate action is performed
   1. Verify a desktop icon appears
   1. In the folder ***C:\Program Files\Meazure***, verify that the ***Profiles*** subfolder exists and is populated
      with sample profiles
   1. In the folder ***C:\Program Files\Meazure***, verify that the ***dtd*** subfolder exists and contains the
      ***PositionLog1.dtd*** file
   1. Verify that .mea profile files are displayed with the Meazure program icon
   1. Start Meazure and verify that the help menu is enabled and help can be displayed
1. Exit Meazure
1. Start Meazure
1. Minimize the program
1. Attempt to start a second copy of Meazure.
1. Verify that the first instance is restored to its original position
1. Exit Meazure
1. Uninstall the program
1. After uninstall, verify the following:
   1. Meazure entry has been removed from the Start menu
   1. Desktop icon has been removed
   1. Program folder and all subfolders have been removed
1. Using the ***Registry Editor***, delete the ***HKCU/Software/C Thing Software*** key
1. Install the previous version of Meazure
1. Start the old version
1. Exit Meazure
1. Install the latest version of Meazure accepting all defaults
1. Verify the old version has been uninstalled
1. After installation is complete verify the following:
   1. Meazure entry appears on Programs menu
   1. Desktop is present
   1. In the folder ***C:\Program Files\Meazure***, verify that the ***Profiles*** subfolder exists and is populated
      with sample profiles
1. Start Meazure and verify that the help menu is disabled. Exit Meazure

## Cursor Tool

1. Select the Cursor Tool using the toolbar
1. Verify the Status bar says ***Cursor hotspot is measurement point***
1. Verify the ***Tools*** menu indicates ***Cursor***
1. Verify the Tool Information section label reads ***Cursor***
1. Verify ***X1*** and ***Y1*** text fields are enabled but not editable
1. Verify ***X1*** and ***Y1*** up/down buttons are enabled but do not do anything
1. Verify you cannot paste a number into the ***X1*** or ***Y1*** fields
1. Verify all other fields in the Tool Information section are disabled
1. Verify the Data window appears next to cursor showing the ***X1*** and ***Y1*** coordinates of the cursor
1. Move the cursor to each corner of screen and verify that the Tool Information and Data window values agree
   with size of screen. Also verify that the Data window position changes to avoid being hidden
1. Verify the Magnifier displays a red square at the center and the view tracks cursor position

## Point Tool

1. Select Point Tool using the toolbar
1. Verify the Status bar says ***Shift locks to H or V***
1. Verify crosshair flashes and then remains red
1. Verify the ***Tools*** menu indicates ***Point***
1. Using the ***Tools*** menu, select ***Cursor***, then select ***Point*** to 
   verify functioning of Tools menu
1. Verify that the Tool Information section label reads ***Point***
1. Verify ***X1*** and ***Y1*** text fields are enabled and are editable
1. Verify all other fields in the Tool Information section are disabled
1. Move mouse over the crosshair and verify that the crosshair color changes to yellow when cursor enters 
   crosshair and changes to red when cursor leaves the crosshair
1. Verify that when the cursor moves over the crosshair, the Data window appears showing ***X1*** and ***Y1*** coordinates
1. Verify that the Data window disappears when cursor leaves the crosshair
1. Verify that hovering over the crosshair shows the tool tip ***Point 1***
1. Drag the crosshair with mouse to each corner of the screen. You may have to adjust drag point 
   to get crosshair completely into the corner. Verify Tool Information and Data window values agree 
   with size of screen.
1. Verify the Data window position changes to avoid being hidden
1. Verify crosshair is centered in Magnifier and the view tracks the crosshair position
1. Enter new ***X1*** and ***Y1*** values in the text fields, press Return and verify crosshair moves to the specified
   position
1. Paste new numerical values into the ***X1*** and ***Y1*** fields, press Return and verify the crosshair moves to the
   specified position
1. Verify that non-numeric text cannot be pastes into the ***X1*** and ***Y1*** fields
1. Use the ***X1*** and ***Y1*** up/down buttons to verify crosshair moves in the correct direction
1. Drag the crosshair while holding down ***Shift*** key and verify that only horizontal and vertical motions are allowed
1. Ensure Meazure has the keyboard focus by clicking on the Meazure window frame
1. Move the mouse cusor to a location on the screen that does not contain the crosshair
1. Press ***Ctrl+Shift+1***
1. Verify that the crosshair moves to the mouse cursor location
1. Press ***Ctrl+1*** and an arrow key
1. Verify that the crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key

## Line Tool

1. Select Line Tool using the toolbar
1. Verify the Status bar says ***Shift locks to H or V, Ctrl moves line***
1. Verify both crosshairs flash and then remain red
1. Verify the ***Tools*** menu indicates ***Line***
1. Verify the Tool Information section label reads ***Line***
1. Verify the ***X1***, ***Y1***, ***X2***, and ***Y2*** text fields are enabled and editable
1. Verify the ***W***, ***H***, ***D***, ***A***, ***As*** and ***Ar*** fields are enabled but not editable
1. Move the mouse over each crosshair and verify the crosshair color changes to yellow when cursor enters 
   crosshair and to red when cursor leaves crosshair
1. When the cursor moves over the crosshair, verify that the Data window appears showing ***X1*** and ***Y1*** coordinates and 
   the ***D*** value.
1. Verify that the Data window disappears when cursor leaves crosshair
1. Verify that hovering over the Point 1 crosshair shows the tool tip ***Point 1***
1. Verify that hovering over the Point 2 crosshair shows the tool tip ***Point 2***
1. Drag each crosshair to each corner of the screen. Verify the Tool Information and Data 
   window values agree with size of screen. Verify the Data window position changes to avoid being 
   hidden. Verify line tracks crosshair with minimal lag
1. Verify the correctness of the ***W***, ***H***, ***D***, ***A***, ***As*** and ***Ar*** values
1. Verify crosshair being dragged is centered in Magnifier and the view tracks the crosshair position
1. Enter new ***X1*** and ***Y1*** values into text fields and verify crosshair moves to specified 
   position. Repeat for ***X2*** and ***Y2***
1. Use the ***X1*** and ***Y1*** up/down buttons to verify the crosshair moves in correct direction. Repeat for ***X2***, ***Y2***
1. Drag the Point 1 crosshair while holding down ***Shift*** key. Verify that only horizontal and vertical motions 
   are allowed. Repeat for Point 2
1. Drag the Point 1 crosshair while holding down ***Ctrl*** key. Verify that the entire line moves. Move to 
   screen edges and verify figure stops at edge. Repeat for Point 2
1. Ensure Meazure has the keyboard focus by clicking on the Meazure window frame
1. Move the mouse cusor to a location on the screen that does not contain any crosshairs
1. Press ***Ctrl+Shift+1***
1. Verify that the Point 1 crosshair moves to the mouse cursor location
1. Press ***Ctrl+Shift+2***
1. Verify that the Point 2 crosshair moves to the mouse cursor location
1. Press ***Ctrl+1*** and an arrow key
1. Verify that the Point 1 crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. Press ***Ctrl+2*** and an arrow key
1. Verify that the Point 2 crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. On the ***Tools*** menu select ***Find Crosshairs*** and verify that the crosshairs flashes between 
   red and yellow a number of times and then stop flashing returning to red
1. On the ***Tools*** menu select ***Hide Crosshairs*** and verify that the crosshairs are no longer 
   displayed leaving only the line
1. On the ***Tools*** menu deselect ***Hide Crosshairs*** and verify that the crosshairs are now displayed

## Rectangle Tool

1. Select the Rectangle Tool using toolbar
1. Verify the Status bar says ***Ctrl moves rect, Ctrl+R captures region***
1. Verify both crosshairs flash and then remain red
1. Verify the ***Tools*** menu indicates ***Rectangle***
1. Verify the Tool Information section label reads ***Rectangle***
1. Verify the ***X1***, ***Y1***, ***X2***, and ***Y2*** text fields are enabled and are editable
1. Verify the ***W***, ***H***, ***D***, ***A***, ***As*** and ***Ar*** fields are enabled but not editable
1. Move the mouse over each crosshair. Verify the crosshair color changes to yellow when cursor enters 
   crosshair and then changes to red when cursor leaves crosshair
1. When cursor moves over crosshair, verify the Data window appears showing the ***X1***, ***Y1*** coordinates and 
   ***W***, ***H*** values.
1. Verify the Data window disappears when cursor leaves crosshair
1. Verify that hovering over the Point 1 crosshair shows the tool tip ***Point 1***. Repeat for Point 2.
1. Drag each crosshair to each corner of the screen and verify that the Tool Information and Data 
   window values agree with size of screen. Also verify that the Data window position changes to avoid being 
   hidden. Verify line tracks crosshair with minimal lag
1. Verify the correctness of the ***W***, ***H***, ***D***, ***A***, ***As*** and ***Ar*** values
1. Verify that the crosshair being dragged is centered in the Magnifier and the view tracks the crosshair position
1. Enter new ***X1*** and ***Y1*** values into the respective text fields and verify that the crosshair moves to the specified 
   position. Repeat for ***X2*** and ***Y2***
1. Use the ***X1*** and ***Y1*** up/down buttons to verify that the crosshair moves in the correct direction. Repeat for ***X2*** and ***Y2***
1. Drag the Point 1 crosshair while holding down ***Ctrl*** key. Verify that the entire rectangle moves. Move 
   to the screen edges and verify the figure stops at edge. Repeat for Point 2
1. Ensure Meazure has the keyboard focus by clicking on the Meazure window frame
1. Move the mouse cusor to a location on the screen that does not contain any crosshairs
1. Press ***Ctrl+Shift+1***
1. Verify that the Point 1 crosshair moves to the mouse cursor location
1. Press ***Ctrl+Shift+2***
1. Verify that the Point 2 crosshair moves to the mouse cursor location
1. Press ***Ctrl+1*** and an arrow key
1. Verify that the Point 1 crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. Press ***Ctrl+2*** and an arrow key
1. Verify that the Point 2 crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. Sweep out an area and press ***Ctrl+R***. Verify the crosshairs flash
1. Open MS Paint, Gimp or Photoshop and paste the image. Verify the image is what was captured.

## Circle Tool

1. Select the Circle Tool using the toolbar
1. Verify the Status bar says ***Ctrl moves circle, Ctrl+R captures region***
1. Verify that both crosshairs flash and then remain red
1. Verify the ***Tools*** menu indicates ***Circle***
1. Verify the Tool Information section label reads ***Circle***
1. Verify the ***X1***, ***Y1***, ***XV***, and ***YV*** text fields are enabled and are editable
1. Verify the ***W***, ***H***, ***D***, ***A*** and ***Ar*** fields are enabled but not editable
1. Verify the ***X2***, ***Y2***, and ***As*** fields is disabled
1. Move the mouse over each crosshair. Verify the crosshair color changes to yellow when cursor enters 
   crosshair and changes to red when cursor leaves crosshair
1. When the cursor moves over the crosshair on the circumference, verify the Data window appears showing 
   the ***X1*** and ***Y1*** coordinates and the ***D*** value
1. Verify the Data window disappears when cursor leaves crosshair
1. Verify that hovering over the crosshair shows the tool top ***Point 1***
1. When cursor moves over center crosshair, verify the Data window appears showing the ***XV*** and ***YV*** 
   coordinates and the ***D*** value
1. Verify the Data window disappears when cursor leaves crosshair. 
1. Verify that hovering over the crosshair shows the tool tip ***Vertex***
1. Set the magnifier to 6X magnification
1. Drag the Point 1 crosshair so that the entire circle can be see in the magnifier.
1. Verify the Tool Information and Data window values agree with size of circle as displayed in the magnifier grid.
1. Set the magnifier to 2X magnification
1. Verify the radius and circle lines track the crosshairs with minimal lag
1. Verify the correctness of the ***W***, ***H***, ***D***, ***A*** and ***Ar*** values
1. Verify that the crosshair being dragged is centered in the Magnifier and the view tracks crosshair position
1. Enter new ***X1*** and ***Y1*** values into text fields and verify crosshair moves to specified 
   position. Repeat for ***XV*** and ***YV***
1. Use the ***X1*** and ***Y1*** up/down buttons to verify crosshair moves in the correct direction. Repeat for ***XV*** and ***YV***
1. Drag the Point 1 crosshair while holding down ***Ctrl*** key and verify that the entire circle moves. Move to 
   the screen edges and verify the figure stops at the edge. Repeat for Vertex
1. Ensure Meazure has the keyboard focus by clicking on the Meazure window frame
1. Move the mouse cusor to a location on the screen that does not contain any crosshairs
1. Press ***Ctrl+Shift+1***
1. Verify that the Point 1 crosshair moves to the mouse cursor location
1. Press ***Ctrl+Shift+3***
1. Verify that the center crosshair moves to the mouse cursor location
1. Press ***Ctrl+1*** and an arrow key
1. Verify that the Point 1 crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. Press ***Ctrl+3*** and an arrow key
1. Verify that the center crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. Sweep out an area and press ***Ctrl+R***. Verify the crosshairs flash
1. Open MS Paint, Gimp or Photoshop and paste the image. Verify the image is what was captured.

## Angle Tool

1. Select the Angle Tool using toolbar
1. Verify the Status bar says ***Shift locks to H or V, Ctrl moves angle***
1. Verify all three crosshairs flash and then remain red
1. Verify the ***Tools*** menu indicates ***Angle***
1. Verify the Tool Information section label reads ***Angle***
1. Verify ***X1***, ***Y1***, ***X2***, ***Y2***, ***XV***, and ***YV*** text fields are enabled and editable.
1. Verify the ***A*** field is enabled but not editable
1. Verify the ***W***, ***H***, ***D***, ***As***, and ***Ar*** fields are disabled
1. Move the mouse over each crosshair. Verify the crosshair color changes to yellow when cursor enters 
   crosshair and changes to red when cursor leaves crosshair
1. When the cursor moves over each crosshair, verfiy the Data window appears showing labels and 
   values ***X1***, ***Y1***, ***A*** or ***X2***, ***Y2***, ***A*** for the outer crosshairs and ***XV***, ***YV***, ***A*** for the vertex crosshair. 
1. Verify the Data window disappears when cursor leaves crosshair
1. Verify that hovering over crosshair shows the tool tip ***Point 1*** or ***Point 2*** for the outer crosshairs and
   ***Vertex*** for the vertex crosshair
1. Press the ***Shift*** key and drag an outer crosshair to form a vertical line. Verify the X 
   coordinates are identical and the angle is 180 degrees. Verify that the angle bisector is horizontal.
1. Repeat the above but set a horizontal line and verify the angle is –180.0 degrees, all Y positions are 
   identical and the angle bisector is vertical
1. Press the ***Shift*** key to set a 90 degree angle. Verify the displayed angle value and the angle bisector 
   is a 45 degree angle in the acute portion of the angle
1. Enter new ***X1*** and ***Y1*** values into the respective text fields and verify the crosshair moves to the specified 
   position. Repeat for Point 2 and the vertex
1. Use the ***X1*** and ***Y1*** up/down buttons to verify the crosshair moves in the correct direction. Repeat for
   ***X2***, ***Y2*** and ***XV***, ***YV***
1. Drag the Point 1 crosshair while holding down the ***Ctrl*** key. Verify the entire angle moves. Move to 
   the screen edges and verify the figure stops at edge. Repeat for Point 2 and the Vertex.
1. Ensure Meazure has the keyboard focus by clicking on the Meazure window frame
1. Move the mouse cusor to a location on the screen that does not contain any crosshairs
1. Press ***Ctrl+Shift+1***
1. Verify that the Point 1 crosshair moves to the mouse cursor location
1. Press ***Ctrl+Shift+2***
1. Verify that the Point 2 crosshair moves to the mouse cursor location
1. Press ***Ctrl+Shift+3***
1. Verify that the vertex crosshair moves to the mouse cursor location
1. Press ***Ctrl+1*** and an arrow key
1. Verify that the Point 1 crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. Press ***Ctrl+2*** and an arrow key
1. Verify that the Point 2 crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key
1. Press ***Ctrl+3*** and an arrow key
1. Verify that the vertex crosshair moves in the direction corresponding to the arrow key being pressed
1. Repeat for each arrow key

## Window Tool

1. Select the Window Tool using toolbar. Verify toolbar receives a red border
1. Verfiy the Status bar says ***Ctrl+R captures window***
1. Verify ***Tools*** menu indicates ***Window***
1. Verify the Tool Information section label reads ***Window***
1. Verify ***X1***, ***Y1***, ***X2***, ***Y2***, ***W***, ***H***, ***D***, ***A***, ***As*** and ***Ar*** are enabled but not editable
1. Verify the ***XV*** and ***YV*** fields are disabled
1. Verify the Data window appears next to the window border showing the ***W*** and ***H*** values. 
1. Move the cursor around the screen and verify the red border is applied to windows
1. Verify the correctness of the ***X1***, ***Y1***, ***X2***, ***Y2***, ***W***, ***H***, ***D***, ***A***, ***As*** and ***Ar*** values
1. Move the cursor to a relatively small window and press ***Ctrl+R***.
1. Open MS Paint, Gimp or Photoshop and paste the image. Verify image is of the window.

## Ruler Tool

1. Select the Ruler Tool using toolbar.
1. Verify rulers appear at the top and left edges of all screens
1. Verify the ***Tools*** menu indicates ***Rulers*** are selected
1. Select the Cursor Tool and verify that the ruler indicators properly track with the position of 
   the cursor and that the coordinates match the indicator location
1. Select the Point Tool and verify indicators properly track the crosshair
1. Select the Line Tool and verify two indicators are displayed on each ruler, one for each 
   crosshair. Verify each indicator properly tracks the crosshairs
1. Select the Angle Tool and verify three indicators are displayed on each ruler, one for each 
   crosshair. Verify each indicator properly tracks the crosshairs
1. Use the mouse pointer to drag each ruler and verify that they move across the screen.
1. Verify that the ruler is partially transparent. 
1. Deselect the ruler tool using the ***Tools*** menu and verify the rulers are no longer displayed

## Grid Tool

1. Select the Grid Tool using the toolbar.
1. Verify a grid appears covering the entire screen.
1. Verify the ***Tools*** menu indicates the ***Screen Grid*** is selected
1. On the ***Tools*** menu select ***Screen Grid Spacing***
1. Uncheck the ***Link horizontal and vertical*** check box
1. Enter new values for the spacing in the text fields
1. Press OK
1. Verify the new spacing using the Point Tool
1. On the ***Tools*** menu select ***Screen Grid Spacing***
1. Use the up and down arrows next to each text field to adjust the grid spacing.
1. Press OK
1. Verify the grid dimensions using the Point Tool
1. On the ***Tools*** menu select ***Screen Grid Spacing***
1. Check the ***Link horizontal and vertical*** check box.
1. Verify that the Vertical text field value changes to match the Horizontal text field value and that the grid becomes square
1. Enter the minimum grid spacing and verify the grid is properly drawn
1. Enter the maximum grid spacing and verify the grid is properly drawn
1. Press ***OK*** and verify grid remains on the screen
1. On the ***Tools*** menu select ***Screen Grid Spacing***
1. Change the horizontal spacing
1. Press ***Cancel*** and verify that the original horizontal spacing is restored
1. Deselect the Grid Tool using the ***Tools*** menu
1. Verify the grid is removed from the screen
1. On the ***Tools*** menu select ***Screen Grid Spacing***.
1. Verify the dialog and screen grid appear
1. Press ***Cancel***
1. Verify the screen grid is removed from the screen

## Resolution Calibration

1. Verify that the calibration warning button is displayed on the Screen Information section
1. Press the calibration warning button
1. Verify that the calibration preference panel is displayed.
1. Verify that the frame title displays ***Resolution for Screen 1 (primary)***
1. Drag the dialog to the second screen and verify that the frame title changes to ***Resolution for Screen 2***.
1. Verify that ***Use operating system value*** is selected
1. Press ***OK*** to close the dialog
1. Record the pixel width and height of the screen shown in the Screen Information section
1. On the Units menu select ***Inches***
1. A warning dialog should be displayed indicating that the resolution should be calibrated. 
1. Press ***Continue*** to dismiss the dialog
1. Record the inch width and height of the screen shown in the Screen Information section
1. On the ***Edit*** menu select ***Preferences*** to display the preferences dialog.
1. Select the ***Calibration*** tab
1. Select ***Set manually*** and ***in***
1. Enter a vertical resolution in the Ry text field that is half the current resolution
1. Verify that the screen height displayed in the ***H*** text field is twice the value previously recorded
1. Verify that the vertical slider corresponds to the entered resolution by clicking on the end of 
   the slider
1. Press ***Apply***
1. Verify that the screen height displayed on the Screen Information section is twice the value previously
   recorded.
1. Verify that the vertical resolution shown in the ***Ry*** text field matches that entered on the Calibration panel
1. Verify that the calibration warning button is still displayed on the Screen Information section
1. Enter a horizontal resolution in the ***Rx*** text field that is half the current resolution
1. Verify that the screen width displayed in the ***W*** text field is twice the value previously recorded
1. Verify that the horizontal slider corresponds to the entered resolution by clicking on the end 
   of the slider
1. Press ***Apply***
1. Verify that the screen width displayed on the Screen Information section is twice the value previously recorded.
1. Verify that the horizontal resolution shown in the ***Rx*** text field matches that entered on the Calibration panel
1. Using a handheld ruler set the vertical and horizontal resolutions using the slides so that 
   they point to the 1 inch mark. The values displayed in the ***W*** and ***H*** text fields should be 
   close to the screen dimensions. Verify this by measuring the screen dimensions with the ruler
1. Enter into the ***W*** and ***H*** text fields the screen dimensions measured using the ruler 
1. Drag the dialog to the primary screen and repeat manual calibration.
1. Drag the dialog back and forth between screens and verify that the correct calibration 
   setting are displayed on each screen.
1. Press ***OK*** to close the dialog
1. Verify that the resolution warning button is no longer displayed in the Screen Information section.
1. Reopen the Calibration preferences dialog.
1. Select ***Use operating system value*** for both screens.
1. Press ***OK***.
1. Verify the screen size and resolution return to the values previously recorded
1. Verify the resolution warning button is displayed

## Standard Units

1. Using the toolbar select the Point Tool
1. Using the toolbar turn on the Rulers
1. On the ***Units*** menu ensure that ***Pixels*** is selected
1. Verify that the Tool and Screen Information sections display pixels for units
1. Verify the Rulers display pixel values
1. Move the cursor over the crosshair and verify that the Data window displays pixel values
1. Record the width, height and resolution of the screen from the Screen Information section
1. Select the Rectangle Tool and record all values from the Tool Information section
1. On the ***Units*** menu select ***Twips***.
1. Verify that the Tool and Screen Information sections display twips for units
1. Verify the Rulers display twips values
1. Move the cursor over the crosshair and verify that the Data window displays twip values
1. Using the pixel values recorded previously, convert the values to twips (twips = pixels / 
   resolution * 1440) and verify that these are the values displayed in the Screen and Tool 
   Info sections
1. On the ***Units*** menu select ***Points***.
1. Verify that the Tool and Screen Information sections display points for units
1. Verify the Rulers display point values
1. Move the cursor over the crosshair and verify that the Data window displays point values
1. Using the pixel values recorded previously, convert the values to points (points = pixels / 
   resolution * 72) and verify that these are the values displayed in the Screen and Tool Information 
   sections
1. On the ***Units*** menu select ***Picas***.
1. Verify that the Tool and Screen Information sections display picas for units
1. Verify the Rulers display pica values
1. Move the cursor over the crosshair and verify that the Data window displays pica values
1. Using the pixel values recorded previously, convert the values to picas (picas = pixels / 
   resolution * 6) and verify that these are the values displayed in the Screen and Tool Information 
   sections
1. On the ***Units*** menu select ***Inches***.
1. Verify that the Tool and Screen Information sections display inches for units
1. Verify the Rulers display inch values
1. Move the cursor over the crosshair and verify that the Data window displays inch values
1. Using the pixel values recorded previously, convert the values to inches (inches = pixels / 
   resolution) and verify that these are the values displayed in the Screen and Tool Information 
   sections
1. On the ***Units*** menu select ***Centimeters***.
1. Verify that the Tool and Screen Information sections display centimeters for units
1. Verify the Rulers display centimeter values
1. Move the cursor over the crosshair and verify that the Data window displays centimeter values
1. Using the pixel values recorded previously, convert the values to centimeters (centimeters 
   = pixels / resolution * 2.54) and verify that these are the values displayed in the Screen and 
   Tool Information sections
1. On the ***Units*** menu select ***Millimeters***.
1. Verify that the Tool and Screen Information sections display millimeters for units
1. Verify the Rulers display millimeter values
1. Move the cursor over the crosshair and verify that the Data window displays millimeter values
1. Using the pixel values recorded previously, convert the values to millimeters (millimeters = 
   pixels / resolution * 25.4) and verify that these are the values displayed in the Screen and 
   Tool Information sections
1. Using the toolbar deselect the Ruler
1. Select the Angle tool
1. On the ***Units*** menu select ***Degrees***
1. Press the ***Shift*** key and move the Angle tool to a 90 degree configuration
1. Verify that the angle display reads 90 degrees
1. On the ***Units*** menu select ***Radians***
1. Verify that the angle display reads 1.571 (i.e. pi / 2)
1. On the ***Units*** menu select ***Pixels*** and ***Degrees***

## Custom Units

1. On the ***Units*** menu ensure that there is a disabled item labeled ***[custom]***
1. On the ***Units*** menu select the item labeled ***Define Custom***
1. Verify that the Preferences dialog is displayed with the ***Custom Units*** tab selected
1. Verify that all items on the panel are disabled except the ***Name*** and ***Abbreviation*** labels and text fields
1. In the ***Name*** field enter ***Miles***
1. In the ***Abbreviation*** field enter ***mi***
1. Verify that all items on the panel are now enabled
1. Ensure that the pixel based factor is selected and enter 0.5
1. Press ***OK***
1. On the ***Units*** menu verify that the ***[custom]*** item has been replaced by an enabled item 
   named ***Miles*** and select that item
1. Verify that ***mi*** is displayed as appropriate on all app sections
1. Verify that for every one pixel moved, the position value changes by 2 mi
1. On the ***Edit*** menu select ***Preferences***
1. Select the ***Custom Units*** tab
1. Change the factor to inch based and set 0.5
1. Press ***OK***
1. Using a ruler, verify that for a change in position of 1 inch, the position value changes by 2 mi
1. On the ***Edit*** menu select ***Preferences***
1. Select the ***Custom Units*** tab
1. Change the factor to centimeter based and set 0.5
1. Press ***OK***
1. Using a ruler, verify that for a change in position of 1 centimeter, the position value changes by 2 mi
1. On the ***Edit*** menu select ***Preferences***
1. Select the ***Custom Units*** tab
1. Press the ***Set Display Precision*** button
1. Verify that the Precision panel is shown with the custom units radio button selected
1. Return to the Custom Units panel and press the ***Clear Units*** button
1. Verify that the ***Name*** and ***Abbreviation*** fields are cleared and all other items on the panel are disabled
1. Press ***OK***

## Coordinate System

1. Select the Cursor Tool
1. Move the cursor to the upper left corner and verify X1/Y1 are both 0
1. Verify that the origin marker is displayed at the 0, 0 point and that its Y axis points down 
   from the origin.
1. Verify the ***View*** menu ***Invert Y*** item is not checked
1. On the ***View*** menu select ***Invert Y***
1. Verify that the origin marker moves to the bottom left corner of the screen and the marker's 
   Y axis points up from the origin.
1. Move the cursor to the upper left corner and verify ***X1*** is 0 and ***Y1*** is H - 1
1. Move the cursor to the lower left corner and verify ***X1*** and ***Y1*** are both 0
1. Move the cursor to the lower right corner of the screen and press ***Ctrl+A***.
1. Verify ***X1*** and ***Y1*** are both 0
1. Verify the origin marker moves to the new 0, 0 position.
1. Move the cursor to the upper left corner and verify that ***X1*** is -(W - 1) and ***Y1*** is (H - 1)
1. On the ***View*** menu deselect ***Invert Y***,
1. Move the cursor to the upper left corner of the screen and verify that ***X1*** is -(W - 1) and ***Y1*** is -(H - 1)
1. On the ***View*** menu select ***Reset Origin***
1. Move the cursor to the upper left corner of the screen and verify that ***X1*** and ***Y1*** are both 0

## Angle Display

1. Verify that the ***View*** menu ***Supplemental Angle*** item is not checked
1. Select the Line Tool
1. Move the point 2 crosshair and verify the included angle is displayed
1. On the ***View*** menu select ***Supplemental Angle***
1. Verify the angle display shows the supplement of the included angle
1. Move the point 2 crosshair and verify the supplemental angle is displayed
1. Verify that the ***View*** menu ***Supplemental Angle*** item is checked
1. On the ***View*** menu select ***Supplemental Angle***
1. Verify the angle display shows the included angle

## Screen Info

1. Using the Display Control Panel record the screen sizes for each screen attached to the system.
1. Select the Cursor tool
1. Drag Meazure to the primary display screen and verify that the Screen Information section 
   frame displays ***Screen 1 (primary)***
1. Verify that the screen size for the primary display matches that reported in the Screen 
   Information section
1. Drag Meazure to the second display screen and verify that the Screen Information section 
   frame displays ***Screen 2***
1. Verify that the screen size for the secondary display matches that reported in the Screen 
   Information section.
1. Close the Meazure program

## High DPI Screen Scaling

1. On a 4K display (i.e. 3840 x 2160 pixels) set the screen scaling to 150%
    1. Bring up the ***Settings > System > Display*** dialog
    1. Set the display resolution to 3840 x 2160
    1. Set the ***Change the size of text, apps, and other items*** to 150%
    1. Close the dialog
1. Start the Meazure program
1. Verify that the user interface is properly arranged with correct margins, spacing and size of elements
1. Verify that the crosshairs are properly sized
1. Verify that the screen dimensions and resolution are correct
1. Select various measurement tools and verify that they are properly displayed and dragable
1. Close the Meazure program
1. Bring up the ***Settings > System > Display*** dialog
1. Set the ***Change the size of text, apps, and other items*** to 100%

## Tool Preferences

1. Start the Meazure program
1. On the toolbar select the Line Tool
1. Move the cursor over the crosshairs and verify that the crosshairs and Data windows are 
   partially transparent.
1. On the ***Edit*** menu select ***Preferences*** to display the preferences dialog.
1. Select the ***Tools*** tab
1. Press the ***Color*** button in the Line Color area and select a new color for lines.
1. Press ***OK***
1. Press ***Apply***
1. Verify the Line Tool line color changes to the new color
1. Press the ***Background*** button in the Crosshair Colors area and select a new color for the 
   crosshair background.
1. Press ***OK***
1. Press ***Apply***
1. Verify the background color changes to the new color on both crosshairs
1. Press the ***Border*** button in the Crosshair Colors area and select a new color for the 
   crosshair border.
1. Press ***OK***
1. Press ***Apply***
1. Verify the border color changes to the new color on both crosshairs
1. Press the ***Highlight*** button in the Crosshair Colors area and select a new color for the 
   crosshair highlight.
1. Press ***OK***
1. Uncheck ***Show origin marker***
1. Press ***Apply***
1. Verify the origin marker is no longer displayed
1. Verify that the opacity slider is enabled and change the opacity to 100%
1. Press ***OK***
1. Move the cursor over a crosshair and verify that the crosshair and Data window are no 
   longer transparent.
1. On the ***Edi***t menu select ***Preferences*** to display the preferences dialog.
1. Select the ***Tools*** tab
1. Change the opacity to 80%
1. Uncheck ***Show popup data windows***
1. Press ***OK***
1. Move the cursor over the crosshairs and verify the highlight color is the new color. Also 
   verify that the Data windows no longer appear
1. On the ***Edit*** menu select ***Preferences*** to display the preferences dialog.
1. Select the ***Tools*** tab
1. Press all ***Default*** buttons
1. Check the ***Show popup data windows*** and ***show origin marker*** boxes
1. Press ***OK***
1. Verify all Line Tool colors have returned to the default colors
1. Verify the Data window once again appears when the cursor is placed over the crosshairs
1. Verify the origin marker is shown

## Ruler Preferences

1. On the toolbar select the Rulers
1. Verify that the rulers are partially transparent
1. On the ***Edit*** menu select ***Preferences*** to display the preferences dialog.
1. Select the ***Rulers*** tab
1. Verify that the opacity slider is enabled
1. Change the opacity to 100%.
1. Press ***OK***
1. Verify that the rulers are no longer partially transparent
1. On the ***Edit*** menu select ***Preferences***
1. Select the ***Rulers*** tab
1. Set the opacity to 80%
1. In the Colors area press the ***Background*** button and select a new color for the ruler background
1. Press ***OK***
1. Press ***Apply***
1. Verify that the background color on the rulers change to the new color
1. In the Colors area press the ***Border*** button and select a new color for the ruler border
1. Press ***OK***
1. Press ***Apply***
1. Verify that the border color on the rulers change to the new color
1. Press all ***Default*** buttons
1. Press ***OK***
1. Verify that all ruler colors return to their defaults
1. On the toolbar deselect the Rulers

## Precision Preferences

1. On the ***Edit*** menu select the ***Preferences*** item
1. Select the ***Precision*** tab
1. For each set of units and each measurement type, set a non-default number of decimal 
   places in such a way that each measurement type has a different number of decimal 
   places. Do a few at a time
1. Verify that the up/down buttons function properly
1. Verify that only integers between 0 and 6 inclusive can be set
1. Press ***OK***
1. Verify that for each set of units the number of decimal places corresponds to those set
1. Repeat the last two steps as needed to cover all units and measurement types
1. Change all units and measurement types from their default values
1. Press the ***Default*** button
1. Verify that all values return to their defaults for all units and measurement types
1. Press ***OK*** to dismiss the dialog

## Magnifier

1. On the toolbar select the Point Tool
1. Verify that the crosshair should be visible in the magnifier window.
1. On the toolbar select the Cursor Tool
1. Verify that a red square is displayed at the center of the magnifier window
1. Move the Zoom slider to ***6X***
1. Verify that a grid is now displayed in the magnifier window
1. On the ***View*** menu select ***Zoom Out***
1. Verify that the magnification goes to ***4X***
1. Press ***Ctrl++***
1. Verify that the magnification goes to ***6X***
1. On the ***View*** menu deselect the ***Magnifier Grid*** menu item
1. Verify that a grid is no longer displayed in the magnifier window
1. Press the ***Pause*** button and move the cursor.
1. Verify that the magnifier window image does not change
1. Press the ***Pause*** button again
1. Verify that the image in the magnifier window changes as the cursor is moved
1. On the toolbar select the Point Tool
1. On the ***View*** menu select ***Color Format*** and ensure that ***RGB*** is selected
1. Verify that the color values and swatch reflect the color of the pixel in the red square at the 
   center of the magnifier window
1. Select all text in the color field
1. Copy the text using the ***Edit*** menu ***Copy*** item
1. Paste the contents of the clipboard into a text editor
1. Verify the pasted text is the same as what was copied
1. On the ***View*** menu select ***Color Format*** and ***#RRGGBB***
1. Verify that the color values are now reported in ***#RRGGBB*** format
1. Press the clipboard button next to the color swatch
1. Paste the contents of the clipboard into a text editor
1. Verify the pasted text is the same as that shown in the color field
1. Repeat for all other color space formats
1. On the ***View*** menu select ***Color Format*** and ***Extended Name***
1. Verify that the color names are now reported
1. Verify that the color swatch shows the matched color
1. Press the clipboard button next to the color swatch
1. Paste the contents of the clipboard into a text editor
1. Verify the pasted text is the same as that shown in the color field
1. On the ***View*** menu select ***Color Format*** and ***Extended #RRGGBB***
1. Verify that hex colors representing the matched color are now reported
1. Verify that the color swatch shows the matched color
1. Press the clipboard button next to the color swatch
1. Paste the contents of the clipboard into a text editor
1. Verify the pasted text is the same as that shown in the color field
1. Repeat for the Basic web color matches

## Master Reset

1. Select the Rectangle Tool
1. on the ***View*** menu select ***Invert Y***
1. on the ***View*** menu select ***Supplemental Angle***
1. Select the rulers and screen grid
1. Change the screen grid spacing
1. Using the preferences dialog panels change the line color, the crosshair colors, the 
   opacities, and the display precisions
1. Define a set of custom units
1. Change the pixel color format to ***CMY***
1. Change the magnifier zoom factor to ***6X***
1. On the Preferences dialog select the ***Advanced*** tab
1. Press the ***Reset*** button
1. Press ***No*** on the confirmation dialog
1. Verify that the configuration has not changed
1. On the Preferences dialog select the ***Advanced*** tab
1. Press the ***Reset*** button
1. Press ***Yes*** on the confirmation dialog
1. Verify that the configuration is restored to the default

## Visibility

1. Verify that the program window cannot be obscured by any other window on the screen
1. On the ***View*** menu deselect the ***Always Visible*** item
1. Verify that the program window can be obscured
1. On the ***View*** menu select the ***Always Visible*** item
1. On the ***View*** menu individually deselect the ***Toolbar***, ***Tool Information***, ***Screen Information***,
   ***Magnifier***, and the ***Status Bar*** items. Verify that the sections of the program are hidden as their respective 
   menu items are deselected
1. On the ***View*** menu individually select the ***Toolbar***, ***Tool Information***, and ***Screen Information*** items. Verify 
   that these sections appear
1. On the ***View*** menu select ***Collapse***
1. Verify that all sections except the menu bar are hidden
1. On the ***View*** menu select ***Expand***
1. Verify that the Toolbar, Tool Information and Screen Information section reappear
1. On the ***View*** menu select the ***Magnifier*** and ***Status Bar*** items.
1. Verify these sections appear

## Persistence

1. Select the Window Tool and Ruler Tool
1. On the ***Edit*** menu select the ***Preferences*** item
1. Select the ***Calibration*** tab.
1. Select the manual resolution setting, change the vertical resolution to 12 and the horizontal resolution to 24
1. Select the ***Rulers*** tab.
1. Change all colors.
1. Select the ***Tools*** tab.
1. Change all colors and deselect the popup Data window.
1. Press ***OK***.
1. On the ***View*** menu select the ***Invert Y*** item
1. On the ***View*** menu deselect the ***Magnifier*** item
1. Move the program window and use the Window Tool to measure the position of the 
   program window.
1. Record the ***X1*** and ***Y1*** values
1. Exit the program
1. Restart the program
1. Verify that all previous settings have been restored
1. Using the Window Tool verify that the program window ***X1*** and ***Y1*** position matches the values 
   previously recorded
1. Exit the program
1. Using the Registry Editor program delete the ***HKCU/Software/C Thing Software*** key

## Profiles

1. Start the program
1. Select the Point Tool and Ruler Tool
1. On the ***File*** menu select ***Save Profile*** and choose a filename. Complete the save
1. Turn off the Rulers
1. Select the Cursor Tool
1. On the ***File*** menu select ***Load Profile*** and choose the file that was previously saved
1. Verify that the Point Tool and Ruler Tool are now selected
1. Turn off the Rulers
1. Select the Cursor Tool
1. Exit the program
1. Double click the profile file
1. Verify that Meazure is launched with the Point Tool and Ruler Tool selected
1. Turn off the Rulers
1. Select the Cursor Tool
1. Exit the program
1. Drag the profile file and drop it on the Meazure program icon.
1. Verify that Meazure is launched with the Point Tool and Ruler Tool selected
1. On the ***Edit*** menu select ***Preferences***
1. Choose the ***Advanced*** tab
1. In the Startup Profile area use the ***…*** button to find the previously saved profile.
1. Press ***OK***
1. Turn off the Rulers
1. Select the Cursor Tool
1. Exit the program
1. Start the program
1. Verify that the Point Tool and Ruler Tool are selected
1. Exit the program
1. Using the Registry Editor program delete the ***HKCU/Software/C Thing Software*** key

## Position Logging

1. Start Meazure
1. Select the Cursor Tool
1. On the ***Units*** menu, select ***Pixels***
1. Make sure that the Meazure program has the focus
1. Move to a number of positions and at each one write down or remember the position and 
   at each position press Ctrl+P to record the position
1. Verify that each time ***Ctrl+P*** is pressed the Data window should flash
1. Select the Line tool
1. Set the line in a number of positions each time pressing ***Ctrl+P*** to record the position
1. On the ***Units*** menu, select ***Inches***
1. If the calibration warning dialog is show, press ***Continue*** to dismiss the dialog
1. Set the line in new positions each time pressing ***Ctrl+P*** to record the position
1. On the ***File*** menu verify that the ***Save Positions*** and ***Save Positions As*** menu items are enabled
1. On the ***Edit*** menu, select the ***Manage Positions*** menu item
1. Verify that the Position Management dialog is displayed
1. Verify that the number of positions corresponds to the number of positions recorded
1. Move the slider slowly back and forth and verify that the recorded positions are played back
1. Enter descriptions for a number of the positions
1. Press the ***Save*** button
1. Verify that the Position save dialog is displayed
1. Enter a filename
1. Enter a title for the positions
1. Enter a description for the positions
1. Save the positions
1. Close the position management dialog
1. Close the Meazure program
1. Examine the saved file and verify that its contents are valid XML and have valid data
1. Run the Meazure program
1. On the ***File*** menu select the ***Load Positions*** item
1. Verify that the title is the one specified when the log file was saved
1. Verify that the description is the one specified when the log file was saved
1. Verify that the Position Management dialog is displayed and that the positions correspond 
   to those originally saved
1. Verify that the positions recorded using the Cursor Tool are now displayed using the Point Tool
1. Move the slider to a given position and press the Delete button
1. Verify that the position is deleted
1. Select the Rectangle Tool
1. Move the position slider to a position somewhere in the middle of the recorded set
1. Press the ***Replace*** button
1. Move the position slider and verify that the original position and tool have been replaced by 
   the Rectangle Tool and its position
1. Move the Rectangle Tool
1. Press the ***Add*** button
1. Verify that the new position has been recorded
1. Close the position management dialog
1. On the ***Edit*** menu select the ***Delete Positions*** item
1. Verify that the position ***Save*** items on the ***File*** menu are disabled
1. Using the position management dialog verify that there are no recorded positions
1. Record a position using any tool
1. Attempt to exit Meazure and verify that a dialog is displayed warning that there are unsaved positions
1. Press ***No***

## Help

1. Start the program
1. On the ***Help*** menu select the ***Contents*** item
1. Verify that the online help window appears with the ***Contents*** tab selected
1. On the ***Help*** menu select the ***What's This*** item.
1. Click on the Tool Information section
1. Verify that the help for the Tool Information section is displayed
1. Display the ***Tools*** menu. Position the cursor over the ***Point*** item and press ***F1***. Verify that 
   the help for the Point Tool is displayed
1. Close the help window
1. On the ***Help*** menu select ***Meazure Releases***
1. Verify that the Meazure releases page on GitHub is displayed in a web browser
1. On the ***Help*** menu select ***Report an Issue***
1. Verify that the Meazure issues page on GitHub is displayd in the browser
1. Close the web browser

## About Box

1. On the ***Help*** menu select the ***About Meazure*** item.
1. Click on the ***www.cthing.com*** URL and verify that the C Thing Software home page is displayed in a web browser
1. Click on the ***Contribute*** button and verify that you are taken to the Meazure sponsors page on GitHub
1. Close the web browser
1. Dismiss the About box
1. Exit the program
