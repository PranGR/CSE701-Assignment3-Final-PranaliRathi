/**
 * @file argumentValidator.hpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Class definition for the argument validator utility class
 * 
 * This is a helper class which contains functions that help validate the command line arguments
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

class argumentValidator
{
public:
    /**
    * @brief Default constructor to create an argumentValidator object
    * 
     */
    argumentValidator();

    /**
     * @brief Confirms that there are the appropriate amount of command line arguments
     * 
     * Checks that args is between the minimum and maximum allowable number of arguments.
     * If not, print error statements
     * 
     * @param args Number of command line arguments supplied
     */
    void requiredArgsPresent(uint16_t args);

    /**
     * @brief Confirms that the filter count supplied is valid
     * 
     * Pareses and confirms that the filter count supplied is a valid integer greater than
     * or equal to 0. If not, print error statements
     * 
     * @param filterCountString command line argument corresponding to the filter count
     * @return Filter Count as an integer 
     */
    int16_t validFilterCount(string filterCountString);

    /**
     * @brief Confirms the number of command line arguments supplied corresponds to filter count
     * 
     * When default filters are chosen, the number of supplied arguments should correspond to the
     * filter count supplied. If not, print error statements
     * 
     * @param filterCount Filter count supplied 
     * @param args Number of command line arguments supplied
     */
    void validFilterCountAndNumOfArgs(uint16_t filterCount, uint16_t args);

    /**
     * @brief Confirms that number of custom coefficient sets correspons to filter count
     * 
     * When custom filters are chosen, the number of parsed sets of coefficients should correspond to 
     * the filter count supplied. If not, print error statements
     * 
     * @param numSetOfCoefficients Number of parsed sets of coefficients
     * @param filterCount Filter count supplied 
     */
    void validSetOfCoefficients(uint16_t numSetOfCoefficients, uint16_t filterCount);
};
