#
# Sources.cmake
# This file contains a list of all sources which will be included in this module.
# *** NO GLOBBING ***
#

#
# Platform dependent sources.
#
IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	SET(MODULE_PLATFORM_SRC
		Include/D3D11ColorMaterial.h
		Include/D3D.h
		Include/D3D11GradientSkydomeMaterial.h
		Include/D3D11BarrelDistortionPostFx.h
		Include/D3D11Renderer.h
		Include/D3D11SpriteMaterial.h
		Include/D3D11TextureMaterial.h
		Include/D3D11VertexBuffer.h
		Include/D3D11Window.h

		Src/D3D11ColorMaterial.cpp
		Src/D3D.cpp
		Src/D3D11BarrelDistortionPostFx.cpp
		Src/D3D11GradientSkydomeMaterial.cpp
		Src/D3D11Renderer.cpp
		Src/D3D11SpriteMaterial.cpp
		Src/D3D11TextureMaterial.cpp
		Src/D3D11VertexBuffer.cpp
		Src/D3D11Window.cpp
		)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET(MODULE_PLATFORM_SRC
		)
ENDIF()

#
# Merge sources.
#
SET(MODULE_SOURCES ${MODULE_SRC} ${MODULE_PLATFORM_SRC} Sources.cmake)