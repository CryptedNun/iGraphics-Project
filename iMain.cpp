#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WORLD_WIDTH 4000
#define WORLD_HEIGHT 600
#define V_Y_INITIAL 5.5
#define THORN_WIDTH 20
#define THORN_HEIGHT 40

// Variables for gravitational influence
double v_y_i = 0;
double del_y = 0;
double gravity = 0.075;
double dt = 0.275;

int gameState = 0;
Image playButton, leaderboardButton, creditsButton, instructionsButton, exitButton, menuBackground, logo, ball, dyn_spike, spike, pop_ball;
double cameraX = 0, cameraY = 0;

double spawnX = 90, spawnY = 150;
double ballx = 90, bally = 150, ball_radius = 20;
double degree = 0;
double animate = 0;
int l = 100;
double velocity_x = .5;
double velocity_y = 1.5;
bool right = 0, left = 0, jump = 0, available_jump = false;
int i = 0;
bool upper_side_collision = false, lower_side_collision = false, left_side_collision = false, right_side_collision = false, collision = false;
// now we dont have work with all those bool functions but they are declared for further collision detection and control . we may need to use them later for controling balls

bool didBallPop = false;
double smallHoop_width = 10, smallHoop_height = 35;
double largeHoop_width = 20, largeHoop_height = 50;

// * Buttons for action: W or Spacebar for jumping, MOUSE_LEFT & MOUSE_RIGHT for movement

// ! Initialises a struct called "platform"
struct platform
{
    double x;
    double y;
    double width;
    double height;
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
// hoop hoops[] = {
//     {260, 2090, 10, 35, false},
//     {1220, 2240, 10, 35, false},
//     {2680, 2090, 10, 35, false},
//     {3010, 2090, 10, 35, false},
//     {3560, 2210, 35, 10, false},
//     {3700, 2210, 35, 10, false}};

// // ! First level platform layout

// platform platforms[] = {
//     {0, 2010, 4000, 40},
//     {0, 2360, 4000, 40},
//     {0, 2050, 40, 310},
//     {3960, 2050, 40, 310},
//     {200, 2160, 120, 200},
//     {450, 2050, 60, 160},
//     {620, 2180, 150, 180},
//     {910, 2050, 50, 110},
//     {910, 2160, 170, 40},
//     {1160, 2160, 120, 40},
//     {1360, 2160, 170, 40},
//     {1480, 2050, 50, 110},
//     {1160, 2310, 120, 50},
//     {1640, 2180, 120, 180},
//     {2240, 2180, 120, 180},
//     {2480, 2050, 60, 160},
//     {2650, 2160, 60, 200},
//     {2820, 2050, 60, 160},
//     {2980, 2160, 60, 200},
//     {3150, 2050, 60, 160},
//     {3400, 2050, 120, 150},
//     {3600, 2160, 60, 200},
//     {3730, 2050, 120, 150}};
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
    {3730.0, 40.0, 120.0, 150.0},
};

hoop hoops[] = {
    {265.0, 80.0, 10, 40, false},
    {1220.0, 230.0, 10, 40, false},
    {2680.0, 80.0, 10, 40, false},
    {3010.0, 80.0, 10, 40, false},
    {3560.0, 205.0, 40, 10, false},
    {3700.0, 205.0, 40, 10, false},
};

trap traps[] = {
    {1010, 90, 1430, 90, 900, 80, .08, .08}
};

thorn thorns[] = {
    {1900, 40},
    {2100, 40},
    {400, 40},
    {3350, 40}
};

int number_of_platforms = 23;
int number_of_hoops = 6;
int number_of_traps = 1;
// TODO: Initialise the below variable after constructing spike code
int number_of_thorns = 4;

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
    for (int i = 0; i < number_of_traps; i++) {
        double tx = traps[i].x;
        double ty = traps[i].y;

        iShowLoadedImage(tx - l/2 - cameraX, ty - l/2 - cameraY, &dyn_spike, 100, 100);

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
    for (int i = 0; i < number_of_platforms; i++) {
        iFilledRectangle(platforms[i].x - cameraX, platforms[i].y - cameraY, platforms[i].width, platforms[i].height);
    }
}
void drawStripesVertical(void) {
    for (int i = 0; i < number_of_platforms; i++) {
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
                    iFilledRectangle(platforms[i].x - cameraX, checked_y - 2.5, platforms[i].width, 5);

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

void drawHoop()
{
    for (int i = 0; i < number_of_hoops; i++)
    {
        if (hoops[i].didBallPass)
            iSetColor(128, 128, 128);
        else
            iSetColor(245, 165, 5);

        iEllipse(hoops[i].x - cameraX, hoops[i].y - cameraY, hoops[i].width, hoops[i].height);
    }
}

//! spikeWidth = 20, spikeHeight = 40.
void drawSpike() {
    // TODO: Write code for rendering spikes
    for(int i=0; i < number_of_thorns; i++) {
        iShowLoadedImage(thorns[i].x - cameraX, thorns[i].y - cameraY, &spike, THORN_WIDTH, THORN_HEIGHT);
    }
    
}

// TODO: Create gameStates, if(gameState <= 5) call gameStateRender to render menus, else run game.
// TODO: 0 -> mainmenu, 1 -> Play, 2 -> leaderboard, 3 -> Contributions, 4 -> Instructions, 5 -> Exit, 6 -> level play.
 void gameStateRender() {
    iShowLoadedImage(0, 0, &menuBackground, 800, 600);

    switch(gameState) {
        case 0:
            // Render the main menu's buttons;
            iShowLoadedImage(520, 340, &playButton, 240, 60);
            iShowLoadedImage(520, 270, &leaderboardButton, 240, 60);
            iShowLoadedImage(520, 200, &creditsButton, 240, 60);
            iShowLoadedImage(520, 130, &instructionsButton, 240, 60);
            iShowLoadedImage(520, 60, &exitButton, 240, 60);
            iShowLoadedImage(460, 325, &logo, 350, 350);
        break;

        case 1:
            //Render the Level Selection Menu after "Play", also progress saved in save file.
        break;

        case 2:
            //Render the Leaderboards from save file.
        break;

        case 3:
            iTextBold(500, 510, "Creators: ");
            iText(500, 510-25, "Safat Ahmed [2405086]", GLUT_BITMAP_HELVETICA_18);
            iText(500, 460, "Hasan Mahmud [2405088]", GLUT_BITMAP_HELVETICA_18);

            iShowLoadedImage(520, 60, &exitButton, 240, 60);
        break;

        case 4:
            //Render the Instructions page
        break;

        case 5:
            exit(0);
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

    // TODO: Completely Implement gameStateRender() here with an if condition.

    if(gameState <= 5) {
        gameStateRender();
    } else {
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

        drawSpike();
        iSetColor(255, 0, 0);

        if(didBallPop) {
            iShowLoadedImage(ballx - cameraX - 20, bally - cameraY - 20, &pop_ball, 40, 40);
        } else {
            iFilledCircle(ballx - cameraX, bally - cameraY, ball_radius);
            iRotate(ballx - cameraX, bally - cameraY, degree);
            iShowLoadedImage(ballx - 20 - cameraX, bally - 20 - cameraY, &ball, 40, 40);
            iUnRotate();
        }
    }
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
            gameState = 6;
        }

        if(mx >= 520 && mx <= 520+240 && my >= 200 && my <= 200+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            gameState = 3;
        }

        if(mx >= 520 && mx <= 520+240 && my >= 60 && my <= 60+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            gameState = 5;
        }
    }
    else if(gameState == 3) {
        if(mx >= 520 && mx <= 520+240 && my >= 60 && my <= 60+60 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                gameState = 0;
            }
    } 
    else {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            left = true;
        }
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        {
            left = false;
        }
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        {
            right = true;
        }
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
        {
            right = false;
        }
    }
}

void iMouseWheel(int dir, int mx, int my)
{
}

void iKeyboard(unsigned char key) {
    if ((key == 'w' || key == ' ') && available_jump) {
        jump = true;
        v_y_i = V_Y_INITIAL;
        
    }
    if(key == 'a') {
        left = !left;
    }
    if(key == 'd') {
        right = !right;
    }
}

void ballmove()
{
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
    for (int i = 0; i < number_of_platforms; i++)
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
            if (ballBottom <= platTop && ballBottom >= platBottom && ballx > platLeft && ballx < platRight)
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

    for (int i = 0; i < number_of_traps; i++)
    {
        double trapLeft = traps[i].x - l / 2;
        double trapRight = traps[i].x + l / 2;
        double trapTop = traps[i].y + l / 2;
        double trapBottom = traps[i].y - l / 2;
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;

        iShowLoadedImage(100, 100, &pop_ball, 40, 40);
        // Detect if the ball collides with any of the traps
        if (ballLeft < trapRight && ballRight > trapLeft && ballTop > trapBottom && ballBottom < trapTop)
        {
            didBallPop = true;
        }
    }

    // TODO: Implement gravitationally influenced y_axis trajectory
    // if (jump) {
    //     v_y_i = V_Y_INITIAL;
    //     // bally += velocity_y;
    //     // i++;
    //     // if (i == 420||lower_side_collision)
    //     // {
    //     //     jump = false;
    //     //     i = 0;

    //     // }
    // }

    // TODO: Implement Hoop Pass and Spike Collision
    //? Used a margin = 3 to make the hoop pass more fluid.
    for (int i = 0; i < number_of_hoops; i++)
    {
        if (ballx >= hoops[i].x - hoops[i].width / 2 - 3 && ballx <= hoops[i].x + hoops[i].width / 2 + 3)
        {
            if (bally >= hoops[i].y - hoops[i].height / 2 - 3 && bally <= hoops[i].y + hoops[i].height / 2 + 3)
            {
                hoops[i].didBallPass = true;
            }
        }
    }

    // TODO: Implement thorn/spike collision
    for(int i=0; i<number_of_thorns; i++) {
        double thornLeft = thorns[i].x;
        double thornRight = thorns[i].x + THORN_WIDTH;
        double thornBottom = thorns[i].y;
        double thornTop = thorns[i].y + THORN_HEIGHT;
        double ballBottom = bally - ball_radius;
        double ballTop = bally + ball_radius;
        double ballLeft = ballx - ball_radius;
        double ballRight = ballx + ball_radius;
        if(ballLeft < thornRight && ballRight > thornLeft && ballTop > thornBottom && ballBottom < thornTop) {
            didBallPop = true;
            // iDelay(2);
            // ballx = spawnX;
            // bally = spawnY;
        }
    }
    updateCamera();
}

void BallPop() {
    if(didBallPop) {
        iDelay(2);
        ballx = spawnX;
        bally = spawnY;
        didBallPop = false;
    }
}

void iSpecialKeyboard(unsigned char key)
{
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

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
        printf("Successfully loaded ballImage!\n");
    else 
        printf("Failed loading ballImage!\n");

    if(iLoadImage(&dyn_spike, "assets/images/dyn_thorn@2x.png"))
        printf("Successfully loaded trap!\n");
    else 
        printf("Failed loading trap!\n");

    if(iLoadImage(&spike, "assets/images/thorn@2x.png"))
        printf("Loaded thorns!\n");
    else 
        printf("Couldn't load thorns.\n");

    if(iLoadImage(&pop_ball, "assets\\images\\ball_pop@2x.png"))
        printf("Loaded ballPop!\n");
    else 
        printf("Couldn't load ballPop.\n");

    iSetTimer(10, animated);
    iSetTimer(1, ballmove);
    iSetTimer(15, BallPop);
    iInitialize(800, 600, "Bounce Classic Lite");
    return 0;
}