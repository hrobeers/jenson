#
# This file contains common .pro file configuration
# (for src.pro and tests.pro)
#

CONFIG += c++11

# Place shared binaries in bin/ folder
count(DESTDIR, 0) {
    DESTDIR = $${_PRO_FILE_PWD_}/../bin
}
