JenSON
======

JenSON is a JSON serialization library based on Qt's Property system and Qt's Json library.

JenSON's most interesting features are:
  - The very permissive BSD 2-clause license.
  - Only one line of JenSON code to make a QObject class (de)serializable.
  - Custom serializers can be implemented to (de)serialize to/from specific data contracts.

This library is originally developed for the finFoil project (https://github.com/hrobeers/finFoil).


## Dependencies

C++11 compliant compiler (GCC >= 4.7, Clang >= 3.0, Intel C++ >= 12.1)

Qt >= 5.1

Boost >= 1.54


## Usage

JenSON requires that the Qt eventloop is running. JenSON uses a std::unique_ptr that deletes QObjects
using QObject::deleteLater(), which needs a running Qt eventloop to ensure proper deletion.

Usage examples can be found in the tests/ folder or in the finFoil project.
