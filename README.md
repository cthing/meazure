# Meazure(tm) 2.0.1

Copyright 2001, 2004, 2011 C Thing Software


## System Requirements

- Pentium 300 MHz processor or higher
- Windows XP, Vista, Windows 7
- Internet Explorer 6.0 or above (for online Help)
- 64 MB memory or higher
- 4 MB video memory or higher
- 5.6 MB disk space


## Installation

Meazure comes with an installer tool to install all
program files on the system. In addition to installing
the program files, the installer configures the system
to run the Meazure program. This configuration includes:

- Associating the file suffix .mea with the Meazure program
- Optionally placing an icon on the desktop
- Creating an entry on the Program menu


## Online Help

Meazure includes extensive online help. The program's
Help menu provides access to the table of contents and
index, as well as a search capability. In addition,
context sensitive help is available using the F1 key,
the SHIFT+F1 keys, and the What's This item on the
Help menu.

## License

Refer to the License.txt file distributed with this program
for details on the licensing of the Meazure program.

## Building

Refer to the Building.txt file on the project site for
information on how to build Meazure from source.


## Changelog

### Changes 2.0 to 2.0.1

- Publish Meazure as an Open Source project on GitHub under
  the GNU General Public License.

- Windows 98, ME, NT and 2000 are no longer supported.

### Changes 1.0 to 2.0

- Feature: A log of tool positions can be recorded and
  saved. The position log can be reloaded and played
  back. The position log file is in XML and the DTD
  is distributed with the program and is also available
  from the C Thing Software website.

- Feature: An Angle tool has been added. Using this tool,
  arbitrarily oriented angles can be measured.

- Feature: A Circle tool has been added. Using this tool,
  a circular area can be measured.

- Feature: The user can define their own set of units.

- Feature: Multiple monitor desktop configurations
  are now fully supported.

- Feature: The rulers can now be dragged across the
  screen.

- Feature: On Windows 2000 and above, the opacity can
  be set on the rulers, crosshairs and data windows.

- Feature: The origin of the coordinate system is now
  indicated by a marker.

- Feature: Additional color spaces are available for
  expressing the color value of a pixel. In addition,
  a text field is used to display the color so that
  the color value can be copied to the clipboard.

- Feature: Angles can now be reported in degrees or
  radians.

- Feature: The number of decimal places displayed for
  each measurement in each system of units can now be
  specified as a preference.

- Feature: Tool crosshairs can now be toggled between
  visible and hidden to aid in measuring small objects.

- Feature: There are now menu items for zooming the
  magnifier in and out.

- Feature: Resolution calibration can now be done in
  metric as well as english units.

- Feature: A master reset is now provided to restore
  the factory default settings.

- Feature: Windows XP display themes are now supported.

- Fix: A red rectangle no longer remains when changing
  to another tool from the Window Tool using the Tools
  menu.

- Fix: Certain screen appearance schemes (e.g. Eggplant)
  would cause the menu to wrap to two lines thereby
  partially obscuring the magnifier zoom slider. This
  has been corrected.

- Fix: When copying a screen region the tool outline and
  crosshairs were sometimes captured with the image. This
  has been corrected.

- Windows 95 is no longer supported.


## Known Issues

- On Windows XP (all versions) the display of the crosshair
  tooltip (e.g. "Point 1") is intermittent due to a problem
  with version 6 of the Microsoft comctl32.dll.


## Feedback

Please contact us with your comments and suggestions at:

meafeedback@cthing.com
http://www.cthing.com
