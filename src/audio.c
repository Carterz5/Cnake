#include "audio.h"




void play_sound (const char* fileName) {
  
    ALuint buffer;
    ALuint source;
    ALint status;

    buffer = alutCreateBufferFromFile(fileName);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);

 
    // do
    //     {
    //     alutSleep (0.1f);
    //     alGetSourcei (source, AL_SOURCE_STATE, &status);
    //     }
    // while (status == AL_PLAYING);
}


void loop_sound (const char* fileName){

    ALuint buffer;
    ALuint source;
    ALint status;


    buffer = alutCreateBufferFromFile(fileName);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, 1);
    alSourcef(source, AL_GAIN, 0.5);
    alSourcePlay(source);

}