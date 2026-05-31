cmake_minimum_required(VERSION 4.3)
project(ivtc_tool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

option(FFMPEG_ROOT "Root of installed FFmpeg library")

find_package(Qt6 REQUIRED 
    COMPONENTS Core Gui Widgets
)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
)
add_executable(ivtc_tool ${SOURCES})
target_include_directories(ivtc_tool PRIVATE "src")
target_link_libraries(ivtc_tool PRIVATE 
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Link FFmpeg
if(FFMPEG_ROOT)
    find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h "${FFMPEG_ROOT}/include" NO_DEFAULT_PATH)
    find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h "${FFMPEG_ROOT}/include" NO_DEFAULT_PATH)
    find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h "${FFMPEG_ROOT}/include" NO_DEFAULT_PATH)

    find_library(AVCODEC_LIBRARY avcodec "${FFMPEG_ROOT}/lib" NO_DEFAULT_PATH)
    find_library(AVFORMAT_LIBRARY avformat "${FFMPEG_ROOT}/lib" NO_DEFAULT_PATH)
    find_library(AVUTIL_LIBRARY avutil "${FFMPEG_ROOT}/lib" NO_DEFAULT_PATH)

    if(AVCODEC_INCLUDE_DIR AND AVCODEC_LIBRARY AND
       AVFORMAT_INCLUDE_DIR AND AVFORMAT_LIBRARY AND
       AVUTIL_INCLUDE_DIR AND AVUTIL_LIBRARY)
        target_include_directories(ivtc_tool PRIVATE 
            ${AVCODEC_INCLUDE_DIR}
            ${AVFORMAT_INCLUDE_DIR}
            ${AVUTIL_INCLUDE_DIR}
        )
        
        target_link_libraries(ivtc_tool PRIVATE 
            ${AVCODEC_LIBRARY}
            ${AVFORMAT_LIBRARY}
            ${AVUTIL_LIBRARY}
        )
    else()
        message(FATAL_ERROR "Cannot find FFmpeg at specified FFMPEG_ROOT. Please set to correct root or unset and use PkgConfig.")
    endif()
else()
    find_package(PkgConfig REQUIRED)
    if(PkgConfig_FOUND)
        pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
        pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
        pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
    endif()

    if((NOT ${PkgConfig_Found}) OR (NOT ${AVCODEC_Found}) OR (NOT ${AVFORMAT_Found}) OR (NOT ${AVUTIL_Found}))
        message(FATAL_ERROR "Cannot Find FFmpeg. Either install with PkgConfig or set FFMPEG_ROOT")
    endif()

    target_link_libraries(ivtc_tool PRIVATE
        PkgConfig::AVCODEC
        PkgConfig::AVFORMAT
        PkgConfig::AVUTIL
    )
endif()