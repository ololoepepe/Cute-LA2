TEMPLATE = subdirs

SUBDIRS += cute-la2

!contains(CLA2_CONFIG, no_plugins) {
    SUBDIRS += plugins
}
