#include <iostream>
#include "raylib.h"

using namespace std;

int main(){

    // Window dimensions and main window initialization //
    const int window_width{512};
    const int window_height{380};

    InitWindow(window_width, window_height, "Dapper Dasher v1.0");

    // Frames per second set //
    SetTargetFPS(60);

    //// General Gameplay Variables ////
    
    // Game Physics //
    const int gravity{1};

    // Check if the player is in the air to avoid double jumpingp
    bool isInAir{false};

    // The amount of velocity on the Y axis simulating a jump
    int player_jump_velocity{-22};


    // // PROTOTYPING USE ONLY // //
    // //// Rectangle Properties ////
    // // Dimensions //
    // int rect_width{30};
    // int rect_height{50};

    // // Initial Position //
    // int rect_positionX{window_width / 2};
    // int rect_positionY{window_height - rect_height};


    // Initialize player's Velocity //
    int player_velocity{0};


    // // // SPRITE INITIALIZATION  // // //
    // Texture2D, Rectangle and Vector2 are "compount data types", which are a custom data type 
    // created by the raylib library. LoadTecture is a method of the Texture2D class
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



    // Begin the Gameloop //
    while(!WindowShouldClose()){
        
        // Start the frame buffer //
        BeginDrawing();

        // Refresh the frame buffer with a white backgroud //
        ClearBackground(WHITE);

        // // PROTOTYPING USE ONLY // //
        // // Draw the rectangle on the screen //
        // DrawRectangle(rect_positionX, rect_positionY, rect_width, rect_height, BLUE);


        // // // GRAVITY // // //

        // Check every frame if the player is on the ground
        if(playerPosition.y >= (window_height - playerRectangle.height))
        {
            player_velocity = 0;
            isInAir = false;
        }
        else
        {
            // Apply Gravity to the player if he is not touching the ground
            player_velocity += gravity;
            isInAir = true;
        }

        // // // PLAYER INPUT // // //

        // Jump //
        if(IsKeyDown(KEY_SPACE) && !isInAir)
        {   
            player_velocity += player_jump_velocity;
        }

        // // // PLAYER INPUT END // //


        // Update the player's velocity by 10 every frame on the Y axis //
        playerPosition.y += player_velocity;

        // Draw the player on the screen //
        DrawTextureRec(player, playerRectangle, playerPosition, WHITE);

        // Close the frame buffer //
        EndDrawing();

    }

    // Unload the texture that is loaded in the Texture buffer.
    // Everytime in Raylib we load a texture at the start of the program,
    // we need to unload the texture at the end of the program.
    UnloadTexture(player);

    // Close the window that we have initialized before the program ends //
    CloseWindow();

}