# Sources
set(SRC
    jenson.cpp
)

# Headers
set(HDR
    jenson.h
    qunique_ptr.hpp
)


#
# The executable
#

if(NOT JENSON_BINARY_NAME)
    set(JENSON_BINARY_NAME "jenson")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_library(${JENSON_BINARY_NAME} ${SRC} ${HDR} ${MOC_SRC})


#
# Linking
#

#Qt modules
target_link_libraries(${JENSON_BINARY_NAME} Qt5::Core)
