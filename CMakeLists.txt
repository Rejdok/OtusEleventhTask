cmake_minimum_required(VERSION 3.5)
project(async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)

find_package (Threads REQUIRED)

add_library(async SHARED async.cpp async.h CommandProcessor.h)

target_link_libraries(async Threads::Threads)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)

install(TARGETS async async_cli 
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib)

set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rejdok")
set(CPAK_PACKAGE_CONTACT "lvbnu@yandex.ru")
include(CPack)
