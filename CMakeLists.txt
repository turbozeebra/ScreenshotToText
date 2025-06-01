cmake_minimum_required(VERSION 3.5)
project( ImageToText )
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets
find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
# Include the wxWidgets use file
include(${wxWidgets_USE_FILE})
endif()


add_executable(ScreenshotToText ./src/main.cpp ./src/ControlPanelFrame.cpp ./src/ScreenShotFrame.cpp ./src/BasicPane.cpp) # ./src/ControlPanelFrame.cpp   ./src/BasicButton.cpp)

target_link_libraries(ScreenshotToText -llept -ltesseract) # must add this line to link Tesseract and leptonica libs
# Link wxWidgets libraries
target_link_libraries(ScreenshotToText ${wxWidgets_LIBRARIES})
