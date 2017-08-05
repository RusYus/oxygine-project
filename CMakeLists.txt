cmake_minimum_required (VERSION 2.6)
project (MyDemo)

add_subdirectory(../oxygine-framework oxygine-framework)
add_definitions(
    -std=gnu++14
    ${OXYGINE_DEFINITIONS}
)
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
    include/BasisCamera.hpp
    include/BasisEventProxy.hpp
    include/BasisEvents.hpp
    include/BasisObject.hpp
    include/Box2DDebugDraw.hpp
    include/CollisionManager.hpp
    include/Constants.hpp
    include/DemoLevel.hpp
    include/example.hpp
    include/Game.hpp
    include/JsonImporter.hpp
    include/MapProperty.hpp
    include/MovementButton.hpp
    include/Player.hpp
    include/Platform.hpp
    include/res.hpp
    include/Utils.hpp
)

set(
	PROJECT_SOURCES
    src/BasisCamera.cpp
    src/Box2DDebugDraw.cpp
    src/CollisionManager.cpp
    src/DemoLevel.cpp
    src/example.cpp
    src/JsonImporter.cpp
    src/main.cpp
    src/Player.cpp
    src/Platform.cpp
    src/res.cpp
    src/Utils.cpp
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
	${PROJECT_LIB}
	${OXYGINE_CORE_LIBS}
)

# === FOR TESTS ===
set(INSTALED_DEPENDENCIES_DIR /usr)
set(BOOST_INCLUDES ${INSTALED_DEPENDENCIES_DIR}/include)
set(BOOST_LINK ${INSTALED_DEPENDENCIES_DIR}/lib)
set(
    BOOST_LIBS
    boost_filesystem
    boost_system
    boost_thread
    boost_log_setup
    boost_log
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_INCLUDES}
)

link_directories(
    ${BOOST_LINK}
)

set(
	PROJECT_USED_LIBRARIES
    ${BOOST_LIBS}
)

add_subdirectory(test)
