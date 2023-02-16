#include <iostream>
#include "raylib.h"

using namespace std;

int main(){

    // Window dimensions and main window initialization //
    const int window_width{512};
    const int window_height{380};

    InitWindow(window_width, window_height, "Dapper Dasher v1.0");
    
    // // GAME PHYSICS // //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Gravity in (pixel/s)/s
    const int gravity{1'000}; // The apostrophe is ignored by the compiler. Is used only to make the number more readable.

    // Check if the player is in the air to avoid double jumpingp
    bool isInAir{false};

    // The amount of velocity on the Y axis simulating a jump in (pixels/second)
    int player_jump_velocity{-600};

    // Initialize player's Velocity //
    int player_velocity{0};

    // Initialize Enemy Nebula Velocity in (pixels/second)//
    int enemyNebula_Velocity{-600}; 

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // // GAME RUNTIME AND ANIMATION RELATED VARIABLES // //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Frames per second set
    SetTargetFPS(60);

    // Initialize Player animation frame in the spritesheet "scarfy.png"
    int frame{};

    // Initialize Enemy Nebula animation frame.
    int enemyNebula_frame{};

    // The amount of time before we update the animation frame
    // We want to update the animation 12 times per second in order to achieve a smooth running animation.
    const float player_updateTime{1.0f / 12.0f};
    const float enemyNebule_updateTime{1.0f/12.0f};

    float player_runningTime{};
    float enemyNebula_runningTime{};

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    // // // SPRITE INITIALIZATION  // // //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Texture2D, Rectangle and Vector2 are "compount data types", which are a custom data type 
    // created by the raylib library. LoadTecture is a method of the Texture2D class

    // // // PLAYER SPRITE // // //
    Texture2D player = LoadTexture("textures/scarfy.png");

    Rectangle playerRectangle; // The rectangle that contains the player sprite

    // Extract the first sprite from the texture and Initialize the player rectagnle 
    playerRectangle.height = player.height; // The height of the sprite sheet
    playerRectangle.width = player.width/6; // The width of the image that we want to extract from the spritesheet. 

    // We divide it by 6 because there are 6 sprites. In general the rule is heght/n for n sprites on the Y axis of the spreadsheet.
    // and width/n for n sprites on the X axis of the spritesheet.

    playerRectangle.x = 0; // The position of where the rectangle starts on the X axis.
    playerRectangle.y = 0; // The position of where the rectangle start on the Y axis.

    // The below vector is to determine where the player will be drawn on the screen.
    Vector2 playerPosition;
    playerPosition.x = window_width/2 - playerRectangle.width/2; // Draw the player in the middle of the window on the X axis.
    // the reason why we subtract it from the playerRectangle is to have the correct offset and align the player.
    // at the middle of the screen. We divide the width of the sprite to center it exactly in the middle.

    playerPosition.y = window_height - playerRectangle.height; // Place the player at the bottom of the screen on the Y axis.

    
    // // // ENEMY SPRITES // // //

    // NEBULA
    Texture2D enemyNebula = LoadTexture("textures/12_nebula_spritesheet.png"); // Loads the Nebula enemy spritesheet (8x8)

    Rectangle enemyNebula_Rectangle{0.0, 0.0, enemyNebula.width/8, enemyNebula.height/8};

    // // Dimensions of the sprite rectangle for every frame
    // enemyNebula_Rectangle.height = enemyNebula.height/8;
    // enemyNebula_Rectangle.width = enemyNebula.width/8;

    // // Starting point of the enemy rectangle on the spritesheet.
    // enemyNebula_Rectangle.x = 0;
    // enemyNebula_Rectangle.y = 0;

    Vector2 enemyNebula_Position{window_width, (window_height - enemyNebula_Rectangle.height)};

    // // The starting position of the enemy nebula in the scene.
    // // We want the enemy to appear outside of the player's sight after the right edge of the window.
    // enemyNebula_Position.x = window_width;
    // enemyNebula_Position.y = window_height - enemyNebula_Rectangle.height;


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    
    // GAME LOOP START //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // While the user does not click on the "X" button loop through the below code //
    while(!WindowShouldClose())
    {
        
        // Start the frame buffer //
        BeginDrawing();

        // Refresh the frame buffer with a white backgroud //
        ClearBackground(WHITE);

        // Delta Time (time since last frame) //
        float delta_time{GetFrameTime()};

        // // // PLAYER GRAVITY // // //

        // Check every frame if the player is on the ground
        if(playerPosition.y >= (window_height - playerRectangle.height))
        {
            player_velocity = 0;
            isInAir = false;
        }
        else
        {
            // Apply Gravity to the player if he is not touching the ground
            player_velocity += gravity * delta_time;
            isInAir = true;
        }

        // // // PLAYER INPUT // // //
        //////////////////////////////////////////////////////////////////////
        // Jump //
        if(IsKeyDown(KEY_SPACE) && !isInAir)
        {   
            player_velocity += player_jump_velocity;
        }

        /////////////////////////////////////////////////////////////////////

        // Update the enemy nebula position on the X axis //
        enemyNebula_Position.x += enemyNebula_Velocity * delta_time;

        // Update the player's velocity by 10 every frame on the Y axis //
        playerPosition.y += player_velocity * delta_time;


        // // ANIMATION // //
        //////////////////////////////////////////////////////////////////////


        // ***General formula for updating animation from a spritesheet***
        // x = frame * width/n
        // x -> the animation we want to draw
        // frame -> the frame on the spritesheet
        // width -> the spritesheet's width
        // n -> the number of animations on the spritesheet

        // We want to freeze the animation while the player is in air
        // so the player does not "run" while jumping.
        if(!isInAir)
        {
            // Update running time
            player_runningTime += delta_time;
            enemyNebula_runningTime += delta_time;

            //Update the player's animation
            // Check if running time has surpassed update time
            if(player_runningTime >= player_updateTime)
            {
                player_runningTime = 0.0f;

                playerRectangle.x = frame * playerRectangle.width;
                frame++; // Go to the next frame
            

                // Reset the frame to avoid exceeding the spritesheet's width
                if(frame > 5)
                {
                    frame = 0;
                }
            }

            // Update the Enemy Nebula Animation
            if(enemyNebula_runningTime >= enemyNebule_updateTime)
            {
                enemyNebula_runningTime = 0.0f;

                enemyNebula_Rectangle.x = enemyNebula_frame * enemyNebula_Rectangle.width;
                enemyNebula_frame++;

                if(enemyNebula_frame > 7)
                {
                    enemyNebula_frame = 0;
                }
            }
        }

        

        ///////////////////////////////////////////////////////////////////////

        // Draw the enemy Nebula //
        DrawTextureRec(enemyNebula, enemyNebula_Rectangle, enemyNebula_Position, WHITE);
        
        // Draw the player on the screen //
        DrawTextureRec(player, playerRectangle, playerPosition, WHITE);

        // Close the frame buffer //
        EndDrawing();

    }

    // // GAME LOOP END // //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Unload the texture that is loaded in the Texture buffer.
    // Everytime in Raylib we load a texture at the start of the program,
    // we need to unload the texture at the end of the program.
    UnloadTexture(player);
    UnloadTexture(enemyNebula);
    // Close the window that we have initialized before the program ends //
    CloseWindow();

}