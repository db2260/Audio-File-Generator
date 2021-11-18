# sine-wave-audio
# This program creates a wav file from scratch.
# Metadata:
# Format chunk [fmt] - describes how the audio is saved and how to play the audio backwards -  such as compression code, number of channels, sample rate, bit rate
# Data chunk - contains the entirety of the audio data - such as samples
# Header chunk [Resource Interchange File Format (RIFF)] - encapsulates the other two chunks - generic file container format based on the concept of chunks
# A .wav file is 44 bytes in length
