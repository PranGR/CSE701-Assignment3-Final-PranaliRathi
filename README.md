# Simple FIR Filter

This Simple FIR Filter is a simple audio processing program that allows the user to filter specified frequencies from a 16-bit input audio wave file.
The FIR filter requires a set of filter coefficients that are configurable, depending on the frequencies to be filtered. This program comes equipped with 4 different sets of filter coefficients that the user can select from, along with the ability to specify custom filters by providing a coefficients file. The user can also specify multiple filters so that the input wav file is run through multiple iterations of processing for further customization of the frequency filtering.

## Program Background and Structure

WAVE files (.wav) are a type of uncompressed audio file. They are made of the a header portion and actual audio data. The following image depicts the structure of wav files.

![WAVE File Format](http://soundfile.sapp.org/doc/WaveFormat/wav-sound-format.gif)

In this program, the header information and the audio data are separated into their own data structures. This makes processing of the wave file easier, as its only the audio data that must actually be modified. When creating the new output wave file, the header information of the original input wave file is copied over as is and the audio data is replaced by the filtered output. To generate the filtered output, the program applies a FIR filter to the audio data.

FIR filters work by shifting input samples through a filter buffer, multiplying each sample by the filter coefficients and accumulating those values, per sample. These accumulated samples are the final output values of the filter. For a detailed explanation of the mechanism behind FIR filter, see reference 1. In the case of this program, there is a limited number of samples that can be filtered at one time in order to reduce memory usage. Thus the total audio input data must be batched into appropriate chunks and are processed separately and then combined into a final output, which becomes the output audio data that is written to the output file.

The ability to filter out specific frequencies with a FIR filter is heavily dependant on the filter coefficients used for processing. As previously stated, this program offers the user the choice to use default filters or specify custom filters. The coefficients used for the default filter were generated using the online coefficient generator in reference 3. These default filter options are as follows:

* low pass filter (`lp`): cuts off at 1000 Hz
* high pass filter (`hp`): cuts off below 5000 Hz
* speech filter (`bp`): a bandpass filter that filters out everything but speech frequencies  (80 - 450 Hz)
* no speech filter (`bs`): a bandstop filter that filters out frequencies around the speech frequencies (80 - 450 Hz)

As this program allows users to use multiple filters on the same wav file, a maximum of 4 filters can be specified when using default filters since this program only provides 4 default filter types. This condition does not apply for custom filters

For custom filters, the user must specify sets of coefficients in a text file. Each set of coefficients should be enclosed in square brackets and individual coefficients should be separated by commas. Each set of coefficients should be separated by commas. There is no limit to the number of sets of coefficients that can be supplied and the program will just continue to iterate through all sets of coefficients.

Once the audio has been filtered as desired using the above options, the processed audio is then stored into the specified output file.

## Running the Program

The overall program consists of the files `audioFilter.cpp`, `argumentValidator.cpp`, `argumentValidator.hpp`, `coeffFileParser.cpp`, `coeffFileParser.hpp`, `defaultFilterCoeffs.hpp`, `firFilter.cpp`, `firFilter.hpp`, `wavFile.cpp`, `wavFile.hpp`, `wavHeader.cpp` and `wavHeader.hpp`. The user will need to compile all the `.cpp` files and then run the resulting executable. The following command will allow the user to compile the project with g++:

`g++ -o AudioFilter.exe audioFilter.cpp argumentValidator.cpp coeffFileParser.cpp firFilter.cpp wavFile.cpp wavHeader.cpp`

When run, the program expects the following command line arguments:

* `input_filename`: specifies the name of the input wav file.
* `output_filename`: specifies the name of the output wav file.
* `default_filter`: specifies if a default filter is used or custom coefficients are to be used. Options include: `y` (default filter), `n` (custom coefficients).
* `filter_count`: Specifies the number of filters to be applied
* `filter_types`: this argument is only valid if using a default filter. Specifies the types of filters to be used, up to a maximum of 4 can be supplied. The options include:
                `lp`, `hp`, `bp`, `bs`.
* `coefficient_filename`: this argument is only valid if using custom coefficients. Specifies the name of the text file with the coefficient values.

It is important to once again note that the input wav file must use 16-bit audio data. An example input file has been provided (`TestStarWars3.wav`) along with an example coefficients file (`coeffsFile.txt`). Once the program has completed its run, the processed audio will be saved under the output file name. Error checks and error messages have also been placed throughout the code to ensure that the user can quickly debug any issues that may occur.

## Example Inputs and Outputs

Assume `coeffsFile.txt` has 2 valid sets of coefficients.
Assume `wrongCoeffsFile.txt` has invalid characters in it.
Assume the executable to run is `AudioFilter`.

**Correct Input**
Command: `AudioFilter TestStarWars3.wav output.wav y 1 lp`
The result will be a newly generated audio wave file named `output.wav` which has the audio data from the `TestStarWars3.wav` file filtered through the default low pass filter

Command: `AudioFilter TestStarWars3.wav output.wav n 2 coeffsFile.txt`
The result will be a newly generated audio wave file named `output.wav` which has the audio data from the `TestStarWars3.wav` file filtered through `2` custom filters whose coefficients are specified in the file `coeffsFile.txt`.

**Incorrect Number of Arguments**
Command: `AudioFilter TestStarWars3.wav output.wav y 2 lp`
Output: `Error! The number of arguments provided does not correspond properly with the filter_count supplied`

**Incorrect filter_count Value**
Command: `AudioFilter TestStarWars3.wav output.wav n 4 coeffsFile.txt`
Output: `Error! The number of set of coefficients parsed (2) does not correspond with the filter_count (4) supplied`

**Invalid Coefficient Data**
Command: `AudioFilter TestStarWars3.wav output.wav n 1 wrongCoeffsFile.txt`
Output: `Error! Invalid coefficient value found in coefficient set 1`

## References Used

[1]<https://barrgroup.com/embedded-systems/how-to/digital-filters-fir-iir>
[2]<http://soundfile.sapp.org/doc/WaveFormat/>
[3]<https://www.arc.id.au/FilterDesign.html>
[4]<https://www2.cs.uic.edu/~i101/SoundFiles/>
