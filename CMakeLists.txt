cmake_minimum_required(VERSION 3.10)
project(TOTAL C)

# 设置交叉编译工具链
set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc)  # Arm32
#set(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc-8")  # 旧版编译器
#set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)  # Arm64

set(CMAKE_CROSSCOMPILING ON)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 设置
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")


# 头文件
include_directories(LCD/include)
include_directories(UART/include)
include_directories(I2C/include)
include_directories(/include)

# 源文件
file(GLOB UART_SOURCES "${PROJECT_SOURCE_DIR}/UART/src/*.c")
file(GLOB LCD_SOURCES "${PROJECT_SOURCE_DIR}/LCD/src/*.c")
file(GLOB I2C_SOURCES "${PROJECT_SOURCE_DIR}/I2C/src/*.c")
file(GLOB TOTAL_SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")


add_library(UART SHARED ${UART_SOURCES})
add_library(I2C SHARED ${I2C_SOURCES})
add_library(LCD SHARED ${LCD_SOURCES})
target_link_libraries(LCD PUBLIC pthread)

add_executable(${PROJECT_NAME} ${TOTAL_SOURCES})
target_link_libraries(TOTAL PRIVATE LCD UART I2C)


# 再编译一份
set(EXECUTABLE_OUTPUT_PATH  "/home/dongyu")
set(LIBRARY_OUTPUT_PATH  "/home/dongyu")
add_library(uart SHARED ${UART_SOURCES})
add_library(lcd SHARED ${LCD_SOURCES})
add_library(i2c SHARED ${I2C_SOURCES})
add_executable(total ${TOTAL_SOURCES})
target_link_libraries(total PRIVATE lcd uart i2c)

#add_executable(uart_test ${UART_SOURCES})
