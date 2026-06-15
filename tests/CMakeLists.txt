Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests
    test_construction.cpp
    test_arithmetic.cpp
    test_norm.cpp
    test_utility.cpp
    test_misc.cpp
    test_iterators.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE matlib)
include(CTest)
include(Catch)
catch_discover_tests(tests)