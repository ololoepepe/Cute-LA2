===============================================================================
 Cute LA2
===============================================================================

Homepage: https://github.com/the-dark-angel/Cute-LA2

Author: Andrey Bogdanov

License: GNU LGPL (see COPYING.txt)

#brief description

==============================================================================
 Overview
==============================================================================

#overview

==============================================================================
 Dependencies
==============================================================================

In order to build and use Cute LA2 you will need the same libraries and
tools as for every other project using Qt.

See: http://qt-project.org/resources/getting_started for details.

Cute LA2 is intended for use with Qt 5, or with Qt 4.8.
In case of using Qt 4.8, some features may be missing.
Support of Qt libraries' versions lower than 4.8.0 is not guaranteed.

In order to use builtin .svgz icons you will also need
the corresponding plugin.

See: http://qt-project.org/doc/qt-5.0/plugins-howto.html for details.

You will also need BeQt libraries version 3.0.0 or higher.

See: https://github.com/the-dark-angel/BeQt for details.

==============================================================================
 Building and installing (briefly)
==============================================================================

In order to build Cute LA2, just cd into the sources directory and
execute the following commands:

 * "qmake [BEQT_PREFIX=<path>]"
   Here, the path to BeQt may be specified

 * "make"
   or other similar command ("nmake", "mingw32-make", etc.)

 * "make install"
   You may need the superuser privileges in UNIX-like systems 

Note: When building from the command line, you have to configure
the environment (path to Qt libraries, etc.).

You may also use Qt Creator. After building the project, cd to
the output directory and execute the "make install" command, or
configure automatic execution of that command in Qt Creator.

See: http://qt-project.org/doc/qtcreator-2.6 for details.

==============================================================================
 Building and installing (in detail)
==============================================================================

When building Cute LA2, you may pass some parameters to qmake:

 * "CLA2_CONFIG+=builtin_resources"
   Embed resources (including translations) into executable file
   See: http://qt-project.org/doc/qt-5.0/resources.html for details

 * "CLA2_CONFIG+=no_install"
   Don't install any files (building only)

 * "BEQT_PREFIX=<path>"
   Set path to BeQt libraries. Must be used if BeQt libraries were
   installed to a custom location

 * "PREFIX=<path>"
   Set install path. See the note below

 * "BINARY_INSTALLS_PATH=<path>"
   Set install path for executable file. See the note below

 * "RESOURCES_INSTALLS_PATH=<path>"
   Set install path for resources. See the note below

Note: In Windows systems Cute LA2 is installed to the
"C:\Program files\Cute-LA2" directory (or other similar directory,
depending on your system) by default. Executable file and resources are
installed to the corresponding subdirectories.

In UNIX-like systems the directory structure during installation
looks like this:

 * "/usr/bin/cute-la2"
   Path to executable file

 * "/usr/share/cute-la2"
   Path to resources

You may set another installation path. To do so,
pass the corresponding parameter to qmake (see above).

Warning: Don't use paths containing spaces. In Windows systems you may replace
directory names, containing spaces, with the corresponding short names:

See: http://support.microsoft.com/kb/142982 for details.

==============================================================================
 FAQ
==============================================================================

Q: What are Cute LA2 license restrictions?
A: There are almost no restrictions. You may use Cute LA2 as you wish,
but don't forget that this statement doesn't apply to the Qt libraries.
See: COPYING.txt, http://qt-project.org/products/licensing for details.

Q: I'm having troubles using Cute LA2, where can I get help?
A: E-mail/Jabber: ololoepepe@gmail.com

Q: I've detected a bug/I have an idea, where can I report/post it?
A: See the answer above.

==============================================================================
 Deploying
==============================================================================

When deploying Cute LA2, don't forget to include the resource files
(images, translations, etc. - see above).

It's recommended to build applications statically, because that helps avoid
dependency issues. You may use the "builtin_resources" parameter
in case of building your project statically.

For details, see:
http://qt-project.org/doc/qt-5.0/deployment.html#static-vs-shared-libraries
