project(specialchess)
add_executable(${PROJECT_NAME} camera.cpp main.cpp model.cpp movements.cpp openglwindow.cpp
                               trackball.cpp)
enable_abcg(${PROJECT_NAME})
