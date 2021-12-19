/**
 * @file coeffFileParser.hpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Class definition for the coefficient file parser class
 * 
 * This is a helper class to help parse the file that contains the list of sets of Coefficients
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
#include <tuple>
#include <cstdint>

using namespace std;

class coeffFileParser
{
public:
    /**
     * @brief Default constructor to create a coeff File Parser object
     * 
     */
    coeffFileParser();

    /**
     * @brief Parses the coefficients text file  
     * 
     * Parses the coefficients text file and returns the sets of coefficients.
     * 
     * @param inputFile The name of the coefficients text file
     * @return The list of sets of coefficients
     */
    vector<vector<double>> parseCoeffs(string inputFile);

private:
    /**
     * @brief Delete the temp file
     * 
     * While parsing the coefficients text file, a temporary file is created.
     * Delete that file once it is no longer needed
     * 
     * @param tempFile The Name of the temporary file
     */
    void deleteTempFile(string tempFile);

    /**
     * @brief Prepare the contents of the coefficients text file to be parsed
     * 
     * Reads the contents of the coefficients text file and writes it to a temporary
     * file after removing all whitespaces. This temporary file is then used to further
     * process the coefficient data
     * 
     * @param inputFile The name of the coefficients text file
     * @param tempFile The name of the temporary file
     */
    void preProcessInputFile(string inputFile, string tempFile);

    /**
     * @brief Get the next of coefficients as a string
     * 
     * Parses the input string to determine what the next set of coefficients is
     * 
     * @param line a string containing the contents of the temporary file to be parsed
     * @return a tuple containing a vector of strings of the current set of coefficients
     *         and the index for the start of the next set of coefficients in the input
     *         string
     */
    tuple<vector<string>, int32_t> getNextCoeffsSetString(string line);
};
