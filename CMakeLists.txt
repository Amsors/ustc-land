cmake_minimum_required(VERSION 3.28)
project(ustc_land)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/run)

add_definitions(-DNANOGUI_USE_OPENGL)
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-D__USTC_LAND_RELEASE__)
else()
    option(NANOGUI_SHOW_WIDGET_BOUNDS "Show widget bounds" OFF)
    if(NANOGUI_SHOW_WIDGET_BOUNDS)
        set(LIB_SUFFIX "-bounded")
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB RUN_FILES CONFIGURE_DEPENDS true ${CMAKE_SOURCE_DIR}/run/*)
# 不删除日志和存档文件夹
foreach(file IN LISTS RUN_FILES)
    if(NOT (file MATCHES ".+log.+" OR file MATCHES ".+save.+"))
        message(STATUS "Deleting ${file}")
        file(REMOVE_RECURSE ${file})
    endif()
endforeach()
file(GLOB RESOURCES CONFIGURE_DEPENDS true ${CMAKE_SOURCE_DIR}/src/resources/*)
file(COPY ${RESOURCES} DESTINATION ${CMAKE_SOURCE_DIR}/run)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/cpp/*.cpp)

add_executable(ustc_land ${SOURCES})
target_link_libraries(ustc_land
        PUBLIC ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/glfw3.lib
        PUBLIC ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/nanogui${LIB_SUFFIX}.lib
        PUBLIC ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/jsoncpp.lib
)
