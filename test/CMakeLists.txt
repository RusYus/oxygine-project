set(PROJECT_TESTS ProjectTests)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(
    PROJECT_TESTS_HEADERS
    DynamicBoxFake.hpp
    PlatformFake.hpp
    PlayerFake.hpp
)

set(
	PROJECT_TESTS_SOURCES
	CollisionManagerTests.cpp
    DynamicBoxFake.cpp
    JsonImporterTests.cpp
    PlatformFake.cpp
    PlayerFake.cpp
	TestExample.cpp
	Tests.cpp
    VelocityTests.cpp
)

add_executable(
    ${PROJECT_TESTS}
    ${PROJECT_TESTS_SOURCES}
)

target_link_libraries(
    ${PROJECT_TESTS}
    ${PROJECT_LIB}
    ${OXYGINE_CORE_LIBS}
    ${PROJECT_USED_LIBRARIES}
)