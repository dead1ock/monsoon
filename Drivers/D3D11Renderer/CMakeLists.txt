CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(RendererD3D11 C CXX)

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(DirectX REQUIRED)

#
# Include/Link Dependencies
#
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/Engine/Include
	${DirectX_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
	${DirectX_LIBRARY_DIR}
)

#
# Platform independent sources.
#
SET(MODULE_SRC
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
ADD_LIBRARY(RendererD3D11 SHARED ${MODULE_SOURCES})
SET_TARGET_PROPERTIES(RendererD3D11 PROPERTIES LINKER_LANGUAGE CXX)
TARGET_LINK_LIBRARIES(RendererD3D11 Engine)