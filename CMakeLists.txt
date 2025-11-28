cmake_minimum_required(VERSION 3.16)
project(cpp-redis-timer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include(FetchContent)

FetchContent_Declare(
    sw_redis
    GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus.git
    GIT_TAG 1.3.15
)
FetchContent_MakeAvailable(sw_redis)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

add_executable(main src/main.cpp)
target_link_libraries(main redis++_static ${HIREDIS_LIBRARIES})
target_include_directories(main PRIVATE ${HIREDIS_INCLUDE_DIRS})