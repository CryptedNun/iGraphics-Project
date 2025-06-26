#include "iGraphics.h"
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WORLD_WIDTH 4000
#define WORLD_HEIGHT 3000

int gameState = 0;
double cameraX = 0, cameraY = 0;

double ballx = 90, bally = 2270, ball_radius = 20;
double animate = 0;
int l = 100;
double velocity_x = .30;
double velocity_y = 1.5;

double gravity = .5;
bool right = 0, left = 0, jump = 0, available_jump = false;
int i = 0;
bool upper_side_collision = false, lower_side_collision = false, left_side_collision = false, right_side_collision = false, collision = false;

double smallHoop_width = 10, smallHoop_height = 35;
double largeHoop_width = 20, largeHoop_height = 50;

// * Buttons for action: W or Spacebar for jumping, MOUSE_LEFT & MOUSE_RIGHT for movement

//! Initialises Image structs for menuButtons
Image playButton, leaderboardButton, creditsButton, instructionsButton, exitButton;

// ! Initialises a struct called "platform"
struct platform {                               
    double x;
    double y;
    double width;
    double height;
};

// ! Initialises a struct called "trap"
struct trap {
    double x1; // Left(x)/Lower(y) Extreme Coordinate
    double y1;
    double x2; // Right(x)/Upper(y) Extreme Coordinate
    double y2;
    double x;  // Starting & Updating Coordinate
    double y;
    double dx; // Speed
    double dy;
};

// ! Initialises a struct called "hoop" (elliptical level markers)
// ? Small hoop: a=10, b=35.
// ? Large hoop: a=20, b=50.
struct hoop {
    double x;
    double y;
    double width;
    double height;
    bool didBallPass;  //? Did the ball pass through hoop? If yes, grey color. Otherwise yellow.
};

// TODO: Implement, utilise and draw "Spikes"
// ! Initialises a struct called "spike" 
struct spike {
    double x;
    double y;
};

hoop hoops[] = {
    {260,  2090, 10, 35, false},
    {1220, 2240, 10, 35, false},
    {2680, 2090, 10, 35, false},
    {3010, 2090, 10, 35, false},
    {3560, 2210, 35, 10, false},
    {3700, 2210, 35, 10, false}
};

// ! First level platform layout
platform platforms[] = {
    {0,    2010, 4000, 40},
    {0,    2360, 4000, 40},
    {0,    2050, 40,   310},
    {3960, 2050, 40,   310},
    {200,  2160, 120,  200},
    {450,  2050, 60,   160},
    {620,  2180, 150,  180},
    {910,  2050, 50,   110},
    {910,  2160, 170,  40},
    {1160, 2160, 120,  40},
    {1360, 2160, 170,  40},
    {1480, 2050, 50,   110},
    {1160, 2310, 120,  50},
    {1640, 2180, 120,  180},
    {2240, 2180, 120,  180},
    {2480, 2050, 60,   160},
    {2650, 2160, 60,   200},
    {2820, 2050, 60,   160},
    {2980, 2160, 60,   200},
    {3150, 2050, 60,   160},
    {3400, 2050, 120,  150},
    {3600, 2160, 60,   200},
    {3730, 2050, 120,  150}
};

trap traps[] = {
    {800, 80, 1000, 80, 900, 80, 0.03, 0.03}
};

int number_of_platforms = 23;
int number_of_hoops = 6;
int number_of_traps = 1;

void updateCamera() {                           // ! Sliding Window Camera
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

void drawCloud(int x, int y) {                  // ! Draws Cloud(s)
    iSetColor(255, 255, 255); // White cloud

    iFilledCircle(x, y, 25);
    iFilledCircle(x - 20, y - 5, 22);
    iFilledCircle(x + 20, y - 5, 22);
    iFilledCircle(x - 10, y + 10, 20);
    iFilledCircle(x + 10, y + 10, 20);

    // Optional: base flattening
    iFilledEllipse(x, y - 10, 30, 10);
}

void drawTrap() {           // ! all square-shaped trap of length l = 100, moving.
    for(int i=0; i < number_of_traps; i++) {
        iSetLineWidth(5);

        double tx = traps[i].x;
        double ty = traps[i].y;

        iLine(tx + l/2 - cameraX, ty - cameraY, tx - l/2 - cameraX, ty - cameraY);
        iLine(tx - cameraX, ty + l/2 - cameraY, tx - cameraX, ty - l/2 - cameraY);
        iLine(tx + l/2 - cameraX, ty + l/2 - cameraY, tx - l/2 - cameraX, ty - l/2 - cameraY);
        iLine(tx + l/2 - cameraX, ty - l/2 - cameraY, tx - l/2 - cameraX, ty + l/2 - cameraY);

        traps[i].x += traps[i].dx;
        traps[i].y += traps[i].dy;

        if(traps[i].x < traps[i].x1 || traps[i].x > traps[i].x2) {
            if(traps[i].x < traps[i].x1) traps[i].x = traps[i].x1;
            else traps[i].x = traps[i].x2;

            traps[i].dx *= -1;
        }

        if(traps[i].y < traps[i].y1 || traps[i].y > traps[i].y2) {
            if(traps[i].y < traps[i].y1) traps[i].y = traps[i].y1;
            else traps[i].y = traps[i].y2;

            traps[i].dy *= -1;
        }
    }
}

void drawPlatform() {
    for (int i = 0; i < number_of_platforms; i++) {
        iFilledRectangle(platforms[i].x - cameraX, platforms[i].y - cameraY, platforms[i].width, platforms[i].height);
    }
}

void drawHoop() {
    for(int i=0; i < number_of_hoops; i++) {
        if(hoops[i].didBallPass) iSetColor(128, 128, 128);
        else iSetColor(245, 165, 5);

        iEllipse(hoops[i].x - cameraX, hoops[i].y - cameraY, hoops[i].width, hoops[i].height);
    }
}

/* void gameStateRender() {
    switch(gameState) {
        case 0:
            Render the main menu's buttons;
        break;

        case 1:
            Render the Level Selection Menu after "Play"
        break;

        case 2:
            Render the Leaderboards from save file.
        break;

        case 3:
            Render the Contributions/Credits
        break;

        case 4:
            Render the Instructions page
        break;

        case 5:
            Exit
        break;
    }
}
*/

void animated() {
    animate += 1.0;
    if (animate >= SCREEN_WIDTH) {
        animate = 0;
    }
}

void iDraw() {
    iClear();
    // TODO: Create gameStates, if(gameState <= 5) call gameStateRender to render menus, else run game.
    // TODO: 0 -> mainmenu, 1 -> Play, 2 -> leaderboard, 3 -> Contributions, 4 -> Instructions, 5 -> Exit.

    // 1. Sky background
    iSetColor(135, 206, 235); // Sky blue
    iFilledRectangle(0, 0, 800, 600);
    iSetColor(29, 133, 79);

    // 2. Draw multiple clouds
    drawCloud(100 - animate, 500);
    drawCloud(250 - animate, 550);
    drawCloud(400 - animate, 520);
    drawCloud(600 - animate, 530);

    iSetColor(100, 50, 0);
    drawPlatform();

    iSetColor(3, 5, 135);
    drawTrap();

    drawHoop();

    iShowLoadedImage(100, 100, &playButton, 300, 100);

    iSetColor(255,0,0);
    iFilledCircle(ballx - cameraX, bally - cameraY, ball_radius);      // * Draws the ball
}

void iMouseMove(int mx, int my) {
}

void iMouseDrag(int mx, int my) {
}

void iMouse(int button, int state, int mx, int my) {
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
        left=true;
    }
    if(button==GLUT_LEFT_BUTTON && state==GLUT_UP) {
        left=false;
    }
    if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
        right=true;
    }
    if(button==GLUT_RIGHT_BUTTON && state==GLUT_UP) {
        right=false;
    }
}

void iMouseWheel(int dir, int mx, int my) {
}

void iKeyboard(unsigned char key) {
    if ((key == 'w'||key==' ') && available_jump) {
        jump = true;
    }
}

void ballmove() {
    bally -= gravity;
    if (right) ballx += velocity_x;
    if (left)  ballx -= velocity_x;

    available_jump = false;
    for (int i = 0; i < number_of_platforms; i++) {
        double platTop = platforms[i].y + platforms[i].height;
        double platBottom = platforms[i].y;
        double platLeft = platforms[i].x;
        double platRight = platforms[i].x + platforms[i].width;
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;

        // Check for collision 
        if (ballRight > platLeft && ballLeft < platRight && ballTop > platBottom && ballBottom < platTop) {
            collision = true;

            // Check if ball is landing on top of the platform
            if (ballBottom <= platTop && ballBottom >= platBottom && ballx > platLeft && ballx < platRight) {
                bally = platTop + ball_radius;
                available_jump = true;
            } else {
                available_jump = false;
            }

            // Hitting the bottom of the platform
            if (ballTop > platBottom && bally < platBottom) {
                bally = platBottom - ball_radius;
            }

            // Hitting the left side of the platform
            if (ballRight > platLeft && ballx < platLeft) {
                ballx = platLeft - ball_radius;
            }

            // Hitting the right side of the platform
            if (ballLeft < platRight && ballx > platRight) {
                ballx = platRight + ball_radius;
            }
        }
    }

    for (int i = 0; i < number_of_traps; i++) {
        double trapLeft = traps[i].x - l/2;
        double trapRight = traps[i].x + l/2;
        double trapTop = traps[i].y + l/2;
        double trapBottom = traps[i].y - l/2;
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;
        
        // Detect if the ball collides with any of the traps
        if(ballLeft < trapRight && ballRight > trapLeft && ballTop > trapBottom && ballBottom < trapTop) {
            iDelay(1);
            ballx = 100;
            bally = 150;
        }
    }

    if (jump) {
        bally += velocity_y;
        i++;
        if (i == 420) {
            jump = false;
            i = 0;
        }
    }

    // TODO: Implement Hoop Pass and Spike Collision
    //? Used a margin = 3 to make the hoop pass detection more fluid.
    for(int i=0; i<number_of_hoops; i++) {
        if(ballx >= hoops[i].x - hoops[i].width/2 - 3 && ballx <= hoops[i].x + hoops[i].width/2 + 3) {
            if(bally >= hoops[i].y - hoops[i].height/2 - 3 && bally <= hoops[i].y + hoops[i].height/2 + 3) {
                hoops[i].didBallPass = true;
            }
        }
    }

    updateCamera();
}

void iSpecialKeyboard(unsigned char key) {
    
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);

    if(iLoadImage(&playButton, "C:/Users/HASAN/Projects/Codes/CSE102 iGraphics/Modern-iGraphics-main/assets/images/menu_button_play@2x.png"))
        printf("Success!\n");
    else
        printf("Failed!\n");

    iSetTimer(10, animated);
    iSetTimer(1, ballmove);
    iInitialize(800, 600, "Bounce Classic Lite");
    return 0;
}