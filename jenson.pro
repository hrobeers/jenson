TEMPLATE = subdirs

# Make sure projects are built in correct order
CONFIG += ordered

SUBDIRS += \
    src \
    tests

# Specify dependencies
tests.depends = src
