/**
 * @file audioFilter.cpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief This file contains the entry point of this audio filtering program
 * 
 * This audio filtering program is a simple FIR filter that allows the user to filter specified frequencies from an input audio wave file.
 * This program comes equipped with 4 different sets of filter coefficients that the user can select from, along with the ability to specify
 * custom filters by providing a coefficients file. The user can also run the input wav file through multiple iterations of processing for further
 * customization of the frequency filtering.
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdint>
#include "wavFile.hpp"
#include "wavHeader.hpp"
#include "firFilter.hpp"
#include "coeffFileParser.hpp"
#include "argumentValidator.hpp"
#include "defaultFilterCoeffs.hpp"

using namespace std;

/**
 * @brief Defines the length of the coefficient sets for the default filters
 * 
 */
constexpr uint16_t Default_Filter_Coeffs_Len = 51;

/**
 * @brief Entry point of this program
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments: 
 *        argv[1] Specifies the name of the input wav file
 *        argv[2] Specifies the name of the output wav file
 *        argv[3] Specifies if default filters are to used or custom coefficients are to be used. Options include: y (default filter), n (custom coefficients)
 *        argv[4] Specifies the number of filters to be applied. If default filters are used, only have a maximum of 4 filters can be applied. If this value
 *                is 0, then the input wav file is simply copied to the output file without modification.
 *        argv[5] If using default filters, specifies the first type of filter to be used. The options include: lp, hp, bp, bs.
 *                If using custom coefficients, specifies the name of the text file with the coefficient values. Each set of coefficients should be enclosed 
 *                in square brackets and individual coefficients should be seperated by commas. Each set of coefficients should be seperated by commas and the
 *                number of sets of coefficients should equal the value of argv[4]
 *        argv[6-8] Optional arguments if using default filters. Specifies type of filters to be used. The options include: lp, hp, bp, bs 
 * @return Program exit code
 */
int main(int argc, char *argv[])
{
    string inputFile, outputFile, defaultFilter;
    int16_t filterCount;
    argumentValidator validator;

    // -- Check if all required arguments are present
    validator.requiredArgsPresent((uint16_t)argc);

    inputFile = argv[1];
    outputFile = argv[2];
    defaultFilter = argv[3];

    // -- Make sure filter count is a valid integer
    filterCount = validator.validFilterCount(argv[4]);

    // -- Open the input file
    FILE *fp = fopen(argv[1], "rb"); // -- read in binary mode
    if (fp == NULL)
    {
        cout << "Error! could not open file "
             << inputFile
             << ": please make sure that this is the correct filename\n ";
        exit(1);
    }

    // -- Create an instance of the wavFile class using the input wave file
    wavFile wav(fp);
    fclose(fp);

    // -- Only if the filter count is greater than 0, then complete further processing
    if (filterCount > 0)
    {
        if (defaultFilter == "y" || defaultFilter == "Y")
        {
            // -- Default filters chosen

            // -- Make sure the filter count agrees with the number of arguments
            validator.validFilterCountAndNumOfArgs(filterCount, (uint16_t)argc);

            // -- For each default filter chosen, process the input audio of the wavefile
            for (int16_t i = 5; i < argc; i++)
            {
                string filterType;
                filterType = argv[i];

                // -- Depending on the filter specified, perform that processing
                if (filterType == "lp" || filterType == "LP" || filterType == "Lp" || filterType == "lP")
                {
                    // -- Process the wav file with a Lowpass filter
                    wav.processFirFilter(lowPassCoeffs, Default_Filter_Coeffs_Len);
                }
                else if (filterType == "hp" || filterType == "HP" || filterType == "Hp" || filterType == "hP")
                {
                    // -- Process the wav file with a Highpass filter
                    wav.processFirFilter(highPassCoeffs, Default_Filter_Coeffs_Len);
                }
                else if (filterType == "bp" || filterType == "BP" || filterType == "Bp" || filterType == "bP")
                {
                    // -- Process the wav file with a Bandpass filter
                    wav.processFirFilter(speechRangeCoeffs, Default_Filter_Coeffs_Len);
                }
                else if (filterType == "bs" || filterType == "BS" || filterType == "Bs" || filterType == "bS")
                {
                    // -- Process the wav file with a Bandstop filter
                    wav.processFirFilter(noSpeechRangeCoeffs, Default_Filter_Coeffs_Len);
                }
                else
                {
                    cout << "Error! invalid filter type chosen for argument "
                         << i + 1
                         << ". Make sure filter_type is only one of the following lp, hp, bp, bs";
                    exit(1);
                }

                // -- For the next round of processing, the current output data should become the next input audio data
                copy(wav.outputData.begin(), wav.outputData.end(), wav.audioData.begin());
            }
        }
        else if (defaultFilter == "n" || defaultFilter == "N")
        {
            // -- Custom filters chosen
            string coefficientFile;
            coeffFileParser fileParser;
            vector<vector<double>> coefficientsVector;

            coefficientFile = argv[5];

            // -- Parse the coefficients file
            coefficientsVector = fileParser.parseCoeffs(coefficientFile);

            // -- Check that the number of set of coefficients is equal to the number of filters supplied in the commandline
            uint16_t coefficientsVectorSize = (uint16_t)coefficientsVector.size();
            validator.validSetOfCoefficients(coefficientsVectorSize, filterCount);

            for (uint16_t i = 0; i < coefficientsVectorSize; i++)
            {
                // -- Get the number of coefficients in each set
                uint16_t coeffsNum = (uint16_t)coefficientsVector[i].size();

                // -- Process the wav file with each set of custom coefficients
                wav.processFirFilter(coefficientsVector[i], coeffsNum);
            }
        }
        else
        {
            cout << "Error! invalid value for default filter. Make sure default_filter is only one of the following "
                 << "y (default filter), n (custom coefficients)";
            exit(1);
        }
    }

    // -- Create a new wav file and see if it was successful
    fp = fopen(argv[2], "wb"); // -- Write in binary mode
    if (fp == NULL)
    {
        cout << "Error! could not create file "
             << argv[2];
        exit(1);
    }

    // -- Write the contents of the output audio file
    wav.writeWavFile(fp);
    fclose(fp);
}