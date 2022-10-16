cmake_minimum_required(VERSION 3.2.0)

# set project name
#set(PROJ_NAME ttttttt)
project(${PROJ_NAME})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${DLL_ARCHIVE_DIR_DEBUG}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${DLL_ARCHIVE_DIR_RELEASE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DLL_ARCHIVE_DIR_DEBUG}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DLL_ARCHIVE_DIR_RELEASE}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# project setting
#######################################################
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h 
					${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
					${CMAKE_CURRENT_SOURCE_DIR}/*.rc 
					${CMAKE_CURRENT_SOURCE_DIR}/*.rc2 
					)
message(STATUS "SOURCE files : ${SRC_FILES}")

# find mfc components
foreach(srcfile ${SRC_FILES})
	
	# check stdafx.h file
	string(TOLOWER "${srcfile}" srcfile_lower)
	get_filename_component(filename ${srcfile_lower} NAME)

	if(filename STREQUAL "stdafx.h")
		message(STATUS "MFC EXT DLL")
		add_definitions(-D_AFXEXT)
		set(CMAKE_MFC_FLAG 2)
	endif() 
endforeach()

# Add executable
ADD_LIBRARY( ${PROJ_NAME} SHARED ${SRC_FILES} )

set_target_properties(${PROJ_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
# link set
target_link_libraries( ${PROJ_NAME} 
                       ${LIBS} 
					   )

