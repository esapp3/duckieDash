    /*****************************************
    I am making a game where the player must collect a fish to open a
     door to a new level or win state. 

    Currently, I have a working player sprite, a sprite that represents
     an object to collide with (works as both fish and door), and Sprites
     that are used for words and numbers.  
    ******************************************/


#include <stdlib.h> 
#include <stdio.h>
#include "myLib.h"
#include "Sprites.h"
#include "bath.h"
#include "background1.h"
#include "collisionwater.h"
#include "gameHelp.h"
#include "duckieSong.h"
#include "biteSoundFX.h"
#include "whiteTiles.h"
#include "Controls.h"
#include "HowPlay.h"
#include "HowPlay2.h"
#include "sound4.h"
#include "collisionfull.h"

void start();
void setUpSplash();
void instructions(); 
void setUpGame();
void resetLevel();
void game();
void updateGameFishAlive();
void updateGameFishEaten();
void updateEnemy();
void animate();
void pause();
void win();
void lose();
void pauseWords();
void hideSprites();
void winWords();

unsigned int buttons;
unsigned int oldButtons;

int hOff;
int vOff;
int directionX;
int directionY;

const u16 *spriteTiles = SpritesTiles;
const u16 *spritePalette = SpritesPal;

ObjAttr shadowOAM[128];

int state;
int level;
int instPage;

enum {STARTSCREEN, PLAYSCREEN, PAUSESCREEN, INFOSCREEN, LOSESCREEN, WINSCREEN}; 
enum {PLAYER_LEFT, PLAYER_RIGHT, PLAYER_UP, PLAYER_DOWN, PLAYER_IDLE};

Sprite player;
Sprite words;
Sprite levelNum;
Sprite target;
Sprite enemy;
Sprite enemy2;
Sprite enemy3;
Sprite enemy4;

int main()
{             
    state = STARTSCREEN;

    while(1)
    {
        oldButtons = buttons;
	buttons = BUTTONS;
        
	switch(state)
	{
            case STARTSCREEN:
                setUpSplash();
                start();
                break;
            case PLAYSCREEN:
                game();
                break;
            case PAUSESCREEN:
                pause();
                break;
            case INFOSCREEN:
                instructions();
                break;
            case LOSESCREEN:
                lose();
                break;
            case WINSCREEN: 
                win(); 
                break;
	}
 
        waitForVblank();
        DMANow(3, shadowOAM, SPRITEMEM, 512);           
    }
    return 0;
}



void start()
{
    if(BUTTON_PRESSED(BUTTON_START))
    {
        setUpGame();
        state = PLAYSCREEN;
    }  
    if(BUTTON_PRESSED(BUTTON_A))
    {
        instPage = 1;
        drawBackgroundImage4(HowPlayBitmap); 
        loadPalette(HowPlayPal);  
        state = INFOSCREEN;
    }    
}

void setUpSplash() 
{
    stopSounds();
    SetMode(MODE4 | BG2_ENABLE);
    drawBackgroundImage4(bathBitmap); 
    loadPalette(bathPal);  
}

void instructions()
{
    if(BUTTON_PRESSED(BUTTON_A) && instPage==1)
    {
        drawBackgroundImage4(HowPlay2Bitmap); 
        loadPalette(HowPlay2Pal); 
        instPage = 2;
    } 

    if(BUTTON_PRESSED(BUTTON_A) && instPage==2)
    {
        while(KEY_DOWN_NOW(BUTTON_A));
        while(!KEY_DOWN_NOW(BUTTON_A));
        while(KEY_DOWN_NOW(BUTTON_A));
        drawBackgroundImage4(ControlsBitmap); 
        loadPalette(ControlsPal); 
        instPage = 3;
    } 
    
    if(BUTTON_PRESSED(BUTTON_A) && instPage==3)
    {
        while(KEY_DOWN_NOW(BUTTON_A));
        while(!KEY_DOWN_NOW(BUTTON_A));
        while(KEY_DOWN_NOW(BUTTON_A));
        state = STARTSCREEN;
        instPage = 0;
    }    
}

void setUpGame() 
{
    SetMode(MODE0| BG1_ENABLE | BG3_ENABLE | OBJ_ENABLE);
    
    REG_BG1CNT = BG_SIZE3 | CBB(0) | COLOR256 | SBB(28); // Bathtub Background, in middle
    REG_BG3CNT = BG_SIZE0 | CBB(2) | COLOR256 | SBB(27); // White Tiles, Farthest Back   
    
    DMANow(3, background1Tiles, &CHARBLOCKBASE[0], (background1TilesLen/2) | DMA_NOW);
    DMANow(3, background1Map, &SCREENBLOCKBASE[28], (background1MapLen/2) | DMA_NOW);
    
    DMANow(3, whiteTilesTiles, &CHARBLOCKBASE[2], (whiteTilesTilesLen/2) | DMA_NOW);
    DMANow(3, whiteTilesMap, &SCREENBLOCKBASE[27], (whiteTilesMapLen/2) | DMA_NOW);

    loadPalette(background1Pal);
    
    int i; 
    for(i=0; i<256; i++) { SPRITEPAL[i] = spritePalette[i]; }
    for(i=0; i<32768/2; i++) { CHARBLOCKBASE[4].tileimg[i] = spriteTiles[i]; }   
    
    for(i=0; i<128; i++) { shadowOAM[i].attr0 = ATTR0_HIDE; }  
    
    setupInterrupts();
    setupSounds();
    stopSounds();
    
    level = 1;
    levelNum.row = 10;
    levelNum.col = 10;
    levelNum.width = 16;
    levelNum.height = 32;     
    levelNum.offset = SPRITEOFFSET(7,7); 
     
    target.bigRow = 160;
    target.bigCol = 400;
    target.offset = SPRITEOFFSET(15,9);    
    
    enemy.shape = ATTR0_SQUARE;
    enemy.size = ATTR1_SIZE16;
    enemy.height = 16;
    enemy.width = 16; 
    enemy.rdel = 1;
    enemy.cdel = 1;

    enemy.bigRow = 100;
    enemy.bigCol = 250;
    enemy.offset = SPRITEOFFSET(27, 7);
    
    enemy2.shape = ATTR0_SQUARE;
    enemy2.size = ATTR1_SIZE16;
    enemy2.height = 16;
    enemy2.width = 16; 
    enemy2.rdel = 1;
    enemy2.cdel = 1;

    enemy2.bigRow = 100;
    enemy2.bigCol = 290;
    enemy2.offset = SPRITEOFFSET(27, 10);
    
    enemy3.shape = ATTR0_SQUARE;
    enemy3.size = ATTR1_SIZE16;
    enemy3.height = 16;
    enemy3.width = 16; 
    enemy3.rdel = 1;
    enemy3.cdel = 1;

    enemy3.bigRow = 105;
    enemy3.bigCol = 320;
    enemy3.offset = SPRITEOFFSET(27, 13);  
    
    enemy4.shape = ATTR0_SQUARE;
    enemy4.size = ATTR1_SIZE16;
    enemy4.height = 16;
    enemy4.width = 16; 
    enemy4.rdel = 1;
    enemy4.cdel = 1;

    enemy4.bigRow = 135;
    enemy4.bigCol = 35;
    enemy4.offset = SPRITEOFFSET(27, 4);    
    
    backgroundMusic(duckieSong, DUCKIESONGLEN, DUCKIESONGFREQ);
    
    resetLevel();
}

void resetLevel() 
{
    if(level==4)
    {
            stopSounds(); 
            backgroundMusic(duckieSong, DUCKIESONGLEN, DUCKIESONGFREQ);
    }
        
    hOff = 1;
    vOff = 90; 
    player.width = 32;
    player.height = 32; 
    player.row = 160/2-player.width/2;
    player.col = 240/2-player.height/2; 
    player.rdel = 1;
    player.cdel = 1;   
    player.shape = ATTR0_SQUARE;
    player.size = ATTR1_SIZE32;
    player.offset = SPRITEOFFSET(11,0);
    player.facing = PLAYER_RIGHT;
    player.frameCount = 0;
    player.frame = 0;
    
    levelNum.row = 10;
    levelNum.col = 10;
    levelNum.width = 16;
    levelNum.height = 32;
    
    target.shape = ATTR0_WIDE;
    target.size = ATTR1_SIZE16;
    target.rdel = 1;
    target.cdel = 1;
    target.width = 16;
    target.height = 8; 

    enemy.row = 160/2-enemy.width/2; 
    enemy.col = 240/2-enemy.height/2;     
    enemy.rdel = 1;
    enemy.cdel = 1;
    
    enemy2.row = 160/2-enemy2.width/2; 
    enemy2.col = 240/2-enemy2.height/2;     
    enemy2.rdel = 1;
    enemy2.cdel = 1;
    
    enemy3.row = 160/2-enemy3.width/2; 
    enemy3.col = 240/2-enemy3.height/2;     
    enemy3.rdel = 1;
    enemy3.cdel = 1;

    enemy4.row = 160/2-enemy4.width/2; 
    enemy4.col = 240/2-enemy4.height/2;     
    enemy4.rdel = 1;
    enemy4.cdel = 1;
        
    if(level==4)
    {
        levelNum.offset = SPRITEOFFSET(7,19);
        target.offset = SPRITEOFFSET(16,6); 
        target.bigRow = 220;
        target.bigCol = 284;
        enemy.bigRow = 124;
        enemy.bigCol = 62;  
        enemy.rdel = 0;
        enemy.cdel = 0;        
        enemy2.bigRow = 102;
        enemy2.bigCol = 142;   
        enemy2.rdel = 0;
        enemy2.cdel = 0;
        enemy3.bigRow = 98;
        enemy3.bigCol = 304;   
        enemy3.rdel = 0;
        enemy3.cdel = 0;
        enemy4.bigRow = 122;
        enemy4.bigCol = 400;   
        enemy4.rdel = 0;
        enemy4.cdel = 0;         
    }
    else if(level==3)
    {
        levelNum.offset = SPRITEOFFSET(7,15);            
        target.offset = SPRITEOFFSET(16,2); 
        target.bigRow = 200;
        target.bigCol = 400;  
        enemy.rdel = 3;
        enemy.cdel = 3;        
        enemy2.bigRow = 190;
        enemy2.bigCol = 320;   
        enemy2.rdel = 3;
        enemy2.cdel = 3;
        enemy3.bigRow = 210;
        enemy3.bigCol = 320;   
        enemy3.rdel = 3;
        enemy3.cdel = 3;        
    }
    else if (level==2)
    {
        levelNum.offset = SPRITEOFFSET(7,11);            
        target.offset = SPRITEOFFSET(15,13);
        target.bigRow = 190;
        target.bigCol = 470; 
        enemy.rdel = 3;
        enemy.cdel = 3;        
        enemy2.bigRow = 190;
        enemy2.bigCol = 320;   
        enemy2.rdel = 3;
        enemy2.cdel = 3;         
    }   
    
    player.fishCaught = 0;   
    player.doorOpened = 0;
}

void game()
{
    //tracks player's position based on movement
    player.bigRow = player.row + vOff;
    player.bigCol = player.col + hOff;
    
    //Updates the stationary player row/col screen position based on its world position
    target.row = target.bigRow - vOff;
    target.col = target.bigCol - hOff; 
    
    //Updates the stationary player row/col screen position based on its world position
    enemy.row = enemy.bigRow - vOff;
    enemy.col = enemy.bigCol - hOff;  

    enemy2.row = enemy2.bigRow - vOff;
    enemy2.col = enemy2.bigCol - hOff;  

    enemy3.row = enemy3.bigRow - vOff;
    enemy3.col = enemy3.bigCol - hOff;  

    enemy4.row = enemy4.bigRow - vOff;
    enemy4.col = enemy4.bigCol - hOff;      
    
    if (BUTTON_PRESSED(BUTTON_R))
    {
        muteSound();
    }
    
    if(BUTTON_PRESSED(BUTTON_START))
    {
        stopSounds();
        state = STARTSCREEN;
    }
    if(BUTTON_PRESSED(BUTTON_SELECT))
    {
        muteSound();
        state = PAUSESCREEN;
    }
    if(BUTTON_PRESSED(BUTTON_B))
    {
        player.fishCaught = 1;
        player.rdel = 2;
        player.cdel = 2;
        target.size = ATTR1_SIZE32; 
        target.width = 32;
        target.height = 32;
        target.shape = ATTR0_SQUARE;
        target.offset = SPRITEOFFSET(13,25);
        target.bigRow = 512-32;
        target.bigCol = 512-32;  
        hOff = 512-240;
        vOff = 512-160;        
    }
    if (player.fishCaught==1 && player.doorOpened==1)
    { 
        if(level==4) 
        {
            stopSounds();
            state = WINSCREEN; 
        }
        else 
        {
            level++;
            resetLevel();
        }
    }  
    if (player.fishCaught == 0) 
    {
        updateGameFishAlive();
    }
    if (player.fishCaught == 1) 
    {
        updateGameFishEaten();
    } 
    updateEnemy();
    animate();
}


void pause()
{
    pauseWords();
    if(BUTTON_PRESSED(BUTTON_SELECT))
    {
        unmuteSound();
        hideSprites();
        state = PLAYSCREEN;
    }
    
    if(BUTTON_PRESSED(BUTTON_START))
    {
        stopSounds();
        state = PLAYSCREEN;
        setUpGame();
    }
}

void win() 
{
    winWords();
    if(BUTTON_PRESSED(BUTTON_START))
    {
        stopSounds();
        state = STARTSCREEN;
    } 
}

void lose()
{
    if(BUTTON_PRESSED(BUTTON_START))
    {
        state = PLAYSCREEN;
        setUpGame();
    }        
}

void updateGameFishAlive() 
{
    player.facing = PLAYER_IDLE;
    
    if(BUTTON_HELD(BUTTON_LEFT))
    {
        player.facing = PLAYER_LEFT;
        if (player.frame==0)
                player.offset = SPRITEOFFSET(11,6);
        if (player.frame==1)
                player.offset = SPRITEOFFSET(18,5);
        if (player.frame==2)
               player.offset = SPRITEOFFSET(22,5);        
        if(collisionwaterBitmap[OFFSET(player.bigRow + 1, player.bigCol-player.cdel, 512)]!=0) 
        {
            if((player.col < (240/2))&&hOff>0)
            {
                hOff = hOff - player.cdel;
            }  
            else if (player.col > 0)
                player.col = player.col - player.cdel;
        }
        player.previousState = player.facing;
        player.frameCount++; 
    }
    if(BUTTON_HELD(BUTTON_RIGHT))
    {
        player.facing = PLAYER_RIGHT;
        if (player.frame==0)
                player.offset = SPRITEOFFSET(11,0);
        if (player.frame==1)
                player.offset = SPRITEOFFSET(18,0);
        if (player.frame ==2)
               player.offset = SPRITEOFFSET(22,0);
        if(collisionwaterBitmap[OFFSET(player.bigRow, player.bigCol+player.cdel+player.width, 512)]!=0 && (player.col+player.width<512)) 
        {
            if((player.col > (240/2))&&hOff<512-240)
            {
                hOff = hOff + player.cdel;
            }  
            else
                player.col = player.col + player.cdel; 
        }
        player.previousState = player.facing;
        player.frameCount++; 
    }
    if(BUTTON_HELD(BUTTON_UP))
    {
        player.facing = PLAYER_UP;
            if (player.frame==0)
                player.offset = SPRITEOFFSET(14,18);
            if (player.frame==1)
                player.offset = SPRITEOFFSET(18,10);
            if (player.frame==2)
               player.offset = SPRITEOFFSET(22,10);          
        if(collisionwaterBitmap[OFFSET(player.bigRow-player.rdel, player.bigCol, 512)]!=0  &&(player.row>0)) 
        {
            if((player.row < (160/2))&&vOff>0)
            {
                vOff = vOff - player.rdel;
            }  
            else
                player.row = player.row - player.rdel; 
        }
        player.previousState = player.facing;
        player.frameCount++; 
    }
    if(BUTTON_HELD(BUTTON_DOWN))
    {
        player.facing = PLAYER_DOWN;
           if (player.frame==0)
                player.offset = SPRITEOFFSET(13,21);
           if (player.frame==1)
                player.offset = SPRITEOFFSET(17,15);
           if (player.frame==2)
               player.offset = SPRITEOFFSET(21,15);         
        if(collisionwaterBitmap[OFFSET(player.bigRow+player.rdel+player.height, player.bigCol, 512)]!=0 && (player.row<512)) 
        {
            if((player.row > (160/2))&&vOff<256-160)
            {
                vOff = vOff + player.rdel;
            }  
            else
                player.row = player.row + player.rdel;
        }
        player.previousState = player.facing;
        player.frameCount++; 
    }
    if ((((player.row+player.height)>(target.row+target.height)) && (player.row<(target.row)))
        && (((player.col)<target.col) && ((player.col+player.width)>(target.col+target.width))))
    {
            eatFishFX(biteSoundFX, BITESOUNDFXLEN, BITESOUNDFXFREQ);
            target.height = 32;
            target.width = 32;
            target.size = ATTR1_SIZE32; 
            target.shape = ATTR0_SQUARE;
            target.offset = SPRITEOFFSET(13,25);
            target.bigRow = 512-32;
            target.bigCol = 512-32; 
            player.fishCaught = 1;
            player.rdel = 2;
            player.cdel = 2;
            hideSprites();
            if(level==4)
            {
                stopSounds();
                backgroundMusic(sound4, SOUND4LEN, SOUND4FREQ);
            }
     }  
     if(player.frameCount%20==0) 
     {
        player.frameCount = 0;
	if (player.frame == 2)
	{
            player.frame = 0;
	}
	else
        {
            player.frame++;
	}
     }
    if(player.facing == PLAYER_IDLE)
    {
        player.frame = 0; 
        if(player.previousState==PLAYER_LEFT)
            player.offset = SPRITEOFFSET(11,6);
        if(player.previousState==PLAYER_RIGHT)
            player.offset = SPRITEOFFSET(11,0);
        if(player.previousState==PLAYER_UP)
            player.offset = SPRITEOFFSET(14,18);
        if(player.previousState==PLAYER_DOWN)
            player.offset = SPRITEOFFSET(13,21);        
    }  
}

void updateGameFishEaten()
{   
    if(BUTTON_HELD(BUTTON_LEFT))
    {
        if (level==4)
                player.offset = SPRITEOFFSET(3, 28);
        else        
        player.offset = SPRITEOFFSET(18,28);
        if(player.col>0) 
        {
            if((player.col < (240/2))&&hOff>0)
            {
                hOff = hOff - player.cdel;
            }  
            else
                player.col = player.col - player.cdel;
        }       
    }
    if(BUTTON_HELD(BUTTON_RIGHT))
    {
        if (level==4)
                player.offset = SPRITEOFFSET(3, 22);
        else
        player.offset = SPRITEOFFSET(18,22);
        if(((player.col+player.width)<512)) 
        {
            if((player.col > (240/2))&&hOff<512-240)
            {
                hOff = hOff + player.cdel;
            }  
            else
                if (player.bigCol+player.width<WIDTHMAX)
                player.col = player.col + player.cdel;   
        }
    }
    if(BUTTON_HELD(BUTTON_UP))
    {
         if (level==4)
                player.offset = SPRITEOFFSET(8, 22);
        else
        player.offset = SPRITEOFFSET(23, 23);
        if((player.row>0)) 
        {
            if((player.row < (160/2))&&vOff>0)
            {
                vOff = vOff - player.rdel;
            }  
            else
                player.row = player.row - player.rdel; 
        }
    }
    if(BUTTON_HELD(BUTTON_DOWN))
    {
        if (level==4)
                player.offset = SPRITEOFFSET(8, 27);
        else
            player.offset = SPRITEOFFSET(23, 28);
        if(((player.row+player.height)<512)) 
        {
            if((player.row > (160/2))&&vOff<512-160)
            {
                vOff = vOff + player.rdel;
            }  
            else 
            {
                if (player.bigRow+player.height<HEIGHTMAX)
                        player.row = player.row + player.rdel;             
            }
                     
        }
    }    
    if (((player.row+player.height)<(target.row+target.height+10)) && (player.row>(target.row-10))
            && (player.col>target.col-10) && ((player.col+player.width)<(target.col+10+target.width)))
    {
        player.doorOpened = 1;
    }
} 

void updateEnemy() 
{   
    //First Enemy
    if ((((player.row+player.height)>(enemy.row+enemy.height)) && (player.row<(enemy.row)))
        && (((player.col)<enemy.col) && ((player.col+player.width)>(enemy.col+enemy.width))))
    {
        resetLevel();
    } 
    if(collisionwaterBitmap[OFFSET(enemy.bigRow+enemy.rdel+enemy.height, enemy.bigCol, 512)]==0&&level!=4) 
    {
        enemy.rdel = enemy.rdel*-1;
    }

    //Second Enemy
     if ( level!=1 &&(((player.row+player.height)>(enemy2.row+enemy2.height)) && (player.row<(enemy2.row)))
        && (((player.col)<enemy2.col) && ((player.col+player.width)>(enemy2.col+enemy2.width))))
    {
        resetLevel();
    } 
    if(collisionwaterBitmap[OFFSET(enemy2.bigRow+enemy2.rdel+enemy2.height, enemy2.bigCol, 512)]==0&&level!=4) 
    {
        enemy2.rdel = enemy2.rdel*-1;
    }
    
    //Third Enemy
        if ((level==3||level==4)&&((((player.row+player.height)>(enemy3.row+enemy3.height)) && (player.row<(enemy3.row)))
        && (((player.col)<enemy3.col) && ((player.col+player.width)>(enemy3.col+enemy3.width)))))
    {
        resetLevel();
    } 
    if(collisionwaterBitmap[OFFSET(enemy3.bigRow, enemy3.bigCol+enemy3.cdel+enemy3.width, 512)]==0&&level!=4) 
    {
        enemy3.cdel = enemy3.cdel*-1;
    }
    
    //Fourth Enemy
        if (level==4&&(((player.row+player.height)>(enemy4.row+enemy4.height)) && (player.row<(enemy4.row)))
        && (((player.col+player.width)<enemy4.col) && ((player.col+player.width)>(enemy4.col+enemy4.width))))
    {
        resetLevel();
    } 
   
        int left = -1*enemy.rdel;
    int right = enemy.rdel;
    int up = -1*enemy.cdel;
    int down = enemy.cdel;
    
    if (level==1 || level==2 || level ==3)
    {
    enemy.bigRow = enemy.bigRow + enemy.rdel;    
    enemy2.bigRow = enemy2.bigRow + enemy2.rdel;    
    enemy3.bigCol = enemy3.bigCol + enemy3.cdel;
    enemy4.bigCol = enemy4.bigCol + enemy4.cdel;
    directionX=left;
    directionY=down;
    }
   
    
    if(level==4)
    {
        
        if(player.fishCaught==1)
        {
            enemy.rdel = 3;
            enemy.cdel = 3;
            enemy2.rdel = 3;
            enemy2.cdel = 3;
            enemy3.rdel = 3;
            enemy3.cdel = 3;
            enemy4.rdel = 3;
            enemy4.cdel = 3; 
            
            player.rdel = 3;
            player.cdel = 3;

            if (collisionfullBitmap[OFFSET(enemy.bigRow+enemy.rdel+enemy.height, enemy.bigCol, 512)]==0){enemy.bigRow = enemy.bigRow + enemy.rdel;}
            if (collisionfullBitmap[OFFSET(enemy.bigRow, enemy.bigCol+enemy.cdel+enemy.width, 512)]==0){enemy.bigCol = enemy.bigCol + enemy.cdel; }

            if(enemy.bigRow<0)
            {
                enemy.rdel = -enemy.rdel;
                enemy.bigRow = enemy.bigRow + enemy.rdel; 
            }
            if(enemy.bigCol<0)
            {
                enemy.cdel = -enemy.cdel;
                enemy.bigCol = enemy.bigCol + enemy.cdel;          
            }
            if(enemy.bigRow+enemy.height > 500)
            {
                enemy.rdel = -enemy.rdel;
                enemy.bigRow = enemy.bigRow + enemy.rdel; 
            }  
            if(enemy.bigCol+enemy.width > 500)
            {
                enemy.cdel = -enemy.cdel;
                enemy.bigCol = enemy.bigCol + enemy.cdel;    
            }              
             if (collisionfullBitmap[OFFSET(enemy2.bigRow+enemy2.rdel+enemy2.height, enemy2.bigCol, 512)]==0){enemy2.bigRow = enemy2.bigRow + enemy2.rdel;}
            if (collisionfullBitmap[OFFSET(enemy2.bigRow, enemy2.bigCol+enemy2.cdel+enemy2.width, 512)]==0){enemy2.bigCol = enemy2.bigCol + enemy2.cdel; }

            if(enemy2.bigRow<0)
            {
                enemy2.rdel = -enemy2.rdel;
                enemy2.bigRow = enemy2.bigRow + enemy2.rdel; 
            }
            if(enemy2.bigCol<0)
            {
                enemy2.cdel = -enemy2.cdel;
                enemy2.bigCol = enemy2.bigCol + enemy2.cdel;          
            }
            if(enemy2.bigRow+enemy2.height > 500)
            {
                enemy2.rdel = -enemy2.rdel;
                enemy2.bigRow = enemy2.bigRow + enemy2.rdel; 
            }  
            if(enemy2.bigCol+enemy2.width > 500)
            {
                enemy2.cdel = -enemy2.cdel;
                enemy2.bigCol = enemy2.bigCol + enemy2.cdel;    
            } 
            
            if (collisionfullBitmap[OFFSET(enemy3.bigRow+enemy3.rdel+enemy3.height, enemy3.bigCol, 512)]==0){enemy3.bigRow = enemy3.bigRow + enemy3.rdel;}
            if (collisionfullBitmap[OFFSET(enemy3.bigRow, enemy3.bigCol+enemy3.cdel+enemy3.width, 512)]==0){enemy3.bigCol = enemy3.bigCol + enemy3.cdel; }

            if(enemy3.bigRow<0)
            {
                enemy3.rdel = -enemy3.rdel;
                enemy3.bigRow = enemy3.bigRow + enemy3.rdel; 
            }
            if(enemy3.bigCol<0)
            {
                enemy3.cdel = -enemy3.cdel;
                enemy3.bigCol = enemy3.bigCol + enemy3.cdel;          
            }
            if(enemy3.bigRow+enemy3.height > 500)
            {
                enemy3.rdel = -enemy3.rdel;
                enemy3.bigRow = enemy3.bigRow + enemy3.rdel; 
            }  
            if(enemy3.bigCol+enemy3.width > 500)
            {
                enemy3.cdel = -enemy3.cdel;
                enemy3.bigCol = enemy3.bigCol + enemy3.cdel;    
            }
            
            
            if (collisionfullBitmap[OFFSET(enemy4.bigRow+enemy4.rdel+enemy4.height, enemy4.bigCol, 512)]==0){enemy4.bigRow = enemy4.bigRow + enemy4.rdel;}
            if (collisionfullBitmap[OFFSET(enemy4.bigRow, enemy4.bigCol+enemy4.cdel+enemy4.width, 512)]==0){enemy4.bigCol = enemy4.bigCol + enemy4.cdel; }

            if(enemy.bigRow<0)
            {
                enemy4.rdel = -enemy4.rdel;
                enemy4.bigRow = enemy4.bigRow + enemy4.rdel; 
            }
            if(enemy4.bigCol<0)
            {
                enemy4.cdel = -enemy4.cdel;
                enemy4.bigCol = enemy4.bigCol + enemy4.cdel;          
            }
            if(enemy4.bigRow+enemy4.height > 500)
            {
                enemy4.rdel = -enemy4.rdel;
                enemy4.bigRow = enemy4.bigRow + enemy4.rdel; 
            }  
            if(enemy4.bigCol+enemy4.width > 500)
            {
                enemy4.cdel = -enemy4.cdel;
                enemy4.bigCol = enemy4.bigCol + enemy4.cdel;    
            }
            
        }
    
    }
    
}
 
void animate() 
{
    //for player
    shadowOAM[0].attr0 = ((ROWMASK)&player.row) | ATTR0_4BPP | player.shape;
    shadowOAM[0].attr1 = ((COLMASK)&player.col) | player.size;
    shadowOAM[0].attr2 = (player.offset) | ATTR2_PALETTE_BANK(0);
    
    //for purple bad guy
    if(enemy.row > 0 && enemy.row < 160
          && enemy.col < 240 && enemy.col > 0
            && (level ==1 || level ==2 || level ==3 || level ==4) )
    {
    shadowOAM[1].attr0 = (ROWMASK&enemy.row) | ATTR0_4BPP | enemy.shape;
    shadowOAM[1].attr1 = (COLMASK&enemy.col) | enemy.size;
    shadowOAM[1].attr2 = (enemy.offset) | ATTR2_PALETTE_BANK(0);
    }
    else
        shadowOAM[1].attr0 = ATTR0_HIDE;
    
        //for green bad guy
    if(enemy2.row > 0 && enemy2.row < 160
          && enemy2.col < 240 && enemy2.col > 0
            && (level ==2 || level ==3 || level ==4))
    {
    shadowOAM[18].attr0 = (ROWMASK&enemy2.row) | ATTR0_4BPP | enemy2.shape;
    shadowOAM[18].attr1 = (COLMASK&enemy2.col) | enemy2.size;
    shadowOAM[18].attr2 = (enemy2.offset) | ATTR2_PALETTE_BANK(0);
    }
    else
        shadowOAM[18].attr0 = ATTR0_HIDE;
    
        //for orange bad guy
    if(enemy3.row > 0 && enemy3.row < 160
          && enemy3.col < 240 && enemy3.col > 0
            && (level ==3 || level ==4))
    {
    shadowOAM[19].attr0 = (ROWMASK&enemy3.row) | ATTR0_4BPP | enemy3.shape;
    shadowOAM[19].attr1 = (COLMASK&enemy3.col) | enemy3.size;
    shadowOAM[19].attr2 = (enemy3.offset) | ATTR2_PALETTE_BANK(0);
    }
    else
        shadowOAM[19].attr0 = ATTR0_HIDE;
    
        //for fire fishie
    if(enemy4.row > 0 && enemy4.row < 160
          && enemy4.col < 240 && enemy4.col > 0
            && (level ==4))
    {
    shadowOAM[20].attr0 = (ROWMASK&enemy4.row) | ATTR0_4BPP | enemy4.shape;
    shadowOAM[20].attr1 = (COLMASK&enemy4.col) | enemy4.size;
    shadowOAM[20].attr2 = (enemy4.offset) | ATTR2_PALETTE_BANK(0);
    }
    else
        shadowOAM[20].attr0 = ATTR0_HIDE;

    //for level
    shadowOAM[2].attr0 = (levelNum.row) | ATTR0_4BPP | ATTR0_TALL;
    shadowOAM[2].attr1 = (levelNum.col) | ATTR1_SIZE32;
    shadowOAM[2].attr2 = (levelNum.offset) | ATTR2_PALETTE_BANK(0);
    
    if(target.row > 0 && target.row < 160
            && target.col < 240 && target.col > 0)
    {
        shadowOAM[3].attr0 = (target.row) | ATTR0_4BPP | target.shape;
        shadowOAM[3].attr1 = (target.col) | target.size;
        shadowOAM[3].attr2 = (target.offset) | ATTR2_PALETTE_BANK(0);
    }
    else
        shadowOAM[3].attr0 = ATTR0_HIDE;

    REG_BG1HOFS = hOff;
    REG_BG1VOFS = vOff; 
}

void pauseWords() 
{
    shadowOAM[4].attr0 = ((ROWMASK)&30) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[4].attr1 = ((COLMASK)&80) | ATTR1_SIZE64;
    shadowOAM[4].attr2 = (SPRITEOFFSET(0,0)) | ATTR2_PALETTE_BANK(0);
            
    //press 
    shadowOAM[5].attr0 = ((ROWMASK)&60) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[5].attr1 = ((COLMASK)&50) | ATTR1_SIZE32;
    shadowOAM[5].attr2 = (SPRITEOFFSET(4,0)) | ATTR2_PALETTE_BANK(0);
    //select
    shadowOAM[6].attr0 = ((ROWMASK)&52) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[6].attr1 = ((COLMASK)&75) | ATTR1_SIZE64;
    shadowOAM[6].attr2 = (SPRITEOFFSET(0,8)) | ATTR2_PALETTE_BANK(0);
    //to
    shadowOAM[7].attr0 = ((ROWMASK)&60) | ATTR0_4BPP | ATTR0_SQUARE;
    shadowOAM[7].attr1 = ((COLMASK)&132) | ATTR1_SIZE16;
    shadowOAM[7].attr2 = (SPRITEOFFSET(4,8)) | ATTR2_PALETTE_BANK(0);            
    //return
    shadowOAM[8].attr0 = ((ROWMASK)&52) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[8].attr1 = ((COLMASK)&140) | ATTR1_SIZE64;
    shadowOAM[8].attr2 = (SPRITEOFFSET(0,15)) | ATTR2_PALETTE_BANK(0);            
    //press
    shadowOAM[9].attr0 = ((ROWMASK)&75) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[9].attr1 = ((COLMASK)&50) | ATTR1_SIZE32;
    shadowOAM[9].attr2 = (SPRITEOFFSET(4,0)) | ATTR2_PALETTE_BANK(0); 
    //start
    shadowOAM[10].attr0 = ((ROWMASK)&66) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[10].attr1 = ((COLMASK)&73) | ATTR1_SIZE64;
    shadowOAM[10].attr2 = (SPRITEOFFSET(0,22)) | ATTR2_PALETTE_BANK(0);       
    //to
    shadowOAM[11].attr0 = ((ROWMASK)&75) | ATTR0_4BPP | ATTR0_SQUARE;
    shadowOAM[11].attr1 = ((COLMASK)&127) | ATTR1_SIZE16;
    shadowOAM[11].attr2 = (SPRITEOFFSET(4,8)) | ATTR2_PALETTE_BANK(0);             
    //restart
    shadowOAM[12].attr0 = ((ROWMASK)&67) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[12].attr1 = ((COLMASK)&140) | ATTR1_SIZE64;
    shadowOAM[12].attr2 = (SPRITEOFFSET(3,10)) | ATTR2_PALETTE_BANK(0);             
}

void hideSprites()
{
    int i;
    for(i=0; i<128; i++) 
    {
        shadowOAM[i].attr0 = ATTR0_HIDE; 
    }  
}

void winWords()
{
    //You Win
    shadowOAM[17].attr0 = ((ROWMASK)&40) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[17].attr1 = ((COLMASK)&80) | ATTR1_SIZE64;
    shadowOAM[17].attr2 = (SPRITEOFFSET(11,13)) | ATTR2_PALETTE_BANK(0);


    //Press
    shadowOAM[13].attr0 = ((ROWMASK)&65) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[13].attr1 = ((COLMASK)&50) | ATTR1_SIZE32;
    shadowOAM[13].attr2 = (SPRITEOFFSET(4,0)) | ATTR2_PALETTE_BANK(0);

    //Start
    shadowOAM[14].attr0 = ((ROWMASK)&57) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[14].attr1 = ((COLMASK)&75) | ATTR1_SIZE64;
    shadowOAM[14].attr2 = (SPRITEOFFSET(0,22)) | ATTR2_PALETTE_BANK(0);

    //to
    shadowOAM[15].attr0 = ((ROWMASK)&65) | ATTR0_4BPP | ATTR0_SQUARE;
    shadowOAM[15].attr1 = ((COLMASK)&132) | ATTR1_SIZE16;
    shadowOAM[15].attr2 = (SPRITEOFFSET(4,8)) | ATTR2_PALETTE_BANK(0); 

    //return
    shadowOAM[16].attr0 = ((ROWMASK)&57) | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[16].attr1 = ((COLMASK)&140) | ATTR1_SIZE64;
    shadowOAM[16].attr2 = (SPRITEOFFSET(0,15)) | ATTR2_PALETTE_BANK(0);
              
}