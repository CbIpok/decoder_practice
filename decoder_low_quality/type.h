#pragma once
#include <cstdint>

//const (todo remove define)
#define XS_MARKER_PIH 0xff12
#define XS_MARKER_NBYTES 2
#define XS_MARKER_SOC 0xff10
#define XS_MARKER_CAP 0xff50

struct PictureHeader
{
    uint32_t  codestreamSize;
    uint16_t profile;
    uint16_t level;
    uint16_t frameWidth;
    uint16_t frameHeight;
    uint16_t precinctWidth;
    uint16_t  slicehHeight;
    /*precinct_height = (1 << conf->ndecomp_v);
    conf->slice_height *= precinct_height;*/
    uint8_t componentsNumber;
    uint8_t codeGroupSize;
    uint8_t significanceGroupSize;
    uint8_t  waveletBitPrecision;
    uint8_t fractionalBits; //4 bit
    uint8_t rawBitsPerCodeGroup; //4 bit
    uint8_t sliceCodingMode; //1 bit
    uint8_t progressionMode; //3 bit
    uint8_t colourDecorrelation; //4 bit
    uint8_t horizontalWaveletLevels; //4 bit
    uint8_t verticalWaveletLevels; // 4 bit
    uint8_t quantizerType; //4 bit
    uint8_t signHandling; //2 bit
    uint8_t runMode; //2 bit

};