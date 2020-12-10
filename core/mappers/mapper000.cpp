
#include "mapper000.h"

#include <iostream>

namespace Core {


Mapper000::Mapper000(Cartridge& cart) : Mapper(cart)
{
    MapRegion(PRG_0, 32, prg);
    MapRegion(CHR_0, 8, chr);
    MapNametable(mMirroring ? VERTICAL : HORIZONTAL);
}


}
