import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Gui"
    Depends { name: "Qt"; submodules: ["core-private", "gui"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: ["/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libQt5Core.so.5.6.2", "pthread"]
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5Gui"
    libNameForLinkerRelease: "Qt5Gui"
    libFilePathDebug: ""
    libFilePathRelease: "/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libQt5Gui.so.5.6.2"
    cpp.defines: []
    cpp.includePaths: ["/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include/qt5/QtGui/5.6.2", "/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include/qt5/QtGui/5.6.2/QtGui"]
    cpp.libraryPaths: []
    
}