cmake_minimum_required (VERSION 3.0)

set(LSBLURAY_VERSION_MAJOR 0)
set(LSBLURAY_VERSION_MINOR 1)
set(LSBLURAY_VERSION_PATCH 0)

project (lsbluray VERSION "${LSBLURAY_VERSION_MAJOR}.${LSBLURAY_VERSION_MINOR}.${LSBLURAY_VERSION_PATCH}")

set(CMAKE_MODULE_PATH ${lsbluray_SOURCE_DIR}/cmake/modules)

find_package(Bluray REQUIRED)

configure_file(src/version.h.in version.h)
include_directories(${CMAKE_BINARY_DIR})

set(lsbluray_SRCS
    src/lsbluray.c
    src/output_human.c
)

add_executable(lsbluray ${lsbluray_SRCS})
target_link_libraries(lsbluray ${BLURAY_LIBRARIES})
