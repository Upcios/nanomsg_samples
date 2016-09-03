cmake_minimum_required (VERSION 3.4)
project (NanomsgPipeline)

# nanomsg
find_path(NANOMSG_INCLUDE_DIR nn.h) # searching for include directory
find_library(NANOMSG_LIBRARY nanomsg) # searching for library file
if (NANOMSG_INCLUDE_DIR AND NANOMSG_LIBRARY)
	include_directories(${NANOMSG_INCLUDE_DIR})
	set(MY_LIBRARIES ${MY_LIBRARIES} ${NANOMSG_LIBRARY})
endif()

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB HEADERS "${PROJECT_SOURCE_DIR}/*.h")

add_executable (nanomsg_pipeline ${SOURCES} ${HEADERS})

target_link_libraries (nanomsg_pipeline ${MY_LIBRARIES} )
