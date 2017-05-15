cmake_minimum_required (VERSION 2.6)
project (MyDemo)

add_subdirectory(../oxygine-framework oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

file(GLOB_RECURSE BOX2DSRC
		./box2d/*.cpp
		./box2d/*.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

set(LAUNCHER MyDemo)
set(PROJECT_LIB ProjectLib)

set(
    PROJECT_HEADERS
    include/Box2DDebugDraw.hpp
    include/Constants.hpp
    include/Game.hpp
	include/BasicCamera.hpp
	include/PlayerEvents.hpp
	include/PlayerEventProxy.hpp
	include/DemoLevel.hpp
	include/Joystick.hpp
	include/Player.hpp
	include/example.hpp
	include/res.hpp
	include/Utils.hpp
)

set(
	PROJECT_SOURCES
	src/Box2DDebugDraw.cpp
	src/DemoLevel.cpp
	src/Joystick.cpp
	src/Player.cpp
	src/example.cpp
	src/main.cpp
	src/res.cpp
	src/BasicCamera.cpp
)

add_library(
    ${PROJECT_LIB}
    STATIC
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
	${LAUNCHER}
	${BOX2DSRC}
)
source_group(box2d FILES ${BOX2DSRC})
include_directories(./box2d)

target_link_libraries(
	${LAUNCHER}
	ProjectLib
	${OXYGINE_CORE_LIBS}
)
