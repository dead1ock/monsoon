PROJECT(InsectContainerOpt)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(DirectX REQUIRED)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/Engine/Include
	${CMAKE_SOURCE_DIR}/Drivers/D3D11Renderer/Include
	${DirectX_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	D:/ovr_sdk_win_0.2.3_public/LibOVR/Include
)

LINK_DIRECTORIES(
	${DirectX_LIBRARY_DIR}
	D:/ovr_sdk_win_0.2.3_public/LibOVR/Lib/x64
)

INCLUDE(Sources.cmake)

ADD_EXECUTABLE(InsectContainerOpt WIN32 ${MODULE_SOURCES})
TARGET_LINK_LIBRARIES(InsectContainerOpt Engine)
TARGET_LINK_LIBRARIES(InsectContainerOpt RendererD3D11)
TARGET_LINK_LIBRARIES(InsectContainerOpt ${DirectX_LIBRARY})
TARGET_LINK_LIBRARIES(InsectContainerOpt libovr64)
TARGET_LINK_LIBRARIES(InsectContainerOpt winmm)