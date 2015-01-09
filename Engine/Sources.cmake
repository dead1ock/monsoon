#
# Sources.cmake
# This file contains a list of all sources which will be included in this module.
# *** NO GLOBBING ***
#

SET(MODULE_SRC
	#Public Sources (Include/)
	Include/App/Application.h
	Include/ECS/EntityManager.h
	Include/ECS/System.h
	Include/Event/EventManager.h
	Include/Math/AABB.h
	Include/Platform/Clock.h
	Include/Platform/DynLib.h
	Include/Platform/Endianness.h
	Include/Platform/Export.h
	Include/Platform/Log.h
	Include/Platform/Platform.h
	Include/Platform/Predef.h
	Include/Platform/Types.h
	Include/Resource/ResourceManager.h
	Include/Renderer/Renderer.h
	Include/Renderer/RendererSettings.h
	Include/Renderer/VertexBuffer.h
	Include/Scene/SpatialSystem.h
	Include/Util/PackedPool.h
	
	# Private Sources (Source/)
	Src/App/Application.cpp
	Src/ECS/EntityMAnager.cpp
	Src/ECS/System.cpp
	Src/Event/EventManager.cpp
	Src/Math/AABB.cpp
	Src/Platform/Clock.cpp
	Src/Platform/Log.cpp
	Src/Resource/ResourceManager.cpp
	Src/Scene/SpatialSystem.cpp
	)

#
# Platform dependent sources.
#
IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	SET(MODULE_PLATFORM_SRC
		
		)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET(MODULE_PLATFORM_SRC
		)
ENDIF()

#
# Merge sources.
#
SET(MODULE_SOURCES ${MODULE_SRC} ${MODULE_PLATFORM_SRC} Sources.cmake)

#
# Define source groups.
#
SOURCE_GROUP(App "App/*")
SOURCE_GROUP(Platform "Platform/*")
SOURCE_GROUP(Renderer "Renderer/*")
SOURCE_GROUP(Scene "Scene/*")
SOURCE_GROUP(Util "Util/*")
SOURCE_GROUP(ECS "ECS/*")
SOURCE_GROUP(Log "Log/*")
SOURCE_GROUP(Math "Math/*")
SOURCE_GROUP(Event "Event/*")
SOURCE_GROUP(Resource "Resource/*")