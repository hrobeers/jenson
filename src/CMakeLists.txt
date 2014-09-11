project(jenson)

# Sources
set(SRC
    jenson.cpp
)

# Headers
set(HDR
    jenson.h
    qmemory.hpp
)


#
# The executable
#

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_library(jenson ${SRC} ${HDR} ${MOC_SRC})


#
# Linking
#

#Qt modules
target_link_libraries(jenson Qt5::Core)
