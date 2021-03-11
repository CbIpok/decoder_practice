#include "Parser.h"

#include <cassert>

#include "bitstream.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include "bitstream.h"

BlockParser::BlockParser(Bitstream&& bitstream) :
    bitstream(std::move(bitstream))
{

}

BlockParser::BlockParser(uint8_t* bitstream, size_t size)
{
    this->bitstream.buf = bitstream;
    this->bitstream.cur = bitstream;
    this->bitstream.len_readed = 0;
    this->bitstream.size = size;

}


BlockOfMemory BlockParser::getHeader()
{
    uint8_t* buf = new uint8_t[HEADER_SIZE];

    readFromBitsream(bitstream, buf, HEADER_SIZE);
    return { buf, HEADER_SIZE };
}

BlockOfMemory BlockParser::getSlice(PictureHeader& pictureHeader)
{
	uint8_t* data{nullptr};
	size_t size = 0;
	size_t sliceCount = std::ceil((pictureHeader.frameHeight + 0.0) / pictureHeader.slicehHeight);
	size_t memorySize = pictureHeader.codestreamSize / sliceCount * 1.5;
	data = new uint8_t[memorySize];
	Bitstream localBistream = bitstream;

	return BlockOfMemory(data, size);
}

//BlockOfMemory BlockParser::getSlice()
//{
//	
//	return BlockOfMemory();
//}



void writeBlockOfMemoryToFile(const BlockOfMemory& blockOfMemory, const std::string& fileName)
{

    try 
    {
        std::ofstream out(fileName, std::ios::binary);
        //out.write((char*)blockOfMemory.first, blockOfMemory.second);
        out.write((char*)blockOfMemory.data.get(), blockOfMemory.len);
    }
    catch (const std::exception& e)
    {
        std::cout << "cannot write to file";
    }
}


BlockOfMemory::BlockOfMemory(uint8_t* data, size_t size) :
    data(data),
    len(size)
{
    assert(data != nullptr);
    bitstream.buf = data;
    bitstream.cur = data;
    bitstream.len_readed = 0;
    bitstream.size = size;
}


BlockOfMemory::BlockOfMemory(BlockOfMemory&& blockOfMemory) noexcept :
    data(blockOfMemory.data),
    len(blockOfMemory.len)
{
    blockOfMemory.data = nullptr;
}

BlockOfMemory::~BlockOfMemory()
{
     
}



PictureHeader DetailParser::parseHeader(BlockOfMemory& blockOfMemory)
{
	PictureHeader pictureHeader;
	int LenghtPih = 26; 
	int precinctHeight; 
	uint16_t marker;
	uint32_t val32 = 0;
	uint16_t val16 = 0;
	uint8_t val8 = 0;

	
	//SOC marker 
	{
		readFromBitsreamAndSwap(blockOfMemory.bitstream, marker, XS_MARKER_NBYTES);
		assert(marker == XS_MARKER_SOC);
	}
	//CAP markers
	{
		readFromBitsreamAndSwap(blockOfMemory.bitstream, marker, XS_MARKER_NBYTES);
		assert(marker == XS_MARKER_CAP);
		//Lcap
		readFromBitsreamAndSwap(blockOfMemory.bitstream, marker, XS_MARKER_NBYTES);
		assert(marker == 2); 
	}
	//PIH markers and data
	{
		readFromBitsreamAndSwap(blockOfMemory.bitstream, marker, XS_MARKER_NBYTES);
		assert(marker == XS_MARKER_PIH);
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val16, sizeof(val16));
		assert(val16 == LenghtPih);
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val32, sizeof(val32));
		pictureHeader.codestreamSize = val32;

		readFromBitsreamAndSwap(blockOfMemory.bitstream, val16, sizeof(val16));
		pictureHeader.profile = val16;

		readFromBitsreamAndSwap(blockOfMemory.bitstream, val16, sizeof(val16));
		pictureHeader.level = val16;

		readFromBitsreamAndSwap(blockOfMemory.bitstream, val16, sizeof(val16));
		pictureHeader.frameHeight = val16;

		readFromBitsreamAndSwap(blockOfMemory.bitstream, val16, sizeof(val16));
		pictureHeader.frameWidth = val16;


		readFromBitsreamAndSwap(blockOfMemory.bitstream, val16, sizeof(val16));
		pictureHeader.precinctWidth = val16;

		readFromBitsreamAndSwap(blockOfMemory.bitstream, val16, sizeof(val16));
		pictureHeader.slicehHeight = val16;
		 
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));
		pictureHeader.componentsNumber = val8;
		 
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));
		pictureHeader.codeGroupSize = val8;
		 
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));
		pictureHeader.significanceGroupSize = val8;
	
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));
		pictureHeader.waveletBitPrecision = val8;
		 
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));

		pictureHeader.fractionalBits =  val8 >> 4; //read 4

		pictureHeader.rawBitsPerCodeGroup = (val8 & 0b0001111); //read 4
		assert(pictureHeader.rawBitsPerCodeGroup == 4);

		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));

		pictureHeader.sliceCodingMode = val8 >> 7; //read 1
	
		pictureHeader.progressionMode = (val8 >> 4) & 0b00000111; //read 3

		pictureHeader.colourDecorrelation = val8 & 0b0001111; //read 4
		
		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));
		pictureHeader.horizontalWaveletLevels = val8 >> 4; //read 4

		pictureHeader.verticalWaveletLevels = val8 & 0b0001111; //read 4
		precinctHeight = (1 << pictureHeader.verticalWaveletLevels);
		pictureHeader.slicehHeight *= precinctHeight;

		readFromBitsreamAndSwap(blockOfMemory.bitstream, val8, sizeof(val8));

		pictureHeader.quantizerType = val8 >> 4;//read 4

		pictureHeader.signHandling = (val8 >> 2) & (0b00000011); //read 2

		pictureHeader.runMode = val8 & 0b000000011; //read 2



	}
	//parse CDT
	{
		
	}
	//parse WGT
	{

	}
	


	
	return pictureHeader;
}
