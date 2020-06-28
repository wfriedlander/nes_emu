#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "json.h"
#include "common.h"

class Serializer
{
    enum class Type {
        REG,
        BYTE,
        WORD,
        BOOL,
        BYTE_ARRAY,
        SERIALIZER
    };

    struct Item {
        Type type;
        std::string name;
        void* pointer = nullptr;
        int length = 0;
    };

public:
    json Serialize();
    void Deserialize(json state);

public:
    void RegisterField(std::string name, reg* value);
    void RegisterField(std::string name, byte* value);
    void RegisterField(std::string name, word* value);
    void RegisterField(std::string name, bool* value);
    void RegisterField(std::string name, byte* value, int length);
    void RegisterField(std::string name, Serializer* value);

private:
    std::vector<Item> mItems;
};

#endif // SERIALIZER_H
