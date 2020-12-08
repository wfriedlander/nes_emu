#pragma once

#include <cstdint>
#include <tuple>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <chrono>


using sbyte = int8_t;
using sword = int16_t;
using sdword = int32_t;
using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;
using nes_time = uint64_t;


inline dword Dword(word h, word l)
{
	return (h << 16) | l;
}

inline word Word(byte h, byte l)
{
	return (h << 8) | l;
}

inline byte Byte(word w)
{
	return w & 0xFF;
}

inline byte ByteLow(word w)
{
    return w & 0xFF;
}

inline byte ByteHigh(word w)
{
    return w >> 8;
}

inline std::tuple<byte, byte> Bytes(word w)
{
	return std::make_tuple<byte, byte>((w & 0xFF00) >> 8, w & 0xFF);
}

struct reg {
    virtual operator byte() const = 0;
    virtual void operator =(byte val) = 0;
};
