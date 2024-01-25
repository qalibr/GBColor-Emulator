cmake_minimum_required(VERSION 3.27)
project(Ayb)

set(CMAKE_CXX_STANDARD 17)

# include_directories("C:/Users/Defeated/_Repo/_Libraries/SDL2-2.28.5/x86_64-w64-mingw32/include")

set(SDL2_PATH_DESKTOP "C:/Users/Defeated/_Repo/_Libraries/SDL2-2.28.5")
set(SDL2_PATH_LAPTOP "C:/Users/jorge/_repo/_resources/SDL2-2.28.5")

if(EXISTS "${SDL2_PATH_DESKTOP}/x86_64-w64-mingw32")
    set(SDL2_PATH "${SDL2_PATH_DESKTOP}")
elseif(EXISTS "${SDL2_PATH_LAPTOP}/x86_64-w64-mingw32")
    set(SDL2_PATH "${SDL2_PATH_LAPTOP}")
else()
    message(FATAL_ERROR "SDL2 not found")
endif()

include_directories("${SDL2_PATH}/x86_64-w64-mingw32/include")
link_directories("${SDL2_PATH}/x86_64-w64-mingw32/lib")

include_directories(header)

link_directories("C:/Users/Defeated/_Repo/_Libraries/SDL2-2.28.5/x86_64-w64-mingw32/lib")

add_executable(Ayb main.cpp
        header/screen.h
        src/screen.cpp
        header/util.h
        header/mmu.h
        header/mem.h
        header/mem_map.h
        header/hw_reg_addr.h
        header/hw_reg.h
        src/hw_reg.cpp
        header/IMbc.h
        header/mbc3.h
        src/mmu.cpp
        header/mem_util.h
        src/mem_util.cpp
        header/cart.h
        src/cart.cpp
        src/mbc3.cpp)

target_link_libraries(Ayb mingw32 SDL2main SDL2)
