set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Sources
set(SRC
    main.cpp
    jensontests.cpp
)

# Headers
set(HDR
    jensontests.h
)


#
# The executable
#

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_executable(${TESTS_BINARY_NAME} WIN32 ${SRC} ${HDR} ${MOC_SRC} ${FORMS_HEADERS} ${RC_SRC} ${QM})


#
# Linking
#

target_link_libraries(${TESTS_BINARY_NAME} Qt5::Core Qt5::Test)
target_link_libraries(${TESTS_BINARY_NAME} jenson)
