# This is the directory for the controller application
The controller library use Qt for Bluetooth LE communication so it must be built
with Qt libraries, the build deps for crossbuilding for Android devices can be
installed with the install-build-deps.sh shell script.
The script itself depends on equivs-buildand apt to build and install the
respective packages so it is Debian specific.
The libraries installed are also x86 architecture specific so it is better to
follow Qt Creator's instructions if you are not using an x86 Debian distro.

To install the build-deps use
`./install-build-deps`
to uninstall them use
`./install-build-deps uninstall`
