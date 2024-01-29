cmake_minimum_required(VERSION 3.27)
project(Ayb)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_PATH_DESKTOP "C:/Users/Defeated/_Repo/_Libraries/SDL2-2.28.5")
set(SDL2_PATH_LAPTOP "C:/Users/jorge/_repo/_resources/SDL2-2.28.5")

if (EXISTS "${SDL2_PATH_DESKTOP}/x86_64-w64-mingw32")
    set(SDL2_PATH "${SDL2_PATH_DESKTOP}")
elseif (EXISTS "${SDL2_PATH_LAPTOP}/x86_64-w64-mingw32")
    set(SDL2_PATH "${SDL2_PATH_LAPTOP}")
else ()
    message(FATAL_ERROR "SDL2 not found")
endif ()

include_directories("${SDL2_PATH}/x86_64-w64-mingw32/include")
link_directories("${SDL2_PATH}/x86_64-w64-mingw32/lib")

include_directories(header)
include_directories(instr)
include_directories(utils)

link_directories("C:/Users/Defeated/_Repo/_Libraries/SDL2-2.28.5/x86_64-w64-mingw32/lib")

add_executable(Ayb src/main.cpp
        header/screen.h
        src/screen.cpp
        utils/bit_util.h
        memory/mmu.h
        memory/mem.h
        memory/mem_map.h
        memory/hw_reg_addr.h
        memory/hw_reg.h
        memory/hw_reg.cpp
        mbc/IMbc.h
        mbc/mbc3.h
        memory/mmu.cpp
        memory/mem_util.h
        memory/mem_util.cpp
        mbc/cart.h
        mbc/cart.cpp
        mbc/mbc3.cpp
        cpu/cpu_reg.h
        cpu/cpu.h
        cpu/cpu_reg.cpp
        cpu/cpu.cpp
        cpu/cpu_flag.h
        cpu/cpu_flag.cpp
        instr/operator.h
        header/timer.h
        src/timer.cpp
        utils/bit_util.cpp
        memory/int_ctrl.h
        memory/int_ctrl.cpp
        instr/operator.cpp
        instr/operator.cpp
        instr/instructions.cpp
        instr/opcode_map.cpp
        instr/instr_debug.h
        utils/util.cpp
        utils/util.h
)

target_link_libraries(Ayb mingw32 SDL2main SDL2)
