project(jenson)

# Sources
set(SRC
    jenson.cpp
)

# Headers
set(HDR
    jenson.h
    jenson_global.hpp
    qmemory.hpp
)


#
# The executable
#

if(JENSON_SHARED)
    set(JENSON_LIBRARY_TYPE SHARED)
    message(STATUS "JENSON_LIBRARY_TYPE = SHARED")
else()
    set(JENSON_LIBRARY_TYPE STATIC)
    message(STATUS "JENSON_LIBRARY_TYPE = STATIC")
endif()

add_library(jenson ${JENSON_LIBRARY_TYPE} ${SRC} ${HDR} ${MOC_SRC})


#
# Linking
#

#Qt modules
target_link_libraries(jenson Qt5::Core)

install(TARGETS jenson
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)
