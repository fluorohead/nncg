cmake_minimum_required(VERSION 3.16)
project(nncg VERSION 0.1.1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(nncg WIN32 MACOSX_BUNDLE
    buttons.cpp buttons.h
    common.h
    csv.cpp csv.h
    main.cpp
    mwindow.cpp mwindow.h
    settings.cpp settings.h
    table.cpp table.h
    template.cpp template.h
    validators.cpp validators.h
    updater.cpp updater.h
)
target_compile_definitions(nncg PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(nncg PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
)


# Resources:
set(nncg_resource_files
    "ca.png"
    "mt.png"
    "mth.png"
    "mtp.png"
    "rtr.png"
    "sep.png"
    "tm.png"
    "tmh.png"
    "tmp.png"
)

qt_add_resources(nncg "nncg"
    PREFIX
        "/"
    FILES
        ${nncg_resource_files}
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(nncg PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS nncg
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET nncg
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
