#pragma once

#include <string>
#include <vector>
#include <memory>
#include "bitstream.h"
#include "type.h"

class BlockOfMemory final// struct with destructure
{
public:
    BlockOfMemory(uint8_t *data,size_t size);

    BlockOfMemory(const BlockOfMemory&) = delete;

    BlockOfMemory(BlockOfMemory&& blockOfMemory) noexcept;

    ~BlockOfMemory() = default;

    std::unique_ptr<uint8_t[]> data;
    std::size_t len;
    Bitstream bitstream;
};

class BlockParser final
{
public:

    BlockParser(Bitstream&& bitstream);

    BlockParser(uint8_t *bitstream, size_t size);

    BlockOfMemory getHeader(); //todo SLICE HEADER LEN IS NOT CONSTANT!!
    
    std::vector<BlockOfMemory> getSlices();

    BlockOfMemory getSlice();

    BlockOfMemory gesEos(); //to simply 

private:
    constexpr static size_t HEADER_SIZE = 96;

    Bitstream bitstream;

    std::unique_ptr<uint8_t> buf;

    enum class ParseState: uint8_t
    {
        HEADER,
        SLISES,
        EOS,
        DONE
    };

    size_t getSliceSize(Bitstream& lbitsream);
    

   
};

class DetailParser final
{
public:
    DetailParser() = default;
    ~DetailParser() = default;

    PictureHeader parseHeader(BlockOfMemory& blockOfMemory);

private:

};

//methods
void writeBlockOfMemoryToFile(const BlockOfMemory& blockOfMemory, const std::string& fileName);