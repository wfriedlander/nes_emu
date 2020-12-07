#include "cartridge.h"
#include "mappers.h"

#include <filesystem>

#include <QDebug>

std::unique_ptr<Cartridge> Cartridge::Load(std::string filename)
{
	byte header[16];
//	auto size = std::filesystem::file_size(filename);
	std::ifstream st(filename, std::ifstream::in | std::ifstream::binary);

	st.read(reinterpret_cast<char*>(header), 16);

	dword constant = Dword(Word(header[0], header[1]), Word(header[2], header[3]));
    if (constant != 0x4E45531A) {
		return nullptr;
	}

	CartridgeConfig config;

	config.prg_size = header[4];
	config.chr_size = header[5];
	config.mirroring = header[6] & 0x1;
	config.persistent = header[6] & 0x2;
	config.trainer = header[6] & 0x4;
	config.four_screen = header[6] & 0x8;
	
	byte ines2 = (header[7] & 0xC) >> 2;
	
	word mapper_id = (header[7] & 0xF0) | ((header[6] & 0xF0) >> 4);


    qDebug() << "prg" << (int)config.prg_size;
    qDebug() << "chr" << (int)config.chr_size;
    qDebug() << "mirror" << (int)config.mirroring;
    qDebug() << "persist" << (int)config.persistent;
    qDebug() << "trainer" << (int)config.trainer;
    qDebug() << "four" << (int)config.four_screen;
    qDebug() << "ines2" << (int)ines2;
    qDebug() << "mapper" << (int)mapper_id;


	switch (mapper_id)
	{
	case 0:  return std::make_unique<Mapper00>(config, st);
    case 1:  return std::make_unique<Mapper01>(config, st);
	default: return nullptr;
	}
}

