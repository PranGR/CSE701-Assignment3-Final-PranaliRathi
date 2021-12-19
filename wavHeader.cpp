/**
 * @file wavHeader.cpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Implementation of the wavHeader class
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <fstream>
#include <iostream>
#include "wavHeader.hpp"

using namespace std;

wavHeader::wavHeader()
{
    // -- Default constructor
}

wavHeader::wavHeader(FILE *fp)
{
    // -- Read the components of the wav file header
    fseek(fp, 0, SEEK_SET);

    // -- RIFF Chunk Descriptor
    if (fread(chunkID, sizeof(chunkID[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read chunkID in file header";
        exit(1);
    }
    if (!(chunkID[0] == 'R' && chunkID[1] == 'I' && chunkID[2] == 'F' && chunkID[3] == 'F'))
    {
        fclose(fp);
        cout << "Error in file header, incorrect chunkID";
        exit(1);
    }
    if (fread(&chunkSize, sizeof(chunkSize), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read chunkSize in file header";
        exit(1);
    }
    if (fread(format, sizeof(format[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read format in file header";
        exit(1);
    }
    if (!(format[0] == 'W' && format[1] == 'A' && format[2] == 'V' && format[3] == 'E'))
    {
        fclose(fp);
        cout << "Error in file header, incorrect format";
        exit(1);
    }

    // -- Fmt sub-chunk
    if (fread(subchunk1ID, sizeof(subchunk1ID[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk1ID in file header";
        exit(1);
    }
    if (!(subchunk1ID[0] == 'f' && subchunk1ID[1] == 'm' && subchunk1ID[2] == 't' && subchunk1ID[3] == ' '))
    {
        fclose(fp);
        cout << "Error in file header, incorrect subchunk1ID";
        exit(1);
    }
    if (fread(&subchunk1Size, sizeof(subchunk1Size), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk1Size in file header";
        exit(1);
    }
    if (fread(&audioFormat, sizeof(audioFormat), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read audioFormat in file header";
        exit(1);
    }
    if (fread(&numChannels, sizeof(numChannels), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read numChannels in file header";
        exit(1);
    }
    if (fread(&sampleRate, sizeof(sampleRate), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read sampleRate in file header";
        exit(1);
    }
    if (fread(&byteRate, sizeof(byteRate), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read byteRate in file header";
        exit(1);
    }
    if (fread(&blockAlign, sizeof(blockAlign), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read blockAlign in file header";
        exit(1);
    }
    if (fread(&bitsPerSample, sizeof(bitsPerSample), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read bitsPerSample in file header";
        exit(1);
    }
    if (bitsPerSample != 16)
    {
        fclose(fp);
        cout << "Error in file header, bitsPerSample is not 16. Audio file must be 16 bits per sample";
        exit(1);
    }

    // -- Data sub-chunk
    if (fread(subchunk2Id, sizeof(subchunk2Id[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk2Id in file header";
        exit(1);
    }
    if (!(subchunk2Id[0] == 'd' && subchunk2Id[1] == 'a' && subchunk2Id[2] == 't' && subchunk2Id[3] == 'a'))
    {
        fclose(fp);
        cout << "Error in file header, incorrect subchunk2Id";
        exit(1);
    }
    if (fread(&subchunk2Size, sizeof(subchunk2Size), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk2Size in file header";
        exit(1);
    }
}

wavHeader::wavHeader(const wavHeader &obj)
{
    // -- Copy constructor
    chunkID[0] = obj.chunkID[0];
    chunkID[1] = obj.chunkID[1];
    chunkID[2] = obj.chunkID[2];
    chunkID[3] = obj.chunkID[3];
    chunkSize = obj.chunkSize;
    format[0] = obj.format[0];
    format[1] = obj.format[1];
    format[2] = obj.format[2];
    format[3] = obj.format[3];
    subchunk1ID[0] = obj.subchunk1ID[0];
    subchunk1ID[1] = obj.subchunk1ID[1];
    subchunk1ID[2] = obj.subchunk1ID[2];
    subchunk1ID[3] = obj.subchunk1ID[3];
    subchunk1Size = obj.subchunk1Size;
    audioFormat = obj.audioFormat;
    numChannels = obj.numChannels;
    sampleRate = obj.sampleRate;
    byteRate = obj.byteRate;
    blockAlign = obj.blockAlign;
    bitsPerSample = obj.bitsPerSample;
    subchunk2Id[0] = obj.subchunk2Id[0];
    subchunk2Id[1] = obj.subchunk2Id[1];
    subchunk2Id[2] = obj.subchunk2Id[2];
    subchunk2Id[3] = obj.subchunk2Id[3];
    subchunk2Size = obj.subchunk2Size;
}

uint16_t wavHeader::getbytesPerSample()
{
    // -- Calculate bytes per sample
    uint16_t bytesPerSample;
    bytesPerSample = (uint16_t)bitsPerSample / 8;
    return bytesPerSample;
}

uint64_t wavHeader::getNumberOfSamples()
{
    // -- Calculate number of samples
    uint64_t numberOfSamples;
    numberOfSamples = (uint64_t)chunkSize / (bitsPerSample / 8);
    return numberOfSamples;
}

uint32_t wavHeader::getSamplesPerSecond()
{
    // -- Find the samples per second
    uint32_t samplesPerSecond;
    samplesPerSecond = (uint32_t)sampleRate;
    return samplesPerSecond;
}

void wavHeader::writeHeader(FILE *fp)
{
    // -- Write the components of the wav file header
    fseek(fp, 0, SEEK_SET);

    // -- RIFF Chunk Descriptor
    if (fwrite(chunkID, sizeof(chunkID[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read chunkID in file header";
        exit(1);
    }
    if (fwrite(&chunkSize, sizeof(chunkSize), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read chunkSize in file header";
        exit(1);
    }
    if (fwrite(format, sizeof(format[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read format in file header";
        exit(1);
    }

    // -- Fmt sub-chunk
    if (fwrite(subchunk1ID, sizeof(subchunk1ID[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk1ID in file header";
        exit(1);
    }
    if (fwrite(&subchunk1Size, sizeof(subchunk1Size), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk1Size in file header";
        exit(1);
    }
    if (fwrite(&audioFormat, sizeof(audioFormat), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read audioFormat in file header";
        exit(1);
    }
    if (fwrite(&numChannels, sizeof(numChannels), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read numChannels in file header";
        exit(1);
    }
    if (fwrite(&sampleRate, sizeof(sampleRate), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read sampleRate in file header";
        exit(1);
    }
    if (fwrite(&byteRate, sizeof(byteRate), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read byteRate in file header";
        exit(1);
    }
    if (fwrite(&blockAlign, sizeof(blockAlign), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read blockAlign in file header";
        exit(1);
    }
    if (fwrite(&bitsPerSample, sizeof(bitsPerSample), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read bitsPerSample in file header";
        exit(1);
    }

    // -- Data sub-chunk
    if (fwrite(subchunk2Id, sizeof(subchunk2Id[0]) * 4, 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk2Id in file header";
        exit(1);
    }
    if (fwrite(&subchunk2Size, sizeof(subchunk2Size), 1, fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not read subchunk2Size in file header";
        exit(1);
    }
}
