Version: Borderless Window Patch 0.6 (http://www.timorg.net/borderless)
Copyright: 2009 Anthony Cassidy (anthonytcassidy@hotmail.com)


For the licence of use and modification see the end of this file.

The newest version (and the source) of this program is at the above site.


0. Table of Contents:

1. The 5 W's
2. Using this package
3. The example programs
4. Thanks and Credits
5. Where to next
6. Licence


1. The 5 W's:

Who: Anthony "Timorg" Cassidy (anthonytcassidy@hotmail.com)
What: Patches for AllegroGL 0.4.3 and OpenLayer 2.1 to remove the window border
When: Version 0.6 on the 20-May-2009
Where: Melbourne, Australia
Why: Because karistouf needed to use multiple monitors for a application.


2. Using this package

2.1 Using the patches:

Unzip this archive to c:\borderless, then follow the below command

2.1.1 To patch AllegroGL:
Copy alleggl.borderless.patch to the root of the AllegroGL source distribution, and run the following command.

	patch.exe -p1 < alleggl.borderless.patch

Then build as normal

2.1.2 To patch Openlayer:
Copy alleggl.borderless.patch to the root of the AllegroGL source distribution, and run the following command.

	patch.exe -p1 < openlayer-2.1.borderless.patch

Run cmake per usual, but before running mingw32-make

	set cplus_include_path=c:\borderless\include\borderless

Then build as normal.


2.2 Using the precompiled binaries.

To use the precompiled binaries you need to set the appropriate search paths.
include: c:\borderless\include\borderless
lib: c:\borderless\lib\borderless

On the gcc command line use
	-Ic:\borderless\include\borderless -Lc:\borderless\include\borderless

and then link against
	-lopenlayer -lglyph-agl -lfreetype.dll -lpng -lzdll -lagl -lalleg -luser32 -lgdi32 -lglu32 -lopengl32


3. The example programs

There are two included example programs, one for OpenLayer (colors) and another for AllegroGL (lathe). To compile the examples run the provided makefile.

Lathe isn't the best example of OpenGL programing, but it does serve the purpose of demonstrating use of multiple monitors.

4. Thanks and Credit:

I didn't write AllegroGL or OpenLayer, I just hacked in a borderless window mode. Thanks to the creators and contributers to AllegroGL and OpenLayer. karistouf for inspiring these patches, and ixilom for testing.


5. Where to next:

The next step is to make this work under linux. I doubt anything will be done beyond that. As I don't have access to any other platforms. If you feel there is something that should be included and isn't let me know.

6. Licence
The example programs are licenced under Attribution-Share Alike 3.0 Unported
(http://creativecommons.org/licenses/by-sa/3.0/).

The patches are covered by the license of the packages they patch.