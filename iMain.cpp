#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include "iSound.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WORLD_WIDTH 6000
#define WORLD_HEIGHT 1800
// #define WORLD_WIDTH 4000
// #define WORLD_HEIGHT 600
#define V_Y_INITIAL 6.5
#define THORN_WIDTH 20
#define THORN_HEIGHT 40

// V_Y_INITIAL 6.5
// WORLD_WIDTH 6000
// WORLD_HEIGHT 1800
// Variables for gravitational influence
double v_y_i = 0;
double del_y = 0;
double gravity = 0.075;
double dt = 0.275;



int gameState = 0; //has huge influence on overall game and game designing



Image playButton, leaderboardButton, creditsButton, instructionsButton, exitButton, pauseButton, menuBackground, logo, ball, dyn_spike, spike, pop_ball, level_interface, checkpoint, checkpoint_catched, life, pauseMenu, passMenu, failMenu,credit_scene, instruction_manual;

//! Variable for ball lives.
int available_lives = 2;
int available_hoops = 0;

double cameraX = 0, cameraY = 0;

double spawnX = 200, spawnY = 150;
double ballx = 200, bally = 150, ball_radius = 20;
double degree = 0;
double animate = 0;
int l = 100;
double velocity_x = .4;
double velocity_y = 1.5;
bool right = 0, left = 0, jump = 0, available_jump = false;
int i = 0;
bool upper_side_collision = false, lower_side_collision = false, left_side_collision = false, right_side_collision = false, collision = false;
// now we dont have work with all those bool functions but they are declared for further collision detection and control . we may need to use them later for controling balls

bool didBallPop = false;
double smallHoop_width = 10, smallHoop_height = 35;
double largeHoop_width = 20, largeHoop_height = 50;

// * Buttons for action: W or Spacebar for jumping, MOUSE_LEFT & MOUSE_RIGHT for movement

// ! Initialises a struct called "platform"riables for level transverrsal 
int platform_starting_index=0;
int platform_ending_index=0;
#define level_one_starting_index 0
#define level_one_ending_index 22
#define level_two_starting_index 23;
#define level_two_ending_index 44
#define level_three_starting_index 45
#define level_three_ending_index 0//level three is still incomplete

int hoop_starting_index=0;
int hoop_ending_index=0;
#define level_one_hoop_starting_index 0
#define level_one_hoop_ending_index 5
#define level_two_hoop_starting_index 6
#define level_two_hoop_ending_index 13
#define level_three_hoop_starting_index 14
#define level_three_hoop_ending_index 14

int trap_starting_index=0;
int trap_ending_index=0;
#define level_one__trap_starting_index 0
#define level_one_trap_ending_index 0
#define level_two_trap_starting_index 1
#define level_two_trap_ending_index 4
#define level_three_trap_starting_index 14
#define level_three_trap_ending_index 14

int thorn_starting_index=0;
int thorn_ending_index=0;
#define level_one_thorn_starting_index 0
#define level_one_thorn_ending_index 1
#define level_two_thorn_starting_index 4
#define level_two_thorn_ending_index 6

int live_starting_index = 0;
int live_ending_index = 0;
#define level_one_live_starting_index 0
#define level_one_live_ending_index 0
#define level_two_live_starting_index 1
#define level_two_live_ending_index 2

int checkpoint_starting_index = 0;
int checkpoint_ending_index = 0;
#define level_one_checkpoint_starting_index 0
#define level_one_checkpoint_ending_index 1
#define level_two_checkpoint_starting_index 2
#define level_two_checkpoint_ending_index 4
//now i am going to declare some variable to stop game mechanics while in game menu . These can essentially stop the iSetTimer function and stop the game

//sound related variables 
int channel_1=0;
int channel_2=0;
int channel_3=0;




int cloud_annimation_index=0;
int ballmove_timer_index=0;
int movement_animation_index=0;
int ball_pop_animation_index=0;
//va
struct platform
{
    double x;
    double y;
    double width;
    double height;
};

struct live {
    double x;
    double y;
    bool didBallPass;
};

struct check_point {
    double x;
    double y;
    bool didBallPass;
};

// ! Initialises a struct called "trap"
struct trap
{
    double x1; // Left(x)/Lower(y) Extreme Coordinate
    double y1;
    double x2; // Right(x)/Upper(y) Extreme Coordinate
    double y2;
    double x; // Starting & Updating Coordinate
    double y;
    double dx; // Speed
    double dy;
};

// ! Initialises a struct called "hoop" (elliptical level markers)
// ? Small hoop: a=10, b=35.
// ? Large hoop: a=20, b=50.
struct hoop
{
    double x;
    double y;
    double width;
    double height;
    bool didBallPass; //? Did the ball pass through hoop? If yes, grey color. Otherwise yellow.
};

// TODO: Implement, utilise and draw "Spikes"
// ! Initialises a struct called "spike"
struct thorn
{
    double x;
    double y;
};

live lives[] = {
    {2830, 260, false},

    {950, 190, false},
    {4750, 410, false}
};

check_point checkpoints[] = {
    {2000, 360, false},
    {3600, 110, false},

    {3600, 260, false},
    {4500, 860, false}, 
    {4500, 1600, false}
};

platform platforms[] = {
    {0.0, 0.0, 4000.0, 40.0},
    {0.0, 560.0, 4000.0, 40.0},
    {0.0, 40.0, 40.0, 520.0},
    {3960.0, 40.0, 40.0, 520.0},
    {200.0, 150.0, 120.0, 410.0},
    {450.0, 40.0, 60.0, 160.0},
    {620.0, 170.0, 150.0, 390.0},
    {910.0, 40.0, 50.0, 110.0},
    {910.0, 150.0, 170.0, 40.0},        
    {1160.0, 150.0, 120.0, 40.0},
    {1360.0, 150.0, 170.0, 40.0},       
    {1480.0, 40.0, 50.0, 110.0},
    {1160.0, 400.0, 120.0, 50.0},
    {1640.0, 170.0, 120.0, 390.0},
    {2240.0, 170.0, 120.0, 390.0},
    {2480.0, 40.0, 60.0, 160.0},
    {2650.0, 150.0, 60.0, 410.0},
    {2820.0, 40.0, 60.0, 160.0},
    {2980.0, 150.0, 60.0, 410.0},
    {3150.0, 40.0, 60.0, 160.0},
    {3400.0, 40.0, 120.0, 150.0},
    {3600.0, 150.0, 60.0, 410.0},
    {3730.0, 40.0, 120.0, 150.0},//end of  level one at index 22


    {0, 0, 6000, 60},
    {0, 540, 5680, 60},
    {0, 60, 60, 480},
    {4440, 600, 60, 1200},
    {4500, 1740, 1500, 60},
    {5940, 60, 60, 1680},
    {800, 60, 60, 200},
    {800, 260, 350, 60},
    {1300, 240, 120, 300},
    {1600, 60, 100, 200},
    {2100, 60, 150, 200},
    {3250, 180, 120, 360},
    {3850, 60, 120, 150},
    {3850, 340, 120, 200},
    {4400, 60, 100, 190},
    {5050, 60, 100, 190},
    {5750, 260, 190, 60},
    {4840, 1090, 1100, 60},
    {4500, 1350, 1100, 60},
    {4500, 940, 120, 60},
    {4800, 600, 100, 190},
    {5220, 600, 100, 190}
};

hoop hoops[] = {
    {265.0, 80.0, 10, 40, false},
    {1220.0, 230.0, 10, 40, false},
    {2680.0, 80.0, 10, 40, false},
    {3010.0, 80.0, 10, 40, false}, 
    {3560.0, 205.0, 40, 10, false},
    {3700.0, 205.0, 40, 10, false},//level one ending index at 5

    {980, 360, 10, 40, false}, 
    {1640, 300, 10, 40, false},
    {3300, 100, 10, 40, false},
    {3900, 250, 10, 40, false},
    {4440, 300, 10, 40, false},
    {5250, 830, 10, 40, false},
    {5400, 1450, 10, 40, false},
    {2850, 350, 10, 40, false}//level two ending index at 13
};

// trap traps[] = {
//     {1010, 90, 1430, 90, 900, 80, .08, .08}
// };

trap traps[] = {
     {1010, 90, 1430, 90, 900, 80, .08, .08},//level one starting and ending at 0

    {110, 110, 110, 450, 110, 120, .5, .5},//starting of level two at the index of 1
    {2500, 110, 2500, 450, 2500, 120, .5, .5},
    {2850, 110, 2850, 450, 2850, 240, .5, .5},
    {3100, 110, 3100, 450, 3100, 360, .5, .5}//elding of level two hoop at 4
};

thorn thorns[] = {
    {1900, 40},
    {2100, 40},
    {400, 40},
    {3350, 40},//ending if level one throrn  at index 3

    {720, 60},//starting of level two throns at index 4
    {5670, 60},
    {5050, 600}//ending of level two throns at index 6
};

int number_of_platforms = 22;
int number_of_hoops = 8;
int number_of_traps = 4;
// TODO: Initialise the below variable after constructing spike code
int number_of_thorns = 3;
int number_of_lives = 2;

void updateCamera()
{ // ! Sliding Window Camera
    cameraX = ballx - SCREEN_WIDTH / 2;
    cameraY = bally - SCREEN_HEIGHT / 2;

    // Clamp camera within world bounds
    if (cameraX < 0)
        cameraX = 0;
    if (cameraY < 0)
        cameraY = 0;
    if (cameraX > WORLD_WIDTH - SCREEN_WIDTH)
        cameraX = WORLD_WIDTH - SCREEN_WIDTH;
    if (cameraY > WORLD_HEIGHT - SCREEN_HEIGHT)
        cameraY = WORLD_HEIGHT - SCREEN_HEIGHT;
}

void drawCloud(int x, int y)
{                             // ! Draws Cloud(s)
    iSetColor(255, 255, 255); // White cloud

    iFilledCircle(x, y, 25);
    iFilledCircle(x - 20, y - 5, 22);
    iFilledCircle(x + 20, y - 5, 22);
    iFilledCircle(x - 10, y + 10, 20);
    iFilledCircle(x + 10, y + 10, 20);

    // Optional: base flattening
    iFilledEllipse(x, y - 10, 30, 10);
}

void drawTrap()
{ // ! all square-shaped trap of length l = 100, moving.
    for (int i = trap_starting_index; i <=trap_ending_index; i++) {
        double tx = traps[i].x;
        double ty = traps[i].y;

        iShowLoadedImage(tx - l/2 - cameraX, ty - l/2 - cameraY, &dyn_spike);

        traps[i].x += traps[i].dx;
        traps[i].y += traps[i].dy;

        if (traps[i].x < traps[i].x1 || traps[i].x > traps[i].x2)
        {
            if (traps[i].x < traps[i].x1)
                traps[i].x = traps[i].x1;
            else
                traps[i].x = traps[i].x2;

            traps[i].dx *= -1;
        }
        if (traps[i].y < traps[i].y1 || traps[i].y > traps[i].y2)
        {
            if (traps[i].y < traps[i].y1)
                traps[i].y = traps[i].y1;
            else
                traps[i].y = traps[i].y2;

            traps[i].dy *= -1;
        }
    }
}

void drawPlatform() {
    for (int i = platform_starting_index; i <= platform_ending_index; i++) {
        iFilledRectangle(platforms[i].x - cameraX, platforms[i].y - cameraY, platforms[i].width, platforms[i].height);
    }
}

void drawStripesVertical(void) {
    for (int i = platform_starting_index; i <=platform_ending_index; i++) {
        double  checked_x = platforms[i].x, 
                checked_y = platforms[i].y, 
                checked_width = 5, 
                checked_height = 20;

        for (int ny = 0; ny * 20 < platforms[i].height; ny++) {
            if ((ny + 1) * 20 > platforms[i].height) {

                //! This commented code had a bug.
                //! checked_height = platforms[i].height - (ny - 1) * 20;

                checked_height = platforms[i].height - ny * 20;
            } else {
                checked_height = 20;
            }

            for (int nx = 0; nx * 30 < platforms[i].width; nx++) {
                if (ny % 2 == 0) {
                    checked_x = nx * 30 + platforms[i].x;
                    checked_y = ny * 20 + platforms[i].y;

                    iFilledRectangle(checked_x - cameraX, checked_y - cameraY, checked_width, checked_height);
                    iFilledRectangle(platforms[i].x - cameraX, checked_y - 2.5-cameraY, platforms[i].width, 5);

                    if (checked_y != platforms[i].y) {
                        iFilledRectangle(platforms[i].x - cameraX, checked_y - 2.5 - cameraY - 20, platforms[i].width, 5);
                    }

                } else {
                    if (nx * 30 + 15 < platforms[i].width) {
                        checked_x = nx * 30 + platforms[i].x + 15;
                        checked_y = ny * 20 + platforms[i].y;

                        iFilledRectangle(checked_x - cameraX, checked_y - cameraY, checked_width, checked_height);
                        iFilledRectangle(platforms[i].x - cameraX, checked_y - 2.5 - cameraY, platforms[i].width, 5);

                        if (checked_y != platforms[i].y) {
                            iFilledRectangle(platforms[i].x - cameraX, checked_y - 2.5 - cameraY - 20, platforms[i].width, 5);
                        } else {
                            //TODO: empty else, could be removed or commented if unused
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

void drawLive() {
    for(int i=live_starting_index; i<=live_ending_index; i++) {
        if(!lives[i].didBallPass)
            iShowLoadedImage(lives[i].x - cameraX, lives[i].y - cameraY, &life);
    }
}

void drawCheckpoint() {
    for(int i=checkpoint_starting_index; i<=checkpoint_ending_index; i++) {
        if(!checkpoints[i].didBallPass) {
            iShowLoadedImage(checkpoints[i].x - cameraX, checkpoints[i].y - cameraY, &checkpoint);
        } else {
            iShowLoadedImage(checkpoints[i].x - cameraX, checkpoints[i].y - cameraY, &checkpoint_catched);
        }
    }
}

void drawHoop()
{
    for (int i = hoop_starting_index; i <=hoop_ending_index; i++)
    {
        if (hoops[i].didBallPass)
            iSetColor(128, 128, 128);
        else
            iSetColor(245, 165, 5);

        iEllipse(hoops[i].x - cameraX, hoops[i].y - cameraY, hoops[i].width, hoops[i].height, 20);
    }
}

//! spikeWidth = 20, spikeHeight = 40.
void drawSpike() {
    // TODO: Write code for rendering spikes
    for(int i=thorn_starting_index; i <=thorn_ending_index; i++) {
        iShowLoadedImage2(thorns[i].x - cameraX, thorns[i].y - cameraY, &spike, THORN_WIDTH, THORN_HEIGHT);
    }
    
}

// TODO: Create gameStates, if(gameState <= 5) call gameStateRender to render menus, else run game.
// TODO: 0 -> mainmenu, 1 -> Play, 2 -> leaderboard, 3 -> Contributions, 4 -> Instructions, 5 -> Exit, 6 -> level play, 
//! 7 -> PauseMenu, 8 -> FailMenu, 9 -> PassMenu
 void gameStateRender() {
    // iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

    switch(gameState) {
        case 0:
            // Render the main menu's buttons;
            iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

            iShowLoadedImage2(520, 340, &playButton, 240, 60);
            iShowLoadedImage2(520, 270, &leaderboardButton, 240, 60);
            iShowLoadedImage2(520, 200, &creditsButton, 240, 60);
            iShowLoadedImage2(520, 130, &instructionsButton, 240, 60);
            iShowLoadedImage2(520, 60, &exitButton, 240, 60);
            iShowLoadedImage2(460, 325, &logo, 350, 350);

            iPauseTimer(cloud_annimation_index);
            iPauseTimer(ball_pop_animation_index);
            iPauseTimer(ballmove_timer_index);
            iPauseTimer(movement_animation_index);
        break;

        case 1:
            //Render the Level Selection Menu after "Play", also progress saved in save file.
            iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

            iShowLoadedImage2(0,0, &level_interface, 800, 600);
            iTextBold(547, 50, "LEVEL 1",GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(139, 118, "LEVEL 2",GLUT_BITMAP_TIMES_ROMAN_24);
        break;

        case 2:
        iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

            //Render the Leaderboards from save file.
        break;

        case 3:
            iShowLoadedImage2(0,0,&credit_scene,800,600);
            iShowLoadedImage2(520, 60, &exitButton, 240, 60);
        break;

        case 4:
        iShowLoadedImage2(0, 0, &instruction_manual, 800, 600);
        iShowLoadedImage2(520, 60, &exitButton, 240, 60);
            //Render the Instructions page
        break;

        case 5:
        iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

            exit(0);
        break;

        //! Level gameState
        case 6:
        iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

            iResumeTimer(cloud_annimation_index);
            iResumeTimer(ball_pop_animation_index);
            iResumeTimer(ballmove_timer_index);
            iResumeTimer(movement_animation_index);
            // 1. Sky background
            iSetColor(135, 206, 235); // Sky blue
            iFilledRectangle(0, 0, 800, 600);
            iSetColor(29, 133, 79);

            // 2. Draw multiple clouds
            drawCloud(100 - animate, 500);
            drawCloud(250 - animate, 550);
            drawCloud(400 - animate, 520);
            drawCloud(600 - animate, 530);

            iSetColor(133,4,4);
            drawPlatform();

            iSetColor(247, 247, 247);
            drawStripesVertical();

            iSetColor(3, 5, 135);
            drawTrap();

            drawHoop();

            drawLive();

            drawCheckpoint();

            drawSpike();

            char str[4];
            sprintf(str, "x%02d", available_lives);
            iShowLoadedImage2(25, 560, &ball, 20, 20);
            iSetColor(0, 0, 0);
            iText(50, 560, str, GLUT_BITMAP_TIMES_ROMAN_24);

            sprintf(str, "x%02d", available_hoops);
            iSetColor(255, 255, 0);
            iEllipse(120, 570, 4, 16);
            iSetColor(0, 0, 0);
            iText(130, 560, str, GLUT_BITMAP_TIMES_ROMAN_24);

            iShowLoadedImage(760, 560, &pauseButton);

            iSetColor(255, 0, 0);

            if(didBallPop) {
                iShowLoadedImage(ballx - cameraX - 20, bally - cameraY - 20, &pop_ball);
            } else {
                iFilledCircle(ballx - cameraX, bally - cameraY, ball_radius);
                iRotate(ballx - cameraX, bally - cameraY, degree);
                iShowLoadedImage(ballx - 20 - cameraX, bally - 20 - cameraY, &ball);
                iUnRotate();
            }
        break;

        case 7:
        iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

            iShowLoadedImage(130, 100, &pauseMenu);
        break;

        case 8:
        iShowLoadedImage2(0, 0, &menuBackground, 800, 600);

            iShowLoadedImage(100, 100, &failMenu);
        break;

        case 9:
        iShowLoadedImage2(0, 0, &menuBackground, 800, 600);


        break;
    }
}

void animated()
{
    animate += 1.0;
    if (animate >= SCREEN_WIDTH)
    {
        animate = 0;
    }

    if(right) degree -= 8.594367/2.2;
    else if(left) degree += 8.594367/2.2;
    if (degree >= 360 || degree <= -360)
    {
        degree = 0;
    }
}

void iDraw() {
    iClear();

    // TODO: Completely Implement gameStateRender() here.

    gameStateRender();
    
    iSetColor(0,0,255);
    iFilledRectangle(8,8,70,14);
    iSetColor(0,255,0);
    iShowSpeed(10,10);
    // iSetTransparentColor(255,255,255,1.0);
    iSetColor(255,255,0);
}

void iMouseMove(int mx, int my)
{
}

void iMouseDrag(int mx, int my)
{
}

void iMouse(int button, int state, int mx, int my)
{
    if(gameState == 0) {
        if(mx >= 520 && mx <= 520+240 && my >= 340 && my <= 340+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            gameState = 1;//for tapping the play button and then it will show the level diagram 
        }
        
        if(mx >= 520 && mx <= 520+240 && my >= 200 && my <= 200+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            gameState = 3;//for showing credit
        }

        if(mx >= 520 && mx <= 520+240 && my >= 60 && my <= 60+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            gameState = 5;//for exiting the game completely
        }
        if(mx >= 520 && mx <= 520+240 && my >= 130 && my <= 130+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            gameState = 4;
            //for instruction designing
        }
    }
if(gameState == 3 || gameState == 4) {
        if(mx >= 520 && mx <= 520+240 && my >= 60 && my <= 60+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                gameState = 0;//if it is in credit menu and it wants to exit then instead of pausing the game it goes back to previous menu
            }
    } 
if(gameState==1){
    if((mx>=500&&mx<=700)&&(my>=0&&my<=250)){
        //level one

        gameState=6;
        platform_starting_index=level_one_starting_index;
        platform_ending_index=level_one_ending_index;
        hoop_starting_index=level_one_hoop_starting_index;
        hoop_ending_index=level_one_hoop_ending_index;
        trap_starting_index=level_one__trap_starting_index;
        trap_ending_index=level_one_trap_ending_index;
        thorn_starting_index=level_one_thorn_starting_index;
        thorn_ending_index=level_one_thorn_ending_index;
        live_starting_index=level_one_live_starting_index;
        live_ending_index=level_one_live_ending_index;
        checkpoint_starting_index = level_one_checkpoint_starting_index;
        checkpoint_ending_index = level_one_checkpoint_ending_index;

        available_hoops = hoop_ending_index - hoop_starting_index + 1;
    }
    if((mx>=90&&mx<=300)&&(my>=70&&my<=290)){
        //level two

        gameState=6;
        platform_starting_index=level_two_starting_index;
        platform_ending_index=level_two_ending_index;
        hoop_starting_index=level_two_hoop_starting_index;
        hoop_ending_index=level_two_hoop_ending_index;
        trap_starting_index=level_two_trap_starting_index;
        trap_ending_index=level_two_trap_ending_index;
        thorn_starting_index=level_two_thorn_starting_index;
        thorn_ending_index=level_two_thorn_ending_index;
        live_starting_index=level_two_live_starting_index;
        live_ending_index=level_two_live_ending_index;
        checkpoint_starting_index = level_two_checkpoint_starting_index;
        checkpoint_ending_index = level_two_checkpoint_ending_index;

        available_hoops = hoop_ending_index - hoop_starting_index + 1;
    }
    if(1){
        //level three
    }
}

if(gameState == 6) {
    if(mx >= 760 && mx <= 800 && my >= 560 && my <= 600) {
        gameState = 7;
    } 
}

if(gameState == 7) {
    if(mx >= 365 && mx <= 625 && my >= 325 && my <= 440) {
        gameState = 6;
    } else if(mx >= 200 && mx <= 315 && my >= 150 && my <= 270) {
        available_lives = 2;
        available_hoops = 0;
        spawnX = 200, spawnY = 150, ballx = 200, bally = 150;
        
        //? Reset The Data of the level
        for(int i=hoop_starting_index; i<=hoop_ending_index; i++) {
            hoops[i].didBallPass = false;
        }

        for(int i=checkpoint_starting_index; i<=checkpoint_ending_index; i++) {
            checkpoints[i].didBallPass = false;
        }

        for(int i=live_starting_index; i<=live_ending_index; i++) {
            lives[i].didBallPass = false;
        }

        gameState = 0;
    } else if(mx >= 200 && mx <= 315 && my >= 330 && my <= 440) {
        available_lives = 2;
        available_hoops = hoop_ending_index - hoop_starting_index + 1;
        spawnX = 200, spawnY = 150, ballx = 200, bally = 150;
        
        //? Reset The Data of the level
        for(int i=hoop_starting_index; i<=hoop_ending_index; i++) {
            hoops[i].didBallPass = false;
        }

        for(int i=checkpoint_starting_index; i<=checkpoint_ending_index; i++) {
            checkpoints[i].didBallPass = false;
        }

        for(int i=live_starting_index; i<=live_ending_index; i++) {
            lives[i].didBallPass = false;
        }

        gameState = 6;
    }
}

//! Fail Menu gameState
if(gameState == 8) {
    if(mx >= 370 && mx <= 630 && my >= 150 && my <= 270) {
        available_lives = 2;
        available_hoops = hoop_ending_index - hoop_starting_index + 1;
        spawnX = 200, spawnY = 150, ballx = 200, bally = 150;
        
        //? Reset The Data of the level
        for(int i=hoop_starting_index; i<=hoop_ending_index; i++) {
            hoops[i].didBallPass = false;
        }

        for(int i=checkpoint_starting_index; i<=checkpoint_ending_index; i++) {
            checkpoints[i].didBallPass = false;
        }

        for(int i=live_starting_index; i<=live_ending_index; i++) {
            lives[i].didBallPass = false;
        }

        gameState = 6;
    } else if(mx >= 200 && mx <= 310 && my >= 150 && my <= 270 && state == GLUT_UP) {
        available_lives = 2;
        available_hoops = 0;
        spawnX = 200, spawnY = 150, ballx = 200, bally = 150;
        
        //? Reset The Data of the level
        for(int i=hoop_starting_index; i<=hoop_ending_index; i++) {
            hoops[i].didBallPass = false;
        }

        for(int i=checkpoint_starting_index; i<=checkpoint_ending_index; i++) {
            checkpoints[i].didBallPass = false;
        }

        for(int i=live_starting_index; i<=live_ending_index; i++) {
            lives[i].didBallPass = false;
        }
        gameState = 1;
    }
}
if(state==GLUT_DOWN){
    printf("x position %d y position %d \n",mx,my); // hey Hasan dont be confused I used it to determine the buttons location to track the gamestate and add comments
    }
}

void iMouseWheel(int dir, int mx, int my)
{
}

void iKeyboard(unsigned char key, int state) {
    if ((key == 'w' || key == ' '||key=='W') && available_jump) {
        jump = true;
        v_y_i = V_Y_INITIAL;
        
    }
    
}

void ballmove()
{
    if(gameState != 6) return;
    del_y = v_y_i*dt - .5*gravity*dt*dt;
    if(del_y <= -1.5) del_y = -1.5;          //* Terminal velocity
    bally += del_y;
    v_y_i -= gravity*dt;

    // bally -= gravity;
    if (right) {
        ballx += velocity_x;
    }
    if (left) {
        ballx -= velocity_x;
    }

    available_jump = false;
    for (int i = platform_starting_index; i <=platform_ending_index; i++)
    {
        double platTop = platforms[i].y + platforms[i].height;
        double platBottom = platforms[i].y;
        double platLeft = platforms[i].x;
        double platRight = platforms[i].x + platforms[i].width;
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;

        // Check for collision
        if (ballRight > platLeft && ballLeft < platRight && ballTop > platBottom && ballBottom < platTop)
        {
            collision = true;

            // Check if ball is landing on top of the platform
            if (ballBottom < platTop && ballBottom > platBottom && ballx > platLeft && ballx < platRight)
            {
                bally = platTop + ball_radius;
                // TODO: Implement immediate jump during rebound, currently that's missing
                v_y_i = 0;
                del_y = 0;
                available_jump = true;
            }
            else
            {
                available_jump = false;
            }

            // Hitting the bottom of the platform
            if (ballTop > platBottom && bally < platBottom)
            {
                bally = platBottom - ball_radius;
                v_y_i = 0; 
                lower_side_collision = true;
            }else{
                lower_side_collision = false;
            } 

            // Hitting the left side of the platform
            if (ballRight > platLeft && ballx < platLeft)
            {
                ballx = platLeft - ball_radius;
            }

            // Hitting the right side of the platform
            if (ballLeft < platRight && ballx > platRight)
            {
                ballx = platRight + ball_radius;
            }
        }
    }

    for (int i = trap_starting_index; i <= trap_ending_index; i++)
    {
        double trapLeft = traps[i].x - l / 2;
        double trapRight = traps[i].x + l / 2;
        double trapTop = traps[i].y + l / 2;
        double trapBottom = traps[i].y - l / 2;
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;

        // iShowLoadedImage2(100, 100, &pop_ball, 40, 40);
        // Detect if the ball collides with any of the traps
        if (ballLeft < trapRight && ballRight > trapLeft && ballTop > trapBottom && ballBottom < trapTop)
        {
            if(!didBallPop) {
            available_lives--;
            didBallPop = true;

            iPlaySound("assets\\sounds\\balloon-pop-48030.mp3", 0, 100);
            }
            break;
        }
}

    

    for (int i = hoop_starting_index; i <=hoop_ending_index; i++)
    {
        if (ballx >= hoops[i].x - hoops[i].width / 2 - 3 && ballx <= hoops[i].x + hoops[i].width / 2 + 3)
        {
            if (bally >= hoops[i].y - hoops[i].height / 2 - 3 && bally <= hoops[i].y + hoops[i].height / 2 + 3)
            {
                if(!hoops[i].didBallPass) {
                    hoops[i].didBallPass = true;
                    available_hoops--;

                    iPlaySound("assets\\sounds\\coin.mp3", 0, 100);
                }
            }
        }
    }

    // TODO: Implement thorn/spike collision
    for(int i=thorn_starting_index; i<=thorn_ending_index; i++) {
        double thornLeft = thorns[i].x;
        double thornRight = thorns[i].x + THORN_WIDTH;
        double thornBottom = thorns[i].y;
        double thornTop = thorns[i].y + THORN_HEIGHT;
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;
        
        if(ballLeft < thornRight && ballRight > thornLeft && ballTop > thornBottom && ballBottom < thornTop) {
            if(!didBallPop){
                available_lives--;
                didBallPop = true;

                iPlaySound("assets\\sounds\\balloon-pop-48030.mp3", 0, 100);
            }
            break;
            // iDelay(2);
            // ballx = spawnX;
            // bally = spawnY;
        }
    }

    //? Implement life_pickup collision detection.
    for(int i=live_starting_index; i<=live_ending_index; i++) {
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;
        double liveLeft = lives[i].x;
        double liveRight = lives[i].x + 40;
        double liveBottom = lives[i].y;
        double liveTop = lives[i].y + 40;

        if(!lives[i].didBallPass && (ballLeft<liveRight && ballRight>liveLeft && ballTop>liveBottom && ballBottom<liveTop)) {
            available_lives++;
            lives[i].didBallPass = true;

            iPlaySound("assets\\sounds\\coin.mp3", 0, 100);
        }
    }
    //? Implement checkpoint collision detection.
    for(int i=checkpoint_starting_index; i<=checkpoint_ending_index; i++) {
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;
        double checkpointLeft = checkpoints[i].x;
        double checkpointRight = checkpoints[i].x + 40;
        double checkpointBottom = checkpoints[i].y;
        double checkpointTop = checkpoints[i].y + 40;

        if(!checkpoints[i].didBallPass && (ballLeft<checkpointRight && ballRight>checkpointLeft && ballTop>checkpointBottom && ballBottom<checkpointTop)) {
            spawnX = checkpoints[i].x + 20;
            spawnY = checkpoints[i].y + 20;
            checkpoints[i].didBallPass = true;

            iPlaySound("assets\\sounds\\coin.mp3", 0, 100);
        }
    }
    updateCamera();
}

void BallPop() {
    if(didBallPop) {
        iDelay(2);
        if(gameState == 6 && available_lives == 0) gameState = 8;
        ballx = spawnX;
        bally = spawnY;
        didBallPop = false;
    }
}


void movement(){
    if(isKeyPressed('a')||isKeyPressed('A')||isSpecialKeyPressed(GLUT_KEY_LEFT)){
         
          ballx-=velocity_x;
          left=true;
    }else{
        left=false;
    }
    if(isKeyPressed('d')||isKeyPressed('D')||isSpecialKeyPressed(GLUT_KEY_RIGHT)){
       ballx += velocity_x;
       right=true;
    }else{
        right=false;
    }
   
}
void iSpecialKeyboard(unsigned char key,int state){
    if ((key == GLUT_KEY_UP) && available_jump) {
        jump = true;
        v_y_i = V_Y_INITIAL;
    }

}

int main(int argc, char *argv[])
{
    // glutInit(&argc, argv);

    if(iLoadImage(&menuBackground, "assets\\images\\menuBackground.png")) 
        printf("Background loaded!\n");
    else 
        printf("Failed to load background!\n");

    if(iLoadImage(&logo, "assets\\images\\bounce_classic_transparent.png")) 
        printf("Logo loaded!\n");
    else
        printf("Failed to load logo!\n");

    if(iLoadImage(&playButton, "assets\\images\\menu_button_play@4x.png"))
        printf("Success loading playButton!\n");
    else
        printf("Failed loading playButton!\n");

    if(iLoadImage(&leaderboardButton, "assets/images/menu_button_leaderboard@2x.png"))
        printf("Success loading leaderboardButton!\n");
    else 
        printf("Failed loading leaderboardButton!\n");

    if(iLoadImage(&creditsButton, "assets\\images\\Credits.jpg"))
        printf("Loaded creditsButton!\n");
    else
        printf("Failed loading creditsButton!\n");

    if(iLoadImage(&instructionsButton, "assets\\images\\instructions.jpg")) 
        printf("Loaded instructionsButton!\n");
    else 
        printf("Failed to load instructionsButton!\n");
    
    if(iLoadImage(&exitButton, "assets\\images\\exitButton.jpg")) 
        printf("Loaded instructionsButton!\n");
    else 
        printf("Failed to load exitButton!\n");
    
    if(iLoadImage(&ball, "assets\\images\\ball_small@2x.png"))
    {
        iResizeImage(&ball, 40, 40);
         printf("Successfully loaded ballImage!\n");
    }
       
    else 
        printf("Failed loading ballImage!\n");

    if(iLoadImage(&dyn_spike, "assets/images/dyn_thorn@2x.png"))
    {
        iResizeImage(&dyn_spike, 100, 100);
        printf("Successfully loaded trap!\n");
    }
        
    else 
        printf("Failed loading trap!\n");

    if(iLoadImage(&spike, "assets/images/thorn@2x.png"))
        printf("Loaded thorns!\n");
    else 
        printf("Couldn't load thorns.\n");

    if(iLoadImage(&pop_ball, "assets\\images\\ball_pop@2x.png"))
    {
        printf("Loaded ballPop!\n");
        iResizeImage(&pop_ball, 40, 40);
    }
    else {
        printf("Couldn't load ballPop.\n");
    }
    if(iLoadImage(&level_interface,"assets\\images\\level_interface.png")){
        printf("Loaded Level interface .\n");
        iResizeImage(&level_interface, SCREEN_WIDTH, SCREEN_HEIGHT);
    }else{
    printf("Failed to load level interface .\n");
    }

    if(iLoadImage(&life,"assets\\images\\life@2x.png")){
        printf("Loaded life icon.\n");
        iResizeImage(&life, 40, 40);
    } else {
        printf("Failed to load life icon.\n");
    }

    if(iLoadImage(&checkpoint, "assets\\images\\checkpoint@2x.png")){
        printf("Loaded checkpoint icon.\n");
        iResizeImage(&checkpoint, 40, 40);
    } else {
        printf("Failed to load checkpoint icon.\n");
    }

    if(iLoadImage(&checkpoint_catched, "assets\\images\\checkpoint_catched@2x.png")){
        printf("Loaded checkpoint_catched icon.\n");
        iResizeImage(&checkpoint_catched, 40, 40);
    } else {
        printf("Failed to load checkpoint_catched icon.\n");
    }

    if(iLoadImage(&pauseMenu, "assets\\images\\pauseMenu.png")) {
        printf("Loaded pauseMenu!\n");
        iResizeImage(&pauseMenu, 550, 460);
    } else {
        printf("Couldn't load pauseMenu.\n");
    }

    if(iLoadImage(&pauseButton, "assets\\images\\pauseButton.png")) {
        printf("Loaded pauseButton\n");
        iResizeImage(&pauseButton, 40, 40);
    } else {
        printf("Couldn't load pauseButton.\n");
    }

    if(iLoadImage(&failMenu, "assets\\images\\failMenu.png")) {
        printf("Fail Menu loaded\n");
        iResizeImage(&failMenu, 610, 470);
    } else {
        printf("FailMenu not loaded.\n");
    }
    if(iLoadImage(&credit_scene,"assets\\images\\Credit Screne.png")){
        printf("Loaded Credit Scene .\n");
        iResizeImage(&credit_scene,800,600);
    }else{
        printf("Failed To Load Credit .\n");
    }

    if(iLoadImage(&instruction_manual,"assets\\images\\instruction_manual.png")){
        printf("Loaded instruction manual .\n\n");
        iResizeImage(&instruction_manual,800,600);
    }else{
        printf("Failed to load instructionn manual .\n");
    }



    channel_1=iPlaySound("assets\\sounds\\menu_background.mp3",true,100);

    cloud_annimation_index= iSetTimer(10, animated);
    ballmove_timer_index= iSetTimer(1, ballmove);
    ball_pop_animation_index= iSetTimer(15, BallPop);
    movement_animation_index= iSetTimer(1, movement);
    iOpenWindow(800, 600, "Bounce Classic Lite");
    return 0;
}