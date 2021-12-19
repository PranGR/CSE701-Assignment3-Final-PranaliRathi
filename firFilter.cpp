/**
 * @file firFilter.cpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Implementation of the fir filter class
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <cmath>
#include <iostream>
#include "firFilter.hpp"

using namespace std;

firFilter::firFilter()
{
    // -- Default constructor
}

firFilter::firFilter(const firFilter &obj)
{
    // -- Copy constructor
    filterBuffer = obj.filterBuffer;
    sampleBuffLen = obj.sampleBuffLen;
}

vector<int16_t> firFilter::applyFirFilter(vector<int16_t> inputSamples, vector<double> filterCoeffs, uint16_t filterCoeffsLen, uint32_t samplesPerSecond)
{
    uint64_t k;
    double accumulatedValue;       // -- Accumulated value
    vector<int16_t> outputSamples; // -- Output vector
    outputSamples.resize(inputSamples.size());

    // -- The number of samples to be processed at one time have been chosen to be the number of samples in one second
    filterBuffer.resize(filterCoeffsLen - 1 + samplesPerSecond);
    // -- Initialize inputSamples buffer
    fill(filterBuffer.begin(), filterBuffer.end(), 0);

    // -- Put the inputSamples at the end of the buffer
    for (uint64_t i = 0; i < inputSamples.size(); i++)
    {
        filterBuffer[(filterCoeffsLen - 1) + i] = inputSamples[i];
    }

    // -- Apply the chosen filter coefficients to each sample and accumulate the value for
    // -- each output sample
    for (uint64_t i = 0; i < inputSamples.size(); i++)
    {
        k = filterCoeffsLen - 1 + i;
        accumulatedValue = 0;

        for (int j = 0; j < filterCoeffsLen; j++)
        {
            accumulatedValue += (filterCoeffs[j]) * (filterBuffer[k - j]);
        }
        outputSamples[i] = (int16_t)round(accumulatedValue);
    }

    // -- Move the last of the current sample batch to beginning of the filter buffer for
    // -- the next batch of samples
    for (uint64_t i = 0; i < filterCoeffsLen; i++)
    {
        filterBuffer[i] = filterBuffer[inputSamples.size() + i];
    }

    return outputSamples;
}
