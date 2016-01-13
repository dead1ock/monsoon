PROJECT(Engine)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(Boost REQUIRED)

INCLUDE_DIRECTORIES(
	${Boost_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
)

INCLUDE_DIRECTORIES(Include/)

INCLUDE(Sources.cmake)

IF(MSVC)
	# This disables a warning about exporting classes with stl members which
	# don't have a DLL interface. Since none of these members should be anything but
	# private, we can disable this warning. A possible fix in the future is to
	# create classes which encapsulate the stl functionality and which are exported.
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
ENDIF(MSVC)

ADD_DEFINITIONS(-DMONSOON_DYNLIB_EXPORT)
ADD_LIBRARY(Engine SHARED ${MODULE_SOURCES})
SET_TARGET_PROPERTIES(Engine PROPERTIES LINKER_LANGUAGE CXX)
