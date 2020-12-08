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


template <typename T>
struct Result
{
    T value;
    bool valid = false;
    std::string error;

    Result(const char* e) : error(e) {}
    Result(std::string e) : error(e) {}
    Result(T t) : value(t), valid(true) {}

    explicit operator bool() {return valid;}
    operator T&() {return value;}
    T& operator*() {return value;}
    T* operator->() {return &value;}
};

template <typename T>
struct Result<T*>
{
    std::unique_ptr<T> value = nullptr;
    bool valid = false;
    std::string error;

    Result(const char* e) : error(e) {}
    Result(std::string e) : error(e) {}
    Result(T* t) : value(t), valid(true) {}

    explicit operator bool() {return valid;}
    T& operator*() {return *value;}
    T* operator->() {return value.get();}
    operator T*() {T* t = value.get(); value.release(); return t;}
    operator std::unique_ptr<T>() {return std::move(value);}
};

template <>
struct Result<std::string>
{
    std::string value;
    bool valid = false;
    std::string error;

    Result(const char* s, bool v=true) : valid(v) {if (v) value = s; else error = s;}
    Result(std::string s, bool v=true) : valid(v) {if (v) value = s; else error = s;}

    explicit operator bool() {return valid;}
    operator std::string() {return value;}
    operator std::string&() {return value;}
    std::string& operator*() {return value;}
    std::string* operator->() {return &value;}
};

