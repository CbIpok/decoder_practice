#include "bitstream.h"
#include <cstdint>
#include <cstring>
#include <cassert>

void readFromBitsream(Bitstream& bitstream, uint8_t *dst, size_t size)
{
    std::memcpy(dst, bitstream.cur, size);
    bitstream.cur += size;
    bitstream.len_readed += size;
}
