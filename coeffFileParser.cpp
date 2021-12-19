/**
 * @file coeffFileParser.cpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Implementation  of the coefficient file parser class
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include <cstdio>
#include <cstring>
#include "coeffFileParser.hpp"

using namespace std;

coeffFileParser::coeffFileParser()
{
    // -- Default constructor
}

void coeffFileParser::deleteTempFile(string tempFile)
{
    int32_t deleteStatus = -1;
    const char *cTempFile = tempFile.c_str();

    // -- Delete the temp file and check that it was successful
    deleteStatus = remove(cTempFile);
    if (deleteStatus != 0)
    {
        // -- This is not a terminating error, but the user should know
        cout << "Error Unable to delete file "
             << tempFile;
    }
}

void coeffFileParser::preProcessInputFile(string inputFile, string tempFile)
{
    string s;
    string tempString;

    // -- Make sure coefficients file is able to be read
    ifstream input(inputFile);
    if (!input.is_open())
    {
        cout << "Error Unable to open file "
             << inputFile
             << ". Please make sure the file name is correct";
        exit(1);
    }

    input.clear();
    input.seekg(0);

    // -- Store the contents of the coefficients file into a temp file, removing all spaces
    ofstream temp(tempFile);
    if (!temp.is_open())
    {
        cout << "Error Unable to create file "
             << tempFile;
        exit(1);
    }

    temp.clear();

    while (getline(input, s))
    {
        stringstream stream(s);

        while (getline(stream, tempString, ' '))
        {
            temp << tempString;
        }
    }

    input.close();
    temp.close();
}

tuple<vector<string>, int32_t> coeffFileParser::getNextCoeffsSetString(string line)
{
    vector<string> coeffStrings;
    int32_t index1, index2, index3;

    index1 = (int32_t)line.find("[");
    index2 = (int32_t)line.find("],");
    index3 = (int32_t)line.find("]");

    // -- Check if matching [] brackets exist that encapsulate a set of coefficients
    if (index1 != 0 || index1 == -1 || (index2 == -1 && index3 == -1))
    {
        return make_tuple(coeffStrings, -1);
    }

    // -- Get the next set of coefficients string
    int32_t length, start, end;
    string set;
    start = index1 + 1;
    end = index2 - 1;
    length = end - start;
    set = line.substr(start, length);

    // -- Parse the set string to get individual coefficient strings and store them in a vector
    stringstream coeffStream(set);
    string coeff;
    while (getline(coeffStream, coeff, ','))
    {
        coeffStrings.push_back(coeff);
    }

    // -- Return the coeffStrings and the index position of the beginning of the next set of coefficients
    // -- from the input line
    int32_t newIndex;
    if (index2 != -1)
    {
        newIndex = index2 + 2;
    }
    else
    {
        newIndex = index3 + 1;
    }

    return make_tuple(coeffStrings, newIndex);
}

vector<vector<double>> coeffFileParser::parseCoeffs(string inputFile)
{
    uint16_t numCoeffSets = 0;
    string line = "";
    string tempFile = "tempCoeffs.txt";
    vector<vector<double>> filterCoeffs;

    // -- Pre-process the contents of the input file and copy it into the temp file
    preProcessInputFile(inputFile, tempFile);

    // -- Read the temp file
    ifstream temp(tempFile);
    if (!temp.is_open())
    {
        cout << "Error Unable to open file "
             << tempFile;
        exit(1);
    }
    temp.clear();
    temp.seekg(0);
    getline(temp, line);

    while (line.length() > 0)
    {
        numCoeffSets++;
        // -- Get the next set of coefficients as strings
        vector<string> coeffStrings;
        int32_t newIndex = -1;
        double coeffValue = 0.0;
        tie(coeffStrings, newIndex) = getNextCoeffsSetString(line);

        if (newIndex == -1)
        {
            // -- Unable to find matching[] brackets that encapsulate the current set of coefficients
            cout << "Error! Unable to find matching[] brackets that encapsulate the set of coefficients in coefficient set "
                 << numCoeffSets;
            temp.close();
            exit(1);
        }

        // -- Convert coeffStrings to double values and store the values in a vector
        vector<double> coeffValues;
        for (int32_t i = 0; i < (int32_t)coeffStrings.size(); i++)
        {
            try
            {
                coeffValue = (double)stod(coeffStrings[i]);
            }
            catch (const invalid_argument &)
            {
                cout << "Error! Invalid coefficient value found in coefficient set "
                     << numCoeffSets;
                temp.close();
                exit(1);
            }
            catch (const out_of_range &)
            {
                cout << "Error! A coefficient value is out of range for a double in coefficient set "
                     << numCoeffSets;
                temp.close();
                exit(1);
            }
            coeffValues.push_back(coeffValue);
        }

        // -- Store the coeffValues vector into the return vector
        filterCoeffs.push_back(coeffValues);

        // -- Set the new starting point of the line to be parsed on the next iteration
        line = line.substr(newIndex);
    }

    // -- Try to delete the temp file
    temp.close();
    deleteTempFile(tempFile);

    // -- Print coefficients parsed for user to view
    for (int32_t i = 0; i < (int32_t)filterCoeffs.size(); i++)
    {
        cout << "\nCoefficient Set "
             << i + 1
             << ": [ ";

        for (int32_t j = 0; j < (int32_t)filterCoeffs[i].size(); j++)
        {
            if (j == (int32_t)filterCoeffs[i].size() - 1)
            {
                cout << filterCoeffs[i][j];
            }
            else
            {
                cout << filterCoeffs[i][j]
                     << ", ";
            }
        }

        cout << " ]\n\n";
    }

    return filterCoeffs;
}