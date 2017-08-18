cmake_minimum_required (VERSION 2.6)
project (MyDemo)

add_subdirectory(../oxygine-framework oxygine-framework)
add_definitions(
    -std=gnu++14
    ${OXYGINE_DEFINITIONS}
)
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

set(LAUNCHER MyDemo)
set(PROJECT_LIB ProjectLib)

set(
    PROJECT_HEADERS
    include/BasisCamera.hpp
    include/BasisEventProxy.hpp
    include/BasisEvents.hpp
    include/BasisObject.hpp
    include/CollisionInfo.hpp
    include/CollisionManager.hpp
    include/Constants.hpp
    include/DemoLevel.hpp
    include/example.hpp
    include/Game.hpp
    include/Ground.hpp
    include/IDrawable.hpp
    include/IDebugDrawable.hpp
    include/IMovable.hpp
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
    src/CollisionManager.cpp
    src/DemoLevel.cpp
    src/example.cpp
    src/IMovable.cpp
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
    ${PROJECT_SOURCES}
)

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
