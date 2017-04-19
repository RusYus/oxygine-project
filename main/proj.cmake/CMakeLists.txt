cmake_minimum_required (VERSION 2.6)
project (MyDemo)

add_subdirectory(../../oxygine-framework oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

file(GLOB_RECURSE BOX2DSRC
		../box2d/*.cpp
		../box2d/*.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

add_executable(MyDemo ${BOX2DSRC} ../src/Box2DDebugDraw.cpp ../src/Box2DDebugDraw.h ../src/Game.cpp ../src/Joystick.cpp ../src/Player.cpp ../src/Unit.cpp ../src/example.cpp ../src/main.cpp ../src/res.cpp  ../src/Game.h ../src/Joystick.h ../src/Player.h ../src/Unit.h ../src/example.h ../src/res.h )
source_group(box2d FILES ${BOX2DSRC})
include_directories(../box2d)

target_link_libraries(MyDemo ${OXYGINE_CORE_LIBS})
