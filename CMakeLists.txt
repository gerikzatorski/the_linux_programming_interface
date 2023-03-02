cmake_minimum_required(VERSION 3.10)

# Use default C compiler
project(tlpi_exercises C)

# enable CTest testing
enable_testing()

# Need TLPI handers and lib (ex. error functions)
file(GLOB TLPI_SRCS lib/*.c)
include_directories(${CMAKE_SOURCE_DIR}/lib)
add_library(tlpi STATIC ${TLPI_SRCS})
link_libraries(tlpi)

add_executable(exercise_03_01 chapter_03/exercise_03_01.c)

add_executable(exercise_04_01 chapter_04/exercise_04_01.c)
add_executable(exercise_04_02 chapter_04/exercise_04_02.c)

add_executable(exercise_05_01 chapter_05/exercise_05_01.c)
add_executable(exercise_05_02 chapter_05/exercise_05_02.c)
add_executable(exercise_05_03 chapter_05/exercise_05_03.c)
add_executable(exercise_05_05 chapter_05/exercise_05_05.c)
add_executable(exercise_05_07 chapter_05/exercise_05_07.c)

add_executable(exercise_06_02 chapter_06/exercise_06_02.c)
add_executable(exercise_06_03 chapter_06/exercise_06_03.c)

add_executable(exercise_07_01 chapter_07/exercise_07_01.c)

add_executable(exercise_08_01 chapter_08/exercise_08_01.c)
add_executable(exercise_08_02 chapter_08/exercise_08_02.c)

add_executable(exercise_09_03 chapter_09/exercise_09_03.c)

add_executable(exercise_12_01 chapter_12/exercise_12_01.c)
add_executable(exercise_12_02 chapter_12/exercise_12_02.c)
add_executable(exercise_12_03 chapter_12/exercise_12_03.c)

add_executable(exercise_13_05 chapter_13/exercise_13_05.c)

add_executable(exercise_14_01 chapter_14/exercise_14_01.c)

add_executable(exercise_15_03 chapter_15/exercise_15_03.c)
add_executable(exercise_15_04 chapter_15/exercise_15_04.c)
add_executable(exercise_15_06 chapter_15/exercise_15_06.c)
add_executable(exercise_15_07 chapter_15/exercise_15_07.c)

add_executable(exercise_16_01 chapter_16/exercise_16_01.c)

add_executable(exercise_17_01 chapter_17/exercise_17_01.c)
target_link_libraries(exercise_17_01 acl)

add_executable(exercise_18_03 chapter_18/exercise_18_03.c)
add_executable(exercise_18_04 chapter_18/exercise_18_04.c)
add_executable(exercise_18_05 chapter_18/exercise_18_05.c)
add_executable(exercise_18_07 chapter_18/exercise_18_07.c)
add_executable(exercise_18_09 chapter_18/exercise_18_09.c)

add_executable(exercise_19_01 chapter_19/exercise_19_01.c)

add_executable(exercise_20_01 chapter_20/exercise_20_01.c)
add_executable(exercise_20_02 chapter_20/exercise_20_02.c)
add_executable(exercise_20_03a chapter_20/exercise_20_03a.c)
add_executable(exercise_20_03b chapter_20/exercise_20_03b.c)
add_executable(exercise_20_04 chapter_20/exercise_20_04.c)

add_executable(exercise_21_01 chapter_21/exercise_21_01.c)

add_executable(exercise_22_01 chapter_22/exercise_22_01.c)
add_executable(exercise_22_02 chapter_22/exercise_22_02.c)
add_executable(exercise_22_03 chapter_22/exercise_22_03.c)

add_executable(exercise_23_01 chapter_23/exercise_23_01.c)
add_executable(exercise_23_03 chapter_23/exercise_23_03.c)
target_link_libraries(exercise_23_03 rt)
add_executable(exercise_23_04 chapter_23/exercise_23_04.c)
target_link_libraries(exercise_23_04 PRIVATE rt)

add_executable(exercise_24_02 chapter_24/exercise_24_02.c)
add_executable(exercise_24_05 chapter_24/exercise_24_05.c)

add_executable(exercise_25_01 chapter_25/exercise_25_01.c)

add_executable(exercise_26_01 chapter_26/exercise_26_01.c)
add_executable(exercise_26_02 chapter_26/exercise_26_02.c)
add_executable(exercise_26_03 chapter_26/exercise_26_03.c)
add_executable(exercise_26_04 chapter_26/exercise_26_04.c)

add_executable(exercise_27_02 chapter_27/exercise_27_02.c)
add_executable(exercise_27_06 chapter_27/exercise_27_06.c)

add_executable(exercise_28_01a chapter_28/exercise_28_01a.c)
add_executable(exercise_28_01b chapter_28/exercise_28_01b.c)

add_executable(exercise_29_01 chapter_29/exercise_29_01.c)
target_link_libraries(exercise_29_01 PRIVATE pthread)

add_executable(exercise_30_01 chapter_30/exercise_30_01.c)
target_link_libraries(exercise_30_01 PRIVATE pthread)
# library and pseudo tests in seperate files
add_library(exercise_30_02_lib STATIC chapter_30/exercise_30_02.c)
target_link_libraries(exercise_30_02_lib PRIVATE pthread)
add_executable(exercise_30_02_tests chapter_30/exercise_30_02_tests.c)
target_link_libraries(exercise_30_02_tests PRIVATE pthread exercise_30_02_lib)
add_test(test_30_02 exercise_30_02_tests)

add_executable(exercise_31_01 chapter_31/exercise_31_01.c)
target_link_libraries(exercise_31_01 PRIVATE pthread)
add_executable(exercise_31_02a chapter_31/exercise_31_02a.c)
target_link_libraries(exercise_31_02a PRIVATE pthread)
add_executable(exercise_31_02b chapter_31/exercise_31_02b.c)
target_link_libraries(exercise_31_02b PRIVATE pthread)

add_executable(exercise_33_01 chapter_33/exercise_33_01.c)
target_link_libraries(exercise_33_01 PRIVATE pthread)

add_executable(exercise_34_02 chapter_34/exercise_34_02.c)
add_executable(exercise_34_03 chapter_34/exercise_34_03.c)
add_executable(exercise_34_04 chapter_34/exercise_34_04.c)
add_executable(exercise_34_06 chapter_34/exercise_34_06.c)
add_executable(exercise_34_07 chapter_34/exercise_34_07.c)

add_executable(exercise_35_01 chapter_35/exercise_35_01.c)
add_executable(exercise_35_02 chapter_35/exercise_35_02.c)
add_executable(exercise_35_03 chapter_35/exercise_35_03.c)

add_executable(exercise_36_01 chapter_36/exercise_36_01.c)
add_executable(exercise_36_02 chapter_36/exercise_36_02.c)
add_executable(exercise_36_03 chapter_36/exercise_36_03.c)

add_executable(exercise_37_01 chapter_37/exercise_37_01.c)

add_executable(exercise_38_02 chapter_38/exercise_38_02.c)
target_link_libraries(exercise_38_02 PRIVATE crypt)

add_executable(exercise_39_01 chapter_39/exercise_39_01.c)
target_link_libraries(exercise_39_01 PRIVATE cap)

# chapter 41 exercise 1 uses gcc cli

# chapter 42 exercise 1 uses gcc cli
add_executable(exercise_42_02 chapter_42/exercise_42_02.c)
target_link_libraries(exercise_42_02 PRIVATE dl)

add_executable(exercise_44_01 chapter_44/exercise_44_01.c)
add_executable(exercise_44_02 chapter_44/exercise_44_02.c)
add_executable(exercise_44_03 chapter_44/exercise_44_03.c)
add_executable(exercise_44_04 chapter_44/exercise_44_04.c)
add_executable(exercise_44_06 chapter_44/exercise_44_06.c)
add_executable(exercise_44_06_malicious chapter_44/exercise_44_06_malicious.c)

add_executable(exercise_49_01 chapter_49/exercise_49_01.c)
add_executable(exercise_49_03a chapter_49/exercise_49_03a.c)
add_executable(exercise_49_03b chapter_49/exercise_49_03b.c)
add_executable(exercise_49_04 chapter_49/exercise_49_04.c)

add_executable(exercise_50_01 chapter_50/exercise_50_01.c)
add_executable(exercise_50_02 chapter_50/exercise_50_02.c)

add_executable(exercise_52_01 chapter_52/exercise_52_01.c)
target_link_libraries(exercise_52_01 rt)
add_executable(exercise_52_02_server chapter_52/exercise_52_02_server.c)
target_link_libraries(exercise_52_02_server rt)
add_executable(exercise_52_02_client chapter_52/exercise_52_02_client.c)
target_link_libraries(exercise_52_02_client rt)
add_executable(exercise_52_05 chapter_52/exercise_52_05.c)
target_link_libraries(exercise_52_05 rt)
add_executable(exercise_52_06 chapter_52/exercise_52_06.c)
target_link_libraries(exercise_52_06 rt)

add_executable(exercise_57_01_server chapter_57/exercise_57_01_server.c)
add_executable(exercise_57_01_client chapter_57/exercise_57_01_client.c)
add_executable(exercise_57_02_server chapter_57/exercise_57_02_server.c)
add_executable(exercise_57_02_client chapter_57/exercise_57_02_client.c)
add_executable(exercise_57_03_server chapter_57/exercise_57_03_server.c)
add_executable(exercise_57_03_client chapter_57/exercise_57_03_client.c)
add_executable(exercise_57_04 chapter_57/exercise_57_04.c)
