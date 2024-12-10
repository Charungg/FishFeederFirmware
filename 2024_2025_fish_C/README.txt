This is the C source folder for an example test of the fish feeder GUI.

The subfolder FishFeederGUI must contain the customjre subfolder for the platform 
you are using. There are versions for the main platforms and processor architectures:

Linuxx64
Linuxaarch64
MacOSaarch64
MacOSx64
Windowsx64

These are downloadable as zip files
customjre_*
where * will be of the above architectures.
I have not yet built the customjre for Linux on an Arm architecture if anyone needs this please ask and
we will build it.

The CMakeLists.tst file has two elements that must be set dependent on the platform you are using:

include_directories
target_link_libraries

You need to uncomment the entries for the platform and comment out or delete the other entries.

Windows also requires a path to these libraries to be set either in the run configuration in CLion
(or as a path entry if the program is to be executed from the terminal) to prevent it finding
incompatible versions of the jvm components if java is installed elsewhere in the system.
*nix based platforms naturally look first in the provided custom runtime folder so don't need
any additional paths to be set.
