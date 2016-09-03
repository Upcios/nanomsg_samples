cmake_minimum_required (VERSION 3.4)

project (NetworkTests)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE})

add_subdirectory(NanomsgPipeline)
add_subdirectory(NanomsgReqRep)
add_subdirectory(NanomsgPair)
add_subdirectory(NanomsgPubSub)
add_subdirectory(NanomsgSurvey)
add_subdirectory(NanomsgBus)