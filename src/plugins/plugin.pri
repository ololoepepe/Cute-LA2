moduleName=$${TARGET}
moduleNameNoHyphen=$$replace(moduleName, "-", "_")

VERSION = 0.1.0
VER_MAJ = 0
VER_MIN = 1
VER_PAT = 0

CONFIG += plugin release

isEmpty(BEQT_PREFIX) {
    mac|unix {
        BEQT_PREFIX=/usr
    } else:win32 {
        BEQT_PREFIX=$$(systemdrive)/PROGRA~1/BeQt
    }
}
include($${BEQT_PREFIX}/share/beqt/depend.pri)

TRANSLATIONS += \
    ../../../translations/$${moduleName}/$${moduleName}_ru.ts

##############################################################################
################################ Generating translations #####################
##############################################################################

#Gets a file name
#Returns the given file name.
#On Windows slash characters will be replaced by backslashes
defineReplace(nativeFileName) {
    fileName=$${1}
    win32:fileName=$$replace(fileName, "/", "\\")
    return($${fileName})
}

translationsTs=$$files($${PWD}/../../translations/$${moduleName}/*.ts)
for(fileName, translationsTs) {
    system(lrelease $$nativeFileName($${fileName}))
}

RESOURCES += \
    $${moduleNameNoHyphen}.qrc \
    ../../../translations/$${moduleName}/$${moduleNameNoHyphen}_translations.qrc

##############################################################################
################################ Installing ##################################
##############################################################################

!contains(CLA2_CONFIG, no_install) {

include($${PWD}/../../prefix.pri)

##############################################################################
################################ Binaries ####################################
##############################################################################

target.path = $${PLUGINS_INSTALLS_PATH}
INSTALLS = target

} #end !contains(CLA2_CONFIG, no_install)
