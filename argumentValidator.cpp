/**
 * @file argumentValidator.cpp
 * @author Pranali Rathi (pranali.r@fourthoracle.com)
 * @brief Implementation of the argument validator utility class
 * 
 * @version 0.1
 * @date 2021-12-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string>
#include "argumentValidator.hpp"

using namespace std;

argumentValidator::argumentValidator()
{
    // -- Default constructor
}

void argumentValidator::requiredArgsPresent(uint16_t args)
{
    // -- Depending on the type of filter and number of filters, command line arguments should be between 6 and 9
    if (args < 6 || args > 9)
    {
        cout << "Error! incorrect number of commandline arguments: The following arguments are required in the specified order\n\n"
             << "- input_filename: specifies the name of the input wav file. The extension \".wav\" must be included.\n\n"
             << "- output_filename: specifies the name of the output wav file. The extension \".wav\" must be included.\n\n"
             << "- default_filter: specifies if a default filter is used or custom coefficients are to be used. Options include:"
             << "y (default filter), n (custom coefficients)\n\n"
             << "- filter_count: Specifies the number of filters to be applied."
             << "- filter_types: this argument is only valid if using a default filter. Specifies the types of filters to be used, up to a maximum"
             << " of 4 can be supplied. The options include: lp, hp, bp, bs.\n\n"
             << "- coefficient_filename: this argument is only valid if using custom coefficients. Specifies the name of the text file with the coefficient values. "
             << "The extension \".txt\" must be included.\n\n";
        exit(1);
    }
}

int16_t argumentValidator::validFilterCount(string filterCountString)
{
    // -- Parse and ensure filterCount is <= 0
    int16_t filterCount = -1;
    try
    {
        filterCount = (int16_t)stoi(filterCountString);
    }
    catch (const invalid_argument &)
    {
        cout << "Error! Invalid argument for filter_count. Please make sure its an integer greater than or equal to 0";
        exit(1);
    }
    catch (const out_of_range &)
    {
        cout << "Error! Filter_count is out of range for a int16_t";
        exit(1);
    }

    if (filterCount <= 0)
    {
        cout << "Error! filter_count is not an integer greater than or equal to 0";
        exit(1);
    }

    return filterCount;
}

void argumentValidator::validFilterCountAndNumOfArgs(uint16_t filterCount, uint16_t args)
{
    // -- For default fiters, make sure the the number of commandline arguments corresponds to filter count
    if ((filterCount == 1 && args != 6) ||
        (filterCount == 2 && args != 7) ||
        (filterCount == 3 && args != 8) ||
        (filterCount == 4 && args != 9))
    {
        cout << "Error! The number of arguments provided does not correspond properly with the filter_count supplied";
        exit(1);
    }
}

void argumentValidator::validSetOfCoefficients(uint16_t numSetOfCoefficients, uint16_t filterCount)
{
    // -- For custom fiters, make sure the the number of parsed sets of coefficients corresponds to filter count
    if (numSetOfCoefficients != filterCount)
    {
        cout << "Error! The number of set of coefficients parsed ("
             << numSetOfCoefficients
             << ") does not correspond with the filter_count ("
             << filterCount
             << ") supplied";
        exit(1);
    }
}