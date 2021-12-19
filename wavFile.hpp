/**
 * @file wavFile.hpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Class definition to read, write and begin the processing of the audio wave files
 * 
 * This class defines the data structure used to store the information found in a wav file.
 * This class contains functions to read a wav file, write information to create a new wav file
 * and calls the fir filter to process the wav file audio data.
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <vector>
#include <cstddef>
#include "wavHeader.hpp"
#include "firFilter.hpp"

using namespace std;

class wavFile
{
public:
    /**
     * @brief Input audio data
     * 
     */
    vector<int16_t> audioData;

    /**
     * @brief Output processed audio data
     * 
     */
    vector<int16_t> outputData;

    /**
     * @brief Default constructor to create a new wav file object
     * 
     */
    wavFile();

    /**
     * @brief Construct a new wav File object given an input wave file
     * 
     * @param fp a pointer to the input wav audio file
     */
    wavFile(FILE *fp);

    /**
     * @brief Copy constructor
     * 
     * @param obj The source wave file to be copied over
     */
    wavFile(const wavFile &obj);

    /**
     * @brief Write the wav file info the output file
     * 
     * Takes the stored input wave file and copies its header information along
     * with the output processed data to the new file
     * 
     * @param fp a pointer to the output wav audio file
     */
    void writeWavFile(FILE *fp);

    /**
     * @brief Process the input audio data with the specified filter coefficients
     * 
     * Calls the fir filter to process the input audio data with the specified filter coefficients.
     * As the fir filter can only process a limited number of samples at a time (specified by
     * samplesPerSecond), the input audio data has to be batched into appropriate chunks. The fir filter
     * is called for each chunk and the resulting output data is stored in outputData vector
     * 
     * @param filterCoeffs Set of filter coefficients
     * @param filterCoeffsLen number of filter coefficients
     */
    void processFirFilter(vector<double> filterCoeffs, uint16_t filterCoeffsLen);

private:
    /**
     * @brief Header component of the wav file
     * 
     */
    wavHeader header;

    /**
     * @brief Instance of the fir filter for this audio file
     * 
     */
    firFilter filter;

    /**
     * @brief Number of bytes per sample for the wav file
     * 
     */
    uint16_t bytesPerSample;

    /**
     * @brief Number of samples for the wav file
     * 
     */
    uint64_t numberOfSamples;

    /**
     * @brief Sample rate for the wav file
     * 
     */
    uint32_t samplesPerSecond;
};
