#Instructions on how to setup the Simple DMX development environment

## Overview ##
Simple DMX spans multiple languages meaning several technologies are required to begin working on Simple DMX.  The minimum set of applications required to utilize all of the development tools is as follows:
  * [JDK 1.5](http://java.sun.com/javase/downloads/index.jsp) - Java Runtime / Compiler
  * gcc / gcb / make - C and C++ Compiler
  * [subversion](http://subversion.tigris.org/) - version control system
  * [Eclipse](http://www.eclipse.org/) - IDE
  * [subclipse](http://subclipse.tigris.org/) - Eclipse Subversion Integration
  * [CDT](http://www.eclipse.org/cdt/) - Eclipse C and C++ Integration

All of the above tools are free.  Installation on a non-Windows platform is relatively painless, however there are some extra steps when using Windows.


## Java 1.5 ##
Make sure you have a Java 1.5 compatable JDK installed.  You can test which version of java you have by typing

`java -version`

at the command line.  If you do not have java installed or need to update the JDK you can download it here:

[JDK 1.5](http://java.sun.com/javase/downloads/index.jsp)


## gcc / gcb / make ##
These C / C++ tools are the most popular free applications for developing in C / C++.  Their functionality is as follows:

  * gcc - C / C++ compiler
  * gcb - C / C++ debugger
  * make - C / C++ compilation file

These C compiler tools have been chosen for Simple DMX to ensure the best cross-platform compatibility.

### Unix Based Setup ###
The C compiler tools listed above come standard with most Unix style operating systems.  If they are not executable from the command line, a package manager or other tool can be used to install them.

### Windows Based Setup ###
The C compiler tools are designed for Unix based environments, so getting them to work in Windows is a bit tricky.  There are two options.  You can install [cygwin](http://www.cygwin.com/) with the GCC package. However, the installation of cygwin is not simple, so we recommend using [Minimalist GNU for Windows](http://www.mingw.org/).  The MinGW tools provide an easy installer for the C compiler tools needed to develop Simple DMX.  Download the latest version of **MinGW-X.X.X.exe** from the MinGW [download page](http://www.mingw.org/download.shtml).  Run the file and click through the installation process, when you come to a page where you need to select which packages to install be sure to check the boxes for the additional packages: **C++ compiler** and **mingw32-make**.

**ADD IMAGE?**

After you have completed the installer you have to add the MinWG binaries directory to your windows PATH variable, in order for Eclipse to easily access the applications.  You can find the binaries directory in
`<install_path>\MinGW\bin`
if you can use the **gcc** and **mingw32-make** commands from the command line you have successfully installed gcc in Windows.


## Subversion ##
The Simple DMX code is stored with the Subversion revision system.

[Subversion Home Page](http://subversion.tigris.org/)

You will need to install a Subversion client and request access to the code repository before you can develop Straigt DMX.  Which Subversion client you need depends on your operating system:

  * **Mac OS X** - we recommend the following pre-build binary package: [Martin Ott's packaging](http://www.codingmonkeys.de/mbo/articles/2006/11/08/subversion-1-4-2). Grab the package and install it. It contains the command line interface to svn (so that you can use svn directly from the terminal as for a traditional UNIX) it also containS the required binding libraries to interface with the eclipse component.
  * **WinXP** - [TortoiseSVN](http://tortoisesvn.net/) provides an interface that is integrated with the Explorer.
  * **Linux** - simply grab a recent SVN client for your distribution (any version above 1.4.0 should do).

### Requesting Access ###
Sign up as a developer at http://code.google.com/p/simple-dmx/.

### Testing ###
Once you have a user name and login setup, you can test your connection to the SVN server with the following command:

`svn list https://simple-dmx.googlecode.com/svn/`

If this command returns a list of directories, you have installed subversion successfully.


## Eclipse ##
Download and install the Eclipse IDE.

[Eclipse Home Page](http://www.eclipse.org/)

Eclipse version 3.2 is required, if you have version 3.1 please upgrade to 3.2.  If you need to use Eclipse 3.1, two versions of Eclipse can be used on the same computer as long as they reside in separate installation directories and the workspaces they use are disjoint.

If you are unfamiliar with Eclipse you may find the following video demonstrations helpful.  [Introduction To Eclipse](http://showmedo.com/videos/series?name=IntroductionToEclipseWithJava_JohnM)

#### Installing an Eclipse Plugin ####
Both Subclipse and CDT tools can be installed through Eclipe's built in plugin manager.

To install a new plugin nagivate to Help > Software Updates > Find and Install.  Then select "Search New Features to Install" and in the next dialog box click on the "New Remote Site..." button.

You will be presented with a dialog box where you should give a name and the URL of the site where the Eclipse extension can be found. For instance, for CDT, you would enter the information as shown below
  * Name - CDT
  * URL - http://download.eclipse.org/tools/cdt/releases/callisto

Finally, select that site (check its box) and click finish. Follow the straightforward directions, read and accept the license agreement and restart Eclipse when complete.

### CDT Plugin ###
CDT is Eclipse's C/C++ Development Tool project. It is an industrial strength C/C++ IDE that also serves as a platform for others to provide value added tooling for C/C++ developers.

If you are not familar with CDT the following links may be helpful:
  * http://wiki.eclipse.org/index.php/CDT:Movies
  * http://dev.eclipse.org/viewcvs/index.cgi/cdt-home/user/Tutorials.html?root=Tools_Project&view=co
  * http://wiki.eclipse.org/index.php/CDT/User/FAQ
  * http://wiki.eclipse.org/index.php/CDT

The CDT can be installed in the Eclipse update manager from the following link: http://download.eclipse.org/tools/cdt/releases/callisto

### Subclipse Plugin ###
Although svn has a command line interface it is easiest to work with Subversion Eclipse integration.  Our recommened Subversion plugin is [Subclipse](http://subclipse.tigris.org/).  Subclipse can be installed easily from the Eclipse Update site provided through their website: http://subclipse.tigris.org/update_1.0.x.

**Note:** If you are using Eclipse 3.2 and the latest version of Subclipse with OS X you may need to change the Eclipse SVN Interface to JavaSVN.  This can be done from Window > Prefrences > Team > SVN.


## Checking-out the source code ##
The subversion repository for Cameo is hosted on cameocontrol.com.  To check out the source code in the repository, follow these directions.

Begin by starting the SVN Checkout wizard, Select, File > New > Other.  A window of wizards should pop up.  Navigate in the dialog box down to the SVN folder (which should be visible since you installed Subclipse) and pick **Checkout Projects from SVN**

In the next dialog box, enter an SVN URL for the root of the repository as follows,

  * Pick "Create a New repository location"
  * Enter the URL https://simple-dmx.googlecode.com/svn/.

If everything goes according to plan, you will be prompted for your password, or even better, subversion will used a cached copy of your password. The outcome should be a dialog box populated with all the _projects_ that exist in the repository.

The next step is **critical**. To check out a project you must select the correct sub-directory. Each subversion project uses the following structure: The root of the project has 3 sub-directories named trunk, tags and branches.
  * trunk - contains the current working revision
  * tags - contain one subdirectory whose name is a symbolic tag
  * branches - plays the same role for experimental (i.e., disruptive) changes that should not be on the trunk

For instance, to checkout the head revision (working revision) of Simple DMX for Windows, you should select the folder win32 > trunk before you click on "next". Once you click on "next", Eclipse will retrieve information about the Eclipse project that exists underneath that directory on the server. If all is working correctly, it should retrieve the project we created and display the project name (SDMX\_Win32 in this case).

At this point, simply clicking on "Finish" will complete the process and initiate the actual Subversion transaction to download the entire content of the project. Depending on your connection speed, this should take a minute or two.

All of the projects are listed below:

| **Project** | **Description** |
|:------------|:----------------|
| win32       | Simple DMX Win32 and Driver Connector |
| win32test   | tests the DLL generated by win32 |
| dmxconnector | takes timeddmx DLL and connects it to the JNI Header file |
| jnitest     | tests the DLL generated by dmxconnector |
| simpledmxjava | the java front end of Simple DMX |
| common      | files common to all Simple DMX distributions |