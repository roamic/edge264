cmake_minimum_required(VERSION 3.14)
project(edge264 VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set(EDGE264_MAJOR 1)
set(EDGE264_MINOR 0)
set(EDGE264_VERSION ${MAJOR}.${MINOR})

option(EDGE264_ENABLE_BASE "Enable decoding using basic instructions" ON)
option(EDGE264_ENABLE_X86_64_V2 "Enable decoding using x86-64-v2 instructions" OFF)
option(EDGE264_ENABLE_X86_64_V3 "Enable decoding using x86-64-v3 instructions" OFF)
option(EDGE264_ENABLE_LOGS "Enable logging" OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR NOT BUILD_SHARED_LIBS)
  add_library(edge264 STATIC src/edge264.c)
else()
  add_library(edge264 SHARED src/edge264.c)
endif()

target_include_directories(edge264 PUBLIC include)

if(EDGE264_ENABLE_BASE)
  add_library(edge264x86_64base STATIC src/edge264_headers.c)
  target_include_directories(edge264x86_64base PUBLIC include)

  if(EDGE264_ENABLE_LOGS)
    target_compile_definitions(edge264x86_64base PRIVATE "HAS_LOGS")
  endif()

  target_compile_definitions(edge264 PRIVATE "HAS_BASE")
  target_link_libraries(edge264 PUBLIC edge264x86_64base)
endif()

if(EDGE264_ENABLE_X86_64_V2)
  add_library(edge264x86_64v2 STATIC src/edge264_headers.c)
  target_compile_definitions(edge264x86_64v2 PRIVATE FN_VARIANT=v2)
  target_include_directories(edge264x86_64v2 PRIVATE include)

  if(EDGE264_ENABLE_LOGS)
    target_compile_definitions(edge264x86_64v2 PRIVATE "HAS_LOGS")
  endif()

  if(MSVC)
    target_compile_options(edge264x86_64v2 PRIVATE /arch:SSE4.2)
  else()
    target_compile_options(edge264x86_64v2 PRIVATE -march=x86-64-v2)
  endif()

  target_compile_definitions(edge264 PRIVATE "HAS_X86_64_V2")
  target_link_libraries(edge264 PUBLIC edge264x86_64v2)
endif()

if(EDGE264_ENABLE_X86_64_V3)
  add_library(edge264x86_64v3 STATIC src/edge264_headers.c)
  target_compile_definitions(edge264x86_64v3 PRIVATE FN_VARIANT=v3)
  target_include_directories(edge264x86_64v3 PRIVATE include)

  if(EDGE264_ENABLE_LOGS)
    target_compile_definitions(edge264x86_64v3 PRIVATE "HAS_LOGS")
  endif()

  if(MSVC)
    target_compile_options(edge264x86_64v3 PRIVATE /arch:AVX2)
  else()
    target_compile_options(edge264x86_64v3 PRIVATE -march=x86-64-v3)
  endif()

  target_compile_definitions(edge264 PRIVATE "HAS_X86_64_V3")
  target_link_libraries(edge264 PUBLIC edge264x86_64v3)
endif()

if(EDGE264_ENABLE_LOGS)
  add_library(edge264logs STATIC src/edge264_headers.c)
  target_compile_definitions(edge264logs PRIVATE HAS_LOGS LOGS FN_VARIANT=log)
  target_include_directories(edge264logs PRIVATE include)

  target_compile_definitions(edge264 PRIVATE "HAS_LOGS")
  target_link_libraries(edge264 PUBLIC edge264logs)
endif()

# Canonical namespaced alias consumed by downstream targets.
add_library(edge264::edge264 ALIAS edge264)
