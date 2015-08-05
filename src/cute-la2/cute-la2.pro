TEMPLATE = app
TARGET = cute-la2

CONFIG += release

BEQT = core network widgets

isEmpty(BEQT_PREFIX) {
    mac|unix {
        BEQT_PREFIX=/usr/share/beqt
    } else:win32 {
        BEQT_PREFIX=$$(systemdrive)/PROGRA~1/BeQt
    }
}
include($${BEQT_PREFIX}/share/beqt/depend.pri)

HEADERS += \
    application.h \
    mainwindow.h \
    global.h \
    generalsettingstab.h \
    manorsettingstab.h \
    fishingsettingstab.h \
    timerwidget.h \
    manorwidget.h \
    fishingwidget.h \
    craftwidget.h

SOURCES += \
    application.cpp \
    main.cpp \
    mainwindow.cpp \
    global.cpp \
    generalsettingstab.cpp \
    manorsettingstab.cpp \
    fishingsettingstab.cpp \
    timerwidget.cpp \
    manorwidget.cpp \
    fishingwidget.cpp \
    craftwidget.cpp

TRANSLATIONS += \
    ../../translations/cute-la2/cute-la2_ru.ts

RC_FILE = win.rc

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

translationsTs=$$files($${PWD}/../../translation/cute-la2s/*.ts)
for(fileName, translationsTs) {
    system(lrelease $$nativeFileName($${fileName}))
}

contains(CLA2_CONFIG, builtin_resources) {
    DEFINES += BUILTIN_RESOURCES
    RESOURCES += \
        cute_la2.qrc \
        cute_la2_doc.qrc \
        ../../translations/cute-la2/cute_la2_translations.qrc
}

##############################################################################
################################ Installing ##################################
##############################################################################

!contains(CLA2_CONFIG, no_install) {

include(../../prefix.pri)

##############################################################################
################################ Binaries ####################################
##############################################################################

target.path = $${BINARY_INSTALLS_PATH}
INSTALLS = target

##############################################################################
################################ Translations ################################
##############################################################################

!contains(CLA2_CONFIG, builtin_resources) {
    installsTranslations.files=$$files($${PWD}/../../translations/cute-la2/*.qm)
    installsTranslations.path=$${RESOURCES_INSTALLS_PATH}/translations
    INSTALLS += installsTranslations
}

##############################################################################
################################ Other resources #############################
##############################################################################

!contains(CLA2_CONFIG, builtin_resources) {
    installsChangelog.files=$$files($${PWD}/changelog/*.txt)
    installsChangelog.path=$${RESOURCES_INSTALLS_PATH}/changelog
    INSTALLS += installsChangelog
    installsCopying.files=$$files($${PWD}/copying/*.txt)
    installsCopying.path=$${RESOURCES_INSTALLS_PATH}/copying
    INSTALLS += installsCopying
    installsDescription.files=$$files($${PWD}/description/*.txt)
    installsDescription.path=$${RESOURCES_INSTALLS_PATH}/description
    INSTALLS += installsDescription
    installsDocs.files=$$files($${PWD}/doc/*)
    installsDocs.path=$${RESOURCES_INSTALLS_PATH}/doc
    INSTALLS += installsDocs
    installsIcons.files=$$files($${PWD}/icons/*)
    installsIcons.path=$${RESOURCES_INSTALLS_PATH}/icons
    INSTALLS += installsIcons
    installsInfos.files=$$files($${PWD}/infos/*.beqt-info)
    installsInfos.path=$${RESOURCES_INSTALLS_PATH}/infos
    INSTALLS += installsInfos
    installsPixmaps.files=$$files($${PWD}/pixmaps/*)
    installsPixmaps.path=$${RESOURCES_INSTALLS_PATH}/pixmaps
    INSTALLS += installsPixmaps
}

} #end !contains(CLA2_CONFIG, no_install)
