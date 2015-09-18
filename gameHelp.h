
#ifndef GAMEHELP_H
#define	GAMEHELP_H

// *** Sprites =========================================================

#define HEIGHTMIN 0
#define HEIGHTMAX 512
#define WIDTHMIN 0
#define WIDTHMAX 512

typedef struct {
    int row;
    int col;
    int rdel;
    int cdel;
    int bigRow;
    int bigCol;
    int shape;
    int size;
    int height;
    int width;
    int offset;
    int facing;
    int frame;
    int previousState;
    int frameCount;
    int fishCaught;
    int doorOpened;
} Sprite;

void pauseWords();
void winWords();


// *** Music =========================================================

typedef struct{
    unsigned char* data;
    int length;
    int frequency;
    int isPlaying;
    int loops;
    int duration;
}SOUND;

int backgroundVCount;
int shooterVCount;
int loops;

void setupSounds();
void backgroundMusic( const unsigned char* sound, int length, int frequency);
void eatFishFX( const unsigned char* sound, int length, int frequency);
void muteSound();
void unmuteSound();
void stopSounds();
void setupInterrupts();
void interruptHandler();

#endif	/* GAMEHELP_H */

