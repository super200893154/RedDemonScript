cmake_minimum_required(VERSION 3.16)

project(ConfigManagerTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(test_config_manager
    test_config_manager.cpp
    ../src/core/ConfigManager.cpp
    ../src/core/ConfigManager.h
    ../src/core/LogManager.cpp
    ../src/core/LogManager.h
    ../src/helpers/DpapiHelper.cpp
    ../src/helpers/DpapiHelper.h
)

target_link_libraries(test_config_manager
    PRIVATE
        Qt6::Core
)

if(WIN32)
    target_link_libraries(test_config_manager PRIVATE advapi32.lib crypt32.lib)
endif()

target_include_directories(test_config_manager PRIVATE ${CMAKE_SOURCE_DIR}/../src)

# 设置测试工作目录
set_target_properties(test_config_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
