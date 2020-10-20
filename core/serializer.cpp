#include "serializer.h"


const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


std::string base64_encode(byte const* bytes, size_t length)
{
    std::string ret;
    ret.reserve((length + 2) / 3 * 4);

    unsigned int pos = 0;

    while (pos < length)
    {
        ret += base64_chars[(bytes[pos + 0] & 0xfc) >> 2];
        if (pos+1 < length)
        {
            ret += base64_chars[((bytes[pos + 0] & 0x03) << 4) + ((bytes[pos + 1] & 0xf0) >> 4)];
            if (pos+2 < length)
            {
                ret += base64_chars[((bytes[pos + 1] & 0x0f) << 2) + ((bytes[pos + 2] & 0xc0) >> 6)];
                ret += base64_chars[  bytes[pos + 2] & 0x3f];
            }
            else
            {
                ret += base64_chars[(bytes[pos + 1] & 0x0f) << 2];
                ret += '=';
            }
        }
        else
        {
            ret += base64_chars[(bytes[pos + 0] & 0x03) << 4];
            ret += '=';
            ret += '=';
        }

        pos += 3;
    }

    return ret;
}

void base64_decode(std::string string, byte* bytes)
{
    size_t string_length = string.length();
    size_t pos = 0;
    size_t byte_pos = 0;

    while (pos < string_length)
    {
        unsigned int pos_of_char_1 = base64_chars.find(string[pos+1]);
        bytes[byte_pos++] = (byte)((base64_chars.find(string[pos+0])) << 2) + ((pos_of_char_1 & 0x30) >> 4);
        if (string[pos+2] != '=')
        {
            unsigned int pos_of_char_2 = base64_chars.find(string[pos+2]);
            bytes[byte_pos++] = ((pos_of_char_1 & 0x0f) << 4) + ((pos_of_char_2 & 0x3c) >> 2);
            if (string[pos+3] != '=')
            {
                bytes[byte_pos++] = (byte)(((pos_of_char_2 & 0x03) << 6) + base64_chars.find(string[pos+3]));
            }
        }

       pos += 4;
    }
}


json Serializable::Serialize()
{
    json state;
    for (auto& item : mItems)
    {
        switch (item.type) {
        case Type::REG:
            state[item.name] = (byte)*static_cast<reg*>(item.pointer);
            break;
        case Type::BYTE:
            state[item.name] = *static_cast<byte*>(item.pointer);
            break;
        case Type::WORD:
            state[item.name] = *static_cast<word*>(item.pointer);
            break;
        case Type::DWORD:
            state[item.name] = *static_cast<dword*>(item.pointer);
            break;
        case Type::TIME:
            state[item.name] = *static_cast<nes_time*>(item.pointer);
            break;
        case Type::BOOL:
            state[item.name] = *static_cast<bool*>(item.pointer);
            break;
        case Type::BYTE_ARRAY:
            state[item.name] = base64_encode(static_cast<byte*>(item.pointer), item.length);
            break;
        case Type::SERIALIZER:
            state[item.name] = static_cast<Serializable*>(item.pointer)->Serialize();
            break;
        }
    }
    return state;
}

void Serializable::Deserialize(json state)
{
    for (auto& item : mItems)
    {
        if (state.contains(item.name))
        {
            switch (item.type) {
            case Type::REG:
                *static_cast<reg*>(item.pointer) = state[item.name];
                break;
            case Type::BYTE:
                *static_cast<byte*>(item.pointer) = state[item.name];
                break;
            case Type::WORD:
                *static_cast<word*>(item.pointer) = state[item.name];
                break;
            case Type::DWORD:
                *static_cast<dword*>(item.pointer) = state[item.name];
                break;
            case Type::TIME:
                *static_cast<nes_time*>(item.pointer) = state[item.name];
                break;
            case Type::BOOL:
                *static_cast<bool*>(item.pointer) = state[item.name];
                break;
            case Type::BYTE_ARRAY:
                base64_decode(state[item.name], static_cast<byte*>(item.pointer));
                break;
            case Type::SERIALIZER:
                static_cast<Serializable*>(item.pointer)->Deserialize(state[item.name]);
                break;
            }
        }
        else
        {
            // MAYBE WARN SOMETHING WENT WRONG HERE?
        }
    }
}

void Serializable::RegisterField(std::string name, reg *value)
{
    mItems.push_back(Item {Type::REG, name, value});
}

void Serializable::RegisterField(std::string name, byte *value)
{
    mItems.push_back(Item {Type::BYTE, name, value});
}

void Serializable::RegisterField(std::string name, word *value)
{
    mItems.push_back(Item {Type::WORD, name, value});
}

void Serializable::RegisterField(std::string name, dword *value)
{
    mItems.push_back(Item {Type::DWORD, name, value});
}

void Serializable::RegisterField(std::string name, nes_time *value)
{
    mItems.push_back(Item {Type::TIME, name, value});
}

void Serializable::RegisterField(std::string name, bool *value)
{
    mItems.push_back(Item {Type::BOOL, name, value});
}

void Serializable::RegisterField(std::string name, byte *value, int length)
{
    mItems.push_back(Item {Type::BYTE_ARRAY, name, value, length});
}

void Serializable::RegisterField(std::string name, Serializable *value)
{
    mItems.push_back(Item {Type::SERIALIZER, name, value});
}
