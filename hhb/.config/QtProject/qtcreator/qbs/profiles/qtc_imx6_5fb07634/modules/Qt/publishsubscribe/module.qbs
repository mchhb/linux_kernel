import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "PublishSubscribe"
    Depends { name: "Qt"; submodules: ["core"]}

    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: ["gobject-2.0", "gconf-2", "glib-2.0", "/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libQt5Core.so.5.6.2", "pthread"]
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5PublishSubscribe"
    libNameForLinkerRelease: "Qt5PublishSubscribe"
    libFilePathDebug: ""
    libFilePathRelease: "/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/lib/libQt5PublishSubscribe.so.5.4.0"
    cpp.defines: ["QT_PUBLISHSUBSCRIBE_LIB"]
    cpp.includePaths: ["/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include/qt5", "/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include/qt5/QtPublishSubscribe"]
    cpp.libraryPaths: []
    
}
