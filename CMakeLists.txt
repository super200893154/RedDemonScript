cmake_minimum_required(VERSION 3.16)

project(RedDemonScript VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Widgets Concurrent)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appRedDemonScript
    main.cpp
    src/core/ConfigManager.h
    src/core/ConfigManager.cpp
    src/core/LogManager.h
    src/core/LogManager.cpp
    src/core/ScriptController.h
    src/core/ScriptController.cpp
    src/core/ScriptThread.h
    src/core/ScriptThread.cpp
    src/core/MonitorThread.h
    src/core/MonitorThread.cpp
    src/core/TimerThread.h
    src/core/TimerThread.cpp
    src/interfaces/IGameLauncher.h
    src/interfaces/IDungeonEntry.h
    src/interfaces/IDungeonRunner.h
    src/helpers/ImageHelper.h
    src/helpers/ImageHelper.cpp
    src/helpers/InputHelper.h
    src/helpers/InputHelper.cpp
    src/account/AccountInfo.h
    src/account/AccountManager.h
    src/account/AccountManager.cpp
)

qt_add_qml_module(appRedDemonScript
    URI RedDemonScript
    QML_FILES
        Main.qml
    RESOURCES
        qtquickcontrols2.conf
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appRedDemonScript PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appRedDemonScript
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRedDemonScript
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Widgets
        Qt6::Concurrent
)

# Windows 系统库
if(WIN32)
    target_link_libraries(appRedDemonScript PRIVATE winmm.lib advapi32.lib crypt32.lib)
endif()

# 添加 src 目录到 include 路径
target_include_directories(appRedDemonScript PRIVATE ${CMAKE_SOURCE_DIR}/src)

# 自动部署 Qt DLL (Windows)
if(WIN32)
    # 获取 windeployqt 路径
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET appRedDemonScript POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --qmldir "${CMAKE_SOURCE_DIR}"
                "$<TARGET_FILE:appRedDemonScript>"
            COMMENT "Deploying Qt dependencies..."
        )
    endif()
endif()

include(GNUInstallDirs)
install(TARGETS appRedDemonScript
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
