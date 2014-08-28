INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Sources
set(SRC
    jenson.cpp
)

# Headers
set(HDR
    jenson.h
)


#
# The executable
#

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_library(${BINARY_NAME} ${SRC} ${HDR} ${MOC_SRC})


#
# Linking
#

#Qt modules
target_link_libraries(jenson Qt5::Core)
