README FILE FOR TWITTLE DEVELOPER SOURCE

Project written by Loren Segal 
for the COMP446 C++ Project in F08


CONTENTS OF THIS DOCUMENT

  1. ARCHIVE CONTENTS
  2. CODE DOCUMENTATION
  3. BUILDING THE PROJECT
  4. RUNNING THE PROJECT
  5. EXTRA NOTES


1. ARCHIVE CONTENTS

- twittle/
	* bin/twittle.exe		- The binary executable (for Win32)
	- doc/
		* manual.pdf 		- The User Manual (individal version)
		* report.pdf 		- The Project Report (for reference)
		* html/index.html 	- The Doxygen C++ documentation index file
	- include/			- Header files (.h)
	- resources/			- Raw photoshop icon files (for reference)
	- src/				- Source code (.cpp)
	* .cproject			- Eclipse CDT project file
	* .project			- Eclipse project file
	* Doxyfile			- Doxygen configuration file
	* twittle.vcproj		- Visual Studio 2008 project file
	* twittle-vs2005.vcproj		- Visual Studio 2005 project file
* twittle-vs2005.sln			- Visual Studio 2005 solution file
* twittle.sln				- Visual Studio 2008 solution file (recommended)


2. CODE DOCUMENTATION

Code documentation can be found by opening the file `twittle/doc/html/index.html` 
in a standard web browser. The documentation was generated using Doxygen
with the configuration file found in `twittle/Doxyfile`. You can regenerate
these documents anytime with the command `doxygen` inside the `twittle/`
directory which will automatically use this configuration file, but note 
that you must have the `dot` utility (Graphviz) to properly generate the 
class diagrams and call graphs.

		
3. BUILDING THE PROJECT

The project was created using Visual Studio 2008, though there are extra
project files for Visual Studio 2005 and Eclipse CDT (Ganymede 3.4). 
Unfortunately, no Makefile is provided for this project.

To build the project, choose the project file of your choice and follow
the standard build procedures with the IDE. Note that in Eclipse, only
a DEBUG build configuration is provided. The recommended IDE is VS2008,
however a VS2005 solution file is provided since they are not backwards
compatible and users may not have access to VS2008. 

Note that changes to the project configurations might be necessary to
properly build the projects.

** IMPORTANT NOTE **

Please remember that to compile the project, you need to have wxWidgets 
installed on your system. The solution files (and Eclipse's project file) 
are setup to use the %WXWIN% environment variable to locate the wxWidgets 
base install directory under Win32. Documentation on installing and compiling
the wxWidgets libraries can be found at http://wxwidgets.org. The recommended
version of wxWidgets is 2.8.9, though anything after and including 2.8.6 
should also work. Please make sure to build a UNICODE RELEASE BUILD, since
this is what the project uses.


4. RUNNING THE PROJECT

A binary release of the project for Win32 systems can be found at 
`twittle/bin/twittle.exe`. Note that this was tested under Windows 
XP and Vista but not Windows 98. Due to a lack of unicode support 
under Windows 98, the platform may not support the application.

Twittle stores application data in the standard application data folders
by system type (Application Data under XP, AppData under Vista). See
the user manual for more information if you do not wish to keep the
data on your system after testing the executable.

** IMPORTANT NOTE FOR WINDOWS **

The binary file requires the Microsoft C++ SP1 Redistributable Package.
Please make sure you have this or you will get a "configuration" error
when running the binary. Details on where you can get this package
can be found in the User Manual.


5. EXTRA NOTES

A version of the project report and user manual are provided in the 
`twittle/doc` directory for convenience purposes, since they can be 
easily navigated with hyperlinks.

Twittle was released to the public (binary release only) on December 9th 
2008. I assume I am permitted to do this. If there are any legal issues 
regarding Concordia's submission policy, please inform me. Otherwise, 
you can see the public release at:

	http://kthx.net/twittle 

You can also follow the Twitter updates made using Twittle during 
development at:

	http://twitter.com/twittletest

