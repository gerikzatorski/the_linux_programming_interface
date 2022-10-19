cmake_minimum_required(VERSION 3.10)

# use default C compiler
project(tlpi_exercises C)

# include tlpi headers
# include_directories(${CMAKE_SOURCE_DIR}/tlpi-dist/lib)

# link to tlpi lib
# link_libraries(${CMAKE_SOURCE_DIR}/tlpi-dist/libtlpi.a)

add_executable(exercise_03_01 chapter_03/exercise_03_01.c)

add_executable(exercise_04_01 chapter_04/exercise_04_01.c)
add_executable(exercise_04_02 chapter_04/exercise_04_02.c)
