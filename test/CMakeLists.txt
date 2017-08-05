set(PROJECT_TESTS ProjectTests)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(
	PROJECT_TESTS_SOURCES

	CollisionManagerTests.cpp
    JsonImporterTests.cpp
	TestExample.cpp
	Tests.cpp
)

add_executable(
    ${PROJECT_TESTS}
    ${PROJECT_TESTS_SOURCES}
)

target_link_libraries(
    ${PROJECT_TESTS}
    ${PROJECT_LIB}
    ${PROJECT_USED_LIBRARIES}
)