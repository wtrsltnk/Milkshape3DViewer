cmake_minimum_required(VERSION 3.1)

project(WrapperTest)

find_package(OpenGL REQUIRED)

add_executable(WrapperTest
    CControl.cpp
    CMainFrame.cpp
    COpenGL.cpp
    CWin.cpp
    MilkshapeModel.cpp
    Model.cpp
    Window.cpp
    WinMain.cpp
    stb.cpp
    )

target_compile_features(WrapperTest
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    )

target_link_libraries(WrapperTest
    ${OPENGL_LIBRARIES}
    )
