project(xadrez)
add_executable(${PROJECT_NAME} main.cpp model.cpp movements.cpp openglwindow.cpp
                               trackball.cpp)
enable_abcg(${PROJECT_NAME})
