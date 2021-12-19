/**
 * @file firFilter.hpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Class definition to create a fir filter 
 * 
 * This class initializes the fir filter and processes data given different filter coefficients
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include "firFilter.hpp"

using namespace std;

class firFilter
{
public:
    /**
     * @brief Default constructor to create a new fir Filter object
     * 
     */
    firFilter();

    /**
     * @brief Copy constructor
     * 
     * @param obj The source fir filter to be copied over
     */
    firFilter(const firFilter &obj);

    /**
     * @brief Process the input data with the chosen filter coefficients
     * 
     * Applies the fir filter to all input samples. The fir filter is configured to use the specified filter
     * coefficients. The filter buffer used for processing is dependant on the number of input samples and 
     * samples per second of the input audio file. Once each sample is processed, its stored in an output vector
     * 
     * @param inputSamples Vector of input samples to be processed
     * @param filterCoeffs Specified coefficients used to configure the fir filter
     * @param filterCoeffsLen Number of coefficients
     * @param samplesPerSecond Samples per second of the input audio file 
     * @return Vector of the processed output samples.
     */
    vector<int16_t> applyFirFilter(vector<int16_t> inputSamples, vector<double> filterCoeffs, uint16_t filterCoeffsLen, uint32_t samplesPerSecond);

private:
    /**
     * @brief Buffer used for filtering
     * 
     */
    vector<double> filterBuffer;

    /**
     * @brief Number of samples filtered per batch
     * 
     */
    uint64_t sampleBuffLen;
};
