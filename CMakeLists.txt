cmake_minimum_required(VERSION 3.10)
project(TOTAL C)

# 设置交叉编译工具链
set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc)  # Arm32
#set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)  # Arm64

set(CMAKE_CROSSCOMPILING ON)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 设置
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")


# 头文件
include_directories(LCD/include)
include_directories(UART/include)
include_directories(/include)

# 源文件
file(GLOB UART_SOURCES "${PROJECT_SOURCE_DIR}/UART/src/*.c")
file(GLOB LCD_SOURCES "${PROJECT_SOURCE_DIR}/LCD/src/*.c")
file(GLOB TOTAL_SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")


add_library(UART SHARED ${UART_SOURCES})
add_library(LCD SHARED ${LCD_SOURCES})



add_executable(UART_test ${UART_SOURCES})

add_executable(${PROJECT_NAME} ${TOTAL_SOURCES})
target_link_libraries(TOTAL PRIVATE LCD UART)