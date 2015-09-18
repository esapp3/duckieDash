    /*****************************************
    These are Helper Methods /Definitions Specific to This Game
     ******************************************/

#include "gameHelp.h"
#include "myLib.h"


// *** Sprites =========================================================

        //for everything else


// *** Music =========================================================


SOUND backgroundSound;
SOUND shooterSound;

void setupSounds()
{
    REG_SOUNDCNT_X = SND_ENABLED;

    REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 | 
                     DSA_OUTPUT_RATIO_100 | 
                     DSA_OUTPUT_TO_BOTH | 
                     DSA_TIMER0 | 
                     DSA_FIFO_RESET |
                     DSB_OUTPUT_RATIO_100 | 
                     DSB_OUTPUT_TO_BOTH | 
                     DSB_TIMER1 | 
                     DSB_FIFO_RESET;

    REG_SOUNDCNT_L = 0;
}

void backgroundMusic( const unsigned char* sound, int length, int frequency) 
{
    dma[1].cnt = 0;
    backgroundVCount = 0;
	
    int interval = 16777216/frequency;
                
    DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);
	
    REG_TM0CNT = 0;
    REG_TM0D = -interval;
    REG_TM0CNT = TIMER_ON;
        
    backgroundSound.data = sound;
    backgroundSound.length = length;
    backgroundSound.frequency = frequency;
    backgroundSound.isPlaying = 1;
    backgroundSound.loops = loops;
    backgroundSound.duration = ((60*length)/frequency) - ((length/frequency)*3)-1;
}


void eatFishFX(const unsigned char* sound, int length, int frequency) 
{
    dma[2].cnt = 0;
    shooterVCount = 0;

    int interval = 16777216/frequency;
        
    DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    REG_TM1CNT = 0;
    REG_TM1D = -interval;
    REG_TM1CNT = TIMER_ON;
        
    shooterSound.data = sound;
    shooterSound.length = length;
    shooterSound.frequency = frequency;
    shooterSound.isPlaying = 1;
    shooterSound.loops = loops;
    shooterSound.duration = ((60*length)/frequency) - ((length/frequency)*3)-1;	
}

void muteSound() 
{
    backgroundSound.isPlaying = 0;
    shooterSound.isPlaying = 0;
    REG_TM0CNT = 0;
    REG_TM1CNT = 0;
}

void unmuteSound() 
{
    backgroundSound.isPlaying = 1;
    shooterSound.isPlaying = 1;
    REG_TM0CNT = TIMER_ON;
    REG_TM1CNT = TIMER_ON;
}

void stopSounds() 
{
    dma[1].cnt = 0;
    dma[2].cnt = 0;       
}

void setupInterrupts()
{
    REG_IME = 0;
    REG_INTERRUPT = (unsigned int)interruptHandler;
    REG_IE |= INT_VBLANK;
    REG_DISPSTAT |= INT_VBLANK_ENABLE;
    REG_IME = 1;
}

void interruptHandler()
{
    REG_IME = 0;
    if(REG_IF & INT_VBLANK)
    {
        if (backgroundSound.isPlaying) 
        {
            backgroundVCount++;
        }
        if (shooterSound.isPlaying)
        {
            shooterVCount++;
        }
        
        if (backgroundVCount > backgroundSound.duration)
        {
            //loop
            backgroundSound.isPlaying = 0;
            backgroundMusic(backgroundSound.data,backgroundSound.length,backgroundSound.frequency);
                    
                }
                if (shooterVCount > shooterSound.duration)
                {
                    //notloop
                    shooterSound.isPlaying = 0;
                    dma[2].cnt = 0;
                }
                
		REG_IF = INT_VBLANK; 
	}

	REG_IME = 1;
}
