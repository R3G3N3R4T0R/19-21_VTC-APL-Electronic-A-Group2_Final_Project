#!/bin/bash
if [ "$1" = "uninstall" ]
then
	apt purge 19-21-vtc-apl-elect-a-g2-build-deps
	exit
fi
cat > build-deps.control <<"EOF"
Package: 19-21-vtc-apl-elect-a-g2-build-deps
Version: 0none
Architecture: all
Maintainer: Equivs Dummy Package Generator <regenerator@regenerator-pc>
Installed-Size: 6
Depends: default-jdk, libstdc++6:i386, libgcc1:i386, zlib1g:i386, libncurses5:i386, libsdl1.2debian:i386
Section: devel
Priority: optional
Multi-Arch: foreign
Description: additional build-deps for client program
EOF
equivs-build build-deps.control
apt install ./19-21-vtc-apl-elect-a-g2-build-deps_0none_all.deb
rm 19-21-vtc-apl-elect-a-g2-build-deps_0none_* build-deps.control
