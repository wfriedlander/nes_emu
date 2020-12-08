#pragma once

#include "common.h"
#include "mappers/mapper.h"

#include <map>




struct iNesRom
{
    uint8_t     header[4];
    uint8_t     prg_size;
    uint8_t     chr_size;
    uint8_t     mirroring:1;
    uint8_t     battery:1;
    uint8_t     trainer_size:1;
    uint8_t     four_screen:1;
    uint8_t     mapper_low:4;
    uint8_t     vs:1;
    uint8_t     playchoice:1;
    uint8_t     ines:2;
    uint8_t     mapper_high:4;
    uint8_t     reserved[8]; // put ines2.0 stuff here

    uint16_t    mapper;

    std::vector<byte> trainer;
    std::vector<byte> prg;
    std::vector<byte> chr;
};


struct DBRecord
{
    std::string system;
    std::string board;
    std::string pcb;
    std::string chip;
    int mapper = 0;
    int prg = 0;
    int chr = 0;
    int vram = 0;
    int wram = 0;
    int sram = 0;
    bool battery = false;
    std::string mirroring;
    std::string controller;
    std::string conflicts;
    std::string submapper;
    std::string vs;
    std::string ppu;
};


class RomLoader
{
public:
    RomLoader();

public:
    Result<Mapper*> Load(std::string filename);

private:
    std::map<uint32_t, DBRecord> mRoms;
};
