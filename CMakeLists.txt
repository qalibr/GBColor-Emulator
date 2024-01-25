cmake_minimum_required(VERSION 3.27)
project(Ayb)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/Users/Defeated/_Repo/_Libraries/SDL2-2.28.5/x86_64-w64-mingw32/include")

include_directories(header)

link_directories("C:/Users/Defeated/_Repo/_Libraries/SDL2-2.28.5/x86_64-w64-mingw32/lib")

add_executable(Ayb main.cpp
        header/Screen.h
        src/Screen.cpp)

target_link_libraries(Ayb mingw32 SDL2main SDL2)
