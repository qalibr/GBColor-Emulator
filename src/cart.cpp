#include "../header/cart.h"

#include "fstream"
#include "sstream"
#include "iomanip"
#include "algorithm"

Cartridge::Cartridge() : mbc(nullptr)
{
}
Cartridge::~Cartridge()
{
	delete mbc;
}
void Cartridge::init_mbc()
{
	if (romData.empty())
	{
		throw std::runtime_error("ROM data is empty");
	}

	switch (romData[0x147])
	{
	case 0x00:
		throw std::runtime_error("Unsupported MBC");
	case 0x01:
	case 0x02:
	case 0x03:
		throw std::runtime_error("Unsupported MBC");
	case 0x05:
	case 0x06:
		throw std::runtime_error("Unsupported MBC");
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		// Load mbc3
		mbc = new Mbc3(romData, get_rom_size(romData[0x148]), get_ram_size(romData[0x149]));
		break;
	case 0x15:
	case 0x16:
	case 0x17:
		throw std::runtime_error("Unsupported MBC");
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
		throw std::runtime_error("Unsupported MBC");
	case 0xFC:
	case 0xFD:
	case 0xFE:
	case 0xFF:
		throw std::runtime_error("Unsupported MBC");
	default:
		throw std::runtime_error("Unknown address, cannot determine MBC");
	}

	// Write info to console
	std::cout << "Game Title:     " << get_game_title() << std::endl;
	std::cout << "ROM Size:       " << format_bytes(get_rom_size(romData[0x148])) << std::endl;
	std::cout << "RAM Size:       " << format_bytes(get_ram_size(romData[0x149])) << std::endl;
	std::cout << "MBC Hex:        0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
			  << static_cast<int>(romData[0x147]) << std::endl;
}
void Cartridge::load_rom()
{
	std::string path_desktop = R"(C:\Users\Defeated\_Repo\_resources\PKMC.gbc)";
	std::string path_laptop  = R"(C:\Users\jorge\Documents\PKMC.gbc)";

	std::vector<std::string> paths = { path_desktop, path_laptop };

	for (const auto& path: paths)
	{
		std::ifstream file(path, std::ios::binary);
		if (file)
		{
			romData = std::vector<uint8_t>(std::istreambuf_iterator<char>(file),
					std::istreambuf_iterator<char>());

			if (!romData.empty())
			{
				std::cout << "Cart: ROM loaded successfully" << std::endl;
			}

			init_mbc();
			return;
		}
	}

	throw std::runtime_error("Failed to load ROM");
}
int Cartridge::get_rom_size(uint8_t val)
{
	switch (val)
	{
	case 0x00:
		return 32768;
	case 0x01:
		return 65536;
	case 0x02:
		return 131072;
	case 0x03:
		return 262144;
	case 0x04:
		return 524288;
	case 0x05:
		return 1048576;
	case 0x06:
		return 2097152;
	case 0x07:
		return 4194304;
	case 0x08:
		return 8388608;
	case 0x52:
		return 1179648;
	case 0x53:
		return 1310720;
	case 0x54:
		return 1572864;
	default:
		throw std::invalid_argument("Invalid ROM size: " + std::to_string(val));
	}
}
int Cartridge::get_ram_size(uint8_t val)
{
	switch (val)
	{
	case 0x00:
		return 0;
	case 0x01:
		return 2048;
	case 0x02:
		return 8192;
	case 0x03:
		return 32768;
	default:
		throw std::invalid_argument("Invalid RAM size: " + std::to_string(val));
	}
}

std::string Cartridge::get_game_title()
{
	std::string title;

	for (int i = 0x134; i <= 0x143; i++)
	{
		char c = romData[i];
		if (c < 32 || c > 126)
		{
			break;
		}

		title += c;
	}

	return title;
}
std::string Cartridge::format_bytes(int B)
{
	std::vector<std::string> suffixes = { "B", "KB", "MB", "GB", "TB" };
	double                   len      = B;
	int                      order    = 0;

	while (len >= 1024 && order < suffixes.size() - 1)
	{
		order++;
		len /= 1024;
	}

	std::stringstream ss;

	ss << std::fixed << std::setprecision(2) << len << " " << suffixes[order];

	return ss.str();
}