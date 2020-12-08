
#include "mapper000.h"


Mapper000::Mapper000(Cartridge& cart) : Mapper(cart)
{
    MapPrg(2, 2, &prg[0]);
    if (cart.prg_size > 16) {
        MapPrg(4, 2, &prg[0x2000]);
    }
    else {
        MapPrg(4, 2, &prg[0]);
    }
    MapChr(0, 8, &chr[0]);
    if (mMirroring == 0) {
        MapName(0, 1, &ram[0]);
        MapName(1, 1, &ram[0]);
        MapName(2, 1, &ram[1024]);
        MapName(3, 1, &ram[1024]);
    } else {
        MapName(0, 2, &ram[0]);
        MapName(2, 2, &ram[0]);
    }
}