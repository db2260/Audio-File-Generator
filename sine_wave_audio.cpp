//This program creates a wav file from scratch.
//Metadata:
//Format chunk [fmt] - describes how the audio is saved and how to play the audio back -  such as compression code, number of channels, sample rate, bit rate
//Data chunk - contains the entirety of the sampled audio data
//Header chunk [Resource Interchange File Format (RIFF)] - encapsulates the other two chunks - generic file container format based on the concept of chunks
//A .wav file is 44 bytes in length

#include <iostream>
//#include <math.h>
//#include <cmath>
#include <fstream>

#define M_PI 3.14159

//using namespace std;

const int sampleRate = 44100;
const int bitDepth = 16;

class SineWaveOscillator {
    float frequency, amplitude, angle = 0.0f, offset = 0.0;
public:
    SineWaveOscillator(float freq, float amp) : frequency(freq), amplitude(amp) {
        offset = 2 * M_PI * frequency / sampleRate;
    }
    float process() {
        auto sample = amplitude * sin(angle);
        angle += offset;
        return sample;
    }
};


//Helper function for writing to files
void writeToFile(std::ofstream& file, int val, int size) {
    file.write(reinterpret_cast<const char*> (&val), size);
}

int main() {
    //Sample for two seconds
    int duration = 2;

    //Create a file, open it in binary mode
    std::ofstream audioFile;
    audioFile.open("waveform.wav", std::ios::binary);

    SineWaveOscillator osc(440, 0.5);

    //Header chunk
    audioFile << "RIFF";
    audioFile << "----";
    audioFile << "WAVE";

    //Format chunk
    audioFile << "fmt ";
    writeToFile(audioFile, 16, 4); //Size
    writeToFile(audioFile, 1, 2); //Compression code
    writeToFile(audioFile, 1, 2); //Number of channels
    writeToFile(audioFile, sampleRate, 4); //Sample Rate
    writeToFile(audioFile, sampleRate * bitDepth * 1 / 8, 4); //Average number of bytes per second
    writeToFile(audioFile, 2, 2); //Block align
    writeToFile(audioFile, bitDepth, 2); //Bits per sample (bit depth)

    //Data chunk
    audioFile << "data";
    audioFile << "----";

    //Returns the position of pointer before going through the data
    int preAudioPos = audioFile.tellp();

    auto maxAmplitude = pow(2, bitDepth - 1) - 1;

    for (int i = 0; i < (sampleRate * duration); i++) {
        //audioFile << sineWaveOscillator.process() << endl;
        auto sample = osc.process();
        int intSamp = static_cast<int> (sample * maxAmplitude);
        //audioFile.write(reinterpret_cast<char*> (&intSamp), 2);
        writeToFile(audioFile, intSamp, 2);

    }

    //Returns the position of pointer after going through the data
    int postAudioPos = audioFile.tellp();

    //The counter is offset by 4 bytes, and that is where the pointer will point to
    audioFile.seekp(preAudioPos - 4);

    //Size of the data chunk
    writeToFile(audioFile, postAudioPos - preAudioPos, 4);

    //The pointer is offset 4 bytes from the beginning of the file
    audioFile.seekp(4, std::ios::beg);

    //Size of the header chunk
    writeToFile(audioFile, postAudioPos - 8, 4);


    //Close the file
    audioFile.close();

}