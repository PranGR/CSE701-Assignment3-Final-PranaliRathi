/**
 * @file wavHeader.hpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Class definition to read and write the header of audio wav files.
 * 
 * This class defines the data structure used to store the information found in a wav file header.
 * This class contains functions to read a wav file header, write information to create a new wav file
 * header and retrieve certain properties of the wav file header
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <fstream>
#include <cstdint>

class wavHeader
{
public:
    /**
     * @brief Default constructor to create a new wav Header object
     * 
     */
    wavHeader();
    /**
     * @brief Construct a new wav Header object given an input wave file 
     * 
     * @param fp A pointer to the input wav audio file
     */
    wavHeader(FILE *fp);

    /**
     * @brief Copy constructor
     * 
     * @param obj The source wave header to be copied over
     */
    wavHeader(const wavHeader &obj);

    /**
     * @brief Gets the number of bytes per sample for the audio file
     * 
     * @return Returns bytesPerSample
     */
    uint16_t getbytesPerSample();

    /**
     * @brief Gets the number of samples for the audio file
     * 
     * @return Returns numberOfSamples
     */
    uint64_t getNumberOfSamples();

    /**
     * @brief Gets the sample rate for the audio file
     * 
     * @return Returns samplesPerSecond
     */
    uint32_t getSamplesPerSecond();

    /**
     * @brief Write the header data to the output wav file
     * 
     * @param fp A pointer to the output wav audio file
     */
    void writeHeader(FILE *fp);

private:
    /**
     * @brief RIFF Chunk Descriptor
     * 
     */
    unsigned char chunkID[4];
    uint32_t chunkSize;
    unsigned char format[4];

    /**
     * @brief Fmt sub-chunk
     * 
     */
    unsigned char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;

    /**
     * @brief Data sub-chunk
     * 
     */
    unsigned char subchunk2Id[4];
    uint32_t subchunk2Size;
};
