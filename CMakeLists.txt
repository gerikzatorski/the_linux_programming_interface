cmake_minimum_required(VERSION 3.10)

# Use default C compiler
project(tlpi_exercises C)

# Include TLPI header and link library when modifying book code
set(TLPI_INCLUDES ${CMAKE_SOURCE_DIR}/tlpi-dist/lib)
set(TLPI_LIBS ${CMAKE_SOURCE_DIR}/tlpi-dist/libtlpi.a)

add_executable(exercise_03_01 chapter_03/exercise_03_01.c)

add_executable(exercise_04_01 chapter_04/exercise_04_01.c)
add_executable(exercise_04_02 chapter_04/exercise_04_02.c)

add_executable(exercise_05_01 chapter_05/exercise_05_01.c)
target_include_directories(exercise_05_01 PRIVATE ${TLPI_INCLUDES})
target_link_libraries(exercise_05_01 PRIVATE ${TLPI_LIBS})
add_executable(exercise_05_02 chapter_05/exercise_05_02.c)
add_executable(exercise_05_03 chapter_05/exercise_05_03.c)
add_executable(exercise_05_05 chapter_05/exercise_05_05.c)
add_executable(exercise_05_07 chapter_05/exercise_05_07.c)

add_executable(exercise_06_02 chapter_06/exercise_06_02.c)
add_executable(exercise_06_03 chapter_06/exercise_06_03.c)

add_executable(exercise_07_01 chapter_07/exercise_07_01.c)
target_include_directories(exercise_07_01 PRIVATE ${TLPI_INCLUDES})
target_link_libraries(exercise_07_01 PRIVATE ${TLPI_LIBS})

add_executable(exercise_08_01 chapter_08/exercise_08_01.c)
add_executable(exercise_08_02 chapter_08/exercise_08_02.c)

add_executable(exercise_09_03 chapter_09/exercise_09_03.c)

add_executable(exercise_12_01 chapter_12/exercise_12_01.c)
add_executable(exercise_12_02 chapter_12/exercise_12_02.c)
add_executable(exercise_12_03 chapter_12/exercise_12_03.c)

add_executable(exercise_13_05 chapter_13/exercise_13_05.c)

add_executable(exercise_14_01 chapter_14/exercise_14_01.c)

add_executable(exercise_15_03 chapter_15/exercise_15_03.c)
target_include_directories(exercise_15_03 PRIVATE ${TLPI_INCLUDES})
target_link_libraries(exercise_15_03 PRIVATE ${TLPI_LIBS})
add_executable(exercise_15_04 chapter_15/exercise_15_04.c)
add_executable(exercise_15_06 chapter_15/exercise_15_06.c)
add_executable(exercise_15_07 chapter_15/exercise_15_07.c)

add_executable(exercise_16_01 chapter_16/exercise_16_01.c)

add_executable(exercise_17_01 chapter_17/exercise_17_01.c)
target_link_libraries(exercise_17_01 acl)

add_executable(exercise_18_03 chapter_18/exercise_18_03.c)
add_executable(exercise_18_04 chapter_18/exercise_18_04.c)
target_include_directories(exercise_18_04 PRIVATE ${TLPI_INCLUDES})
target_link_libraries(exercise_18_04 PRIVATE ${TLPI_LIBS})
add_executable(exercise_18_05 chapter_18/exercise_18_05.c)
add_executable(exercise_18_07 chapter_18/exercise_18_07.c)
add_executable(exercise_18_09 chapter_18/exercise_18_09.c)

add_executable(exercise_19_01 chapter_19/exercise_19_01.c)

add_executable(exercise_20_01 chapter_20/exercise_20_01.c)
target_include_directories(exercise_20_01 PRIVATE ${TLPI_INCLUDES})
target_link_libraries(exercise_20_01 PRIVATE ${TLPI_LIBS})
add_executable(exercise_20_02 chapter_20/exercise_20_02.c)
add_executable(exercise_20_03a chapter_20/exercise_20_03a.c)
add_executable(exercise_20_03b chapter_20/exercise_20_03b.c)
add_executable(exercise_20_04 chapter_20/exercise_20_04.c)

add_executable(exercise_21_01 chapter_21/exercise_21_01.c)
