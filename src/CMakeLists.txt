add_library(matlib STATIC matrix.cpp)
target_include_directories(matlib
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
target_compile_features(matlib PUBLIC cxx_std_20)
target_compile_options(matlib PUBLIC -Wall -Wextra -Wpedantic)
