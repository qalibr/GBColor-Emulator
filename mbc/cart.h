#ifndef AYB_CART_H
#define AYB_CART_H

#include "iostream"
#include "vector"
#include "stdexcept"

#include "IMbc.h"
#include "mbc3.h"

class Cartridge {
private:
	std::vector<uint8_t> romData;
	IMbc* mbc;

	void init_mbc();
	static int get_rom_size(uint8_t val);
	static int get_ram_size(uint8_t val);
	std::string get_game_title();
	static std::string format_bytes(int B);

public:
	Cartridge();
	~Cartridge();

	void load_rom();

	[[nodiscard]] IMbc* get_mbc() const {
		return mbc;
	}
};

#endif //AYB_CART_H
