/**
 * @file wavFile.cpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Implementation of the wavFile class
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <vector>
#include <cstddef>
#include <fstream>
#include <iostream>
#include "wavFile.hpp"

using namespace std;

wavFile::wavFile()
{
    // -- Default constructor
}

wavFile::wavFile(FILE *fp) : header(fp), filter()
{
    // -- Initialize some properties of the wave file
    bytesPerSample = header.getbytesPerSample();
    numberOfSamples = header.getNumberOfSamples();
    samplesPerSecond = header.getSamplesPerSecond();

    audioData.resize(numberOfSamples);
    outputData.resize(numberOfSamples);

    // -- Read raw audio data into the data buffer
    uint64_t sampleCount;
    int16_t sample;
    sampleCount = 0;
    sample = 0;

    do
    {
        // -- Reading 16 bit audio data
        if (fread(&sample, sizeof(int16_t), 1, fp) != 1)
        {
            // -- Nothing left to read
            break;
        }

        sampleCount++;
        audioData[sampleCount] = sample;

    } while (sampleCount < numberOfSamples);

    // -- Check if audio data was able to be read
    if (sampleCount == 0)
    {
        fclose(fp);
        cout << "Error! could not read raw audio data from input file";
        exit(1);
    }

    // -- Initialize output data with the input audio data
    copy(audioData.begin(), audioData.end(), outputData.begin());
}

wavFile::wavFile(const wavFile &obj) : header(obj.header), filter(obj.filter)
{
    // -- Copy constructor
    audioData = obj.audioData;
    outputData = obj.outputData;
    bytesPerSample = obj.bytesPerSample;
    numberOfSamples = obj.numberOfSamples;
    samplesPerSecond = obj.samplesPerSecond;
}

void wavFile::processFirFilter(vector<double> filterCoeffs, uint16_t filterCoeffsLen)
{
    uint64_t batches, remaining;
    vector<int16_t> batchData;

    // -- Split up the total number of samples into processable chunks
    batches = numberOfSamples / samplesPerSecond;
    remaining = numberOfSamples % samplesPerSecond;
    batchData.resize(samplesPerSecond);
    fill(batchData.begin(), batchData.end(), 0);
    fill(outputData.begin(), outputData.end(), 0);

    // -- For each chunk apply the filter and repeat until all sample data has been processed

    for (uint64_t i = 0; i < batches; i++)
    {
        // -- Calculate the vector offsets for where to start reading and writting the batch data
        uint64_t audioDataStartOffset, audioDataEndOffset, outputDataOffset;
        audioDataStartOffset = (i * samplesPerSecond);
        audioDataEndOffset = ((i + 1) * samplesPerSecond - 1);
        outputDataOffset = (i * samplesPerSecond);

        copy(audioData.begin() + audioDataStartOffset, audioData.begin() + audioDataEndOffset, batchData.begin());
        batchData = filter.applyFirFilter(batchData, filterCoeffs, filterCoeffsLen, samplesPerSecond);
        copy(batchData.begin(), batchData.end(), outputData.begin() + outputDataOffset);
    }

    // -- Filter the last remaining samples that were not divisible by the batch size
    fill(batchData.begin(), batchData.end(), 0);
    copy(audioData.end() - remaining, audioData.end(), batchData.begin());
    batchData = filter.applyFirFilter(batchData, filterCoeffs, filterCoeffsLen, samplesPerSecond);
    copy(batchData.begin(), batchData.begin() + remaining, outputData.end() - remaining);
}

void wavFile::writeWavFile(FILE *fp)
{
    // -- Write the file header
    header.writeHeader(fp);

    // -- Write the data buffer to the output file
    if (fwrite(&outputData[0], bytesPerSample, outputData.size(), fp) == 0)
    {
        fclose(fp);
        cout << "Error! could not write audio data into output file";
        exit(1);
    }
}
