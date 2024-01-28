#include "iostream"

#include "SDL2/SDL.h"

#include "cart.h"
#include "cpu.h"
#include "hw_reg.h"
#include "operator.h"

int main(int argc, char* argv[]) {
	Cartridge cart;
	cart.load_rom();
	HardwareRegisters hw_reg;
	Mmu               mmu(hw_reg, cart.get_mbc());
	Cpu               cpu(hw_reg, cart.get_mbc(), mmu);
	OpcodeMap         opcode_map(cart.get_mbc());

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	try {
		int loop = 0;
		while (loop < 100) {
			loop++;
			opcode_map.execute(cpu.fetch_opcode());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
