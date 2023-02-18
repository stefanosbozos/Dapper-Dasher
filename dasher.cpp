#include <iostream>
#include "raylib.h"

using namespace std;

// // END OF HEADERS AND NAMESPACES // //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    SpriteAnimationData2D is a custom data type to store
    all the data related to the animation of the sprite.

    It contains:

    - the rectangle dimensions to be used to exctract
    the sprite from the spritesheet.
    - The position of the sprite on the screen
    - The frame that we want to choose from the spritesheet.
    - The time that the animation is running.
    - The time that the animation is being updated.

    We create a custom data type using the keyword "struct". 
*/ 

// {Rectangle spriteRectangle, Vector2 position, int frame, float runningTime, float updateTime}
//
// This struct is a blueprint to initialize all the animation
// variables for a 2D sprite.
struct SpriteAnimationData2D
{
    // (Rectangle)
    //
    //The rectangle that contains the sprite.
    Rectangle spriteRectangle; 

    // (Vector2) 
    //
    //The position of the sprite on the canvas.
    Vector2 position;

    // (int) 
    //
    //The current sprite animation of the spritesheet.
    int frame;

    // (float)
    //
    // Timer that keeps time of the animation display on the canvas.
    float runningTime;

    // (float)
    //
    // The amount of time before we update the animation frame
    // We want to update the animation 12 times per second in order to achieve a smooth running animation.
    float updateTime;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(){

    // // MAIN CANVAS INITIALIZATION // //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const int window_width{512};                                        // Main Canvas width dimensions in pixels
    const int window_height{380};                                       // Main Canvas height dimensions in pixels
    
    InitWindow(window_width, window_height, "Dapper Dahser v1.0");      // Creates a canvas on the screen given the width and heigh in pixels, and the title of the window.
    
    SetTargetFPS(60);                                                   // Set the targer frames per second of the main canvas.
    
    

    // // GAME PHYSICS & VELOCITIES // //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const int gravity{1'000};                               // Gravity modifier in pixels per second (The apostrophe is ignored by the compiler. Is used only to make the number more readable.)

    bool isInAir{false};                                    // Check if the player is in the air to avoid double jumping

    int player_jump_velocity{-600};                         // The amount of velocity on the Y axis simulating a jump in (pixels/second)

    int player_velocity{0};                                 // Player's vertical velocity (pixels per second)   

    int enemyNebula_Velocity{-600};                         // Enemy's Nebula horizontal velocity in (pixels per second)



    // // // ANIMATION INITIALIZATION  // // //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // // // PLAYER ANIMATION // // //
    Texture2D playerSpritesheet = LoadTexture("textures/scarfy.png");

    // The player's sprite animation frame
    SpriteAnimationData2D playerAnimation{
        {0.0, 0.0, playerSpritesheet.width/6, playerSpritesheet.height},                                                        // Sprite rectangle dimensions
        {window_width/2 - playerAnimation.spriteRectangle.width/2, window_height - playerAnimation.spriteRectangle.height},     // PosX and PosY on the spritesheet
        {0},                                                                                                                    // Current frame
        {0.0f},                                                                                                                 // Timer that keeps time of the animation display
        {1.0f/12.0f}                                                                                                            // Time that the current frame will be changed to the next on the spritesheet
    }; 
    


    // // // ENEMY ANIMATION // // //

    // NEBULA
    Texture2D nebulaSpritesheet = LoadTexture("textures/12_nebula_spritesheet.png"); // Loads the Nebula enemy spritesheet (8x8)

    // Enemy's nebula sprite animation frame
    SpriteAnimationData2D nebula{
        {0.0, 0.0, nebulaSpritesheet.width/8, nebulaSpritesheet.height/8},      // Rectangle Dimensions
        {window_width, window_height - nebula.spriteRectangle.height/8},        // Rectangle posX and posY
        {0},                                                                    // Current frame of the spritesheet
        {0.0},                                                                  // Timer that keeps time of the animation display
        {1.0f/12.0f}                                                            // The amount of time before we update the animation
    };

    // Second enemy nebula sprite animation frame
    SpriteAnimationData2D nebula2{
        {0.0, 0.0, nebulaSpritesheet.width/8, nebulaSpritesheet.height/8},      // Rectangle Dimensions
        {window_width + 300, window_height - nebula.spriteRectangle.height/8},  // Rectangle posX and posY
        {0},                                                                    // Current frame of the spritesheet
        {0.0},                                                                  // Timer that keeps time of the animation display
        {1.0f/12.0f}                                                            // The amount of time before we update the animation
    };


    
    // GAME LOOP START //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // While the user does not click on the "X" button loop through the below code //
    while(!WindowShouldClose())
    {
        
        BeginDrawing();                                                         // Start the frame buffer

        ClearBackground(WHITE);                                                 // Refresh the frame buffer with a white backgroud

        float delta_time{GetFrameTime()};                                       // Delta Time (time since last frame)
        

        // // // PLAYER GRAVITY // // //

        // Check every frame if the player is on the ground
        if(playerAnimation.position.y >= (window_height - playerAnimation.spriteRectangle.height))
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
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        // Player Jumping
        if(IsKeyDown(KEY_SPACE) && !isInAir)
        {   
            player_velocity += player_jump_velocity;
        }

        

        // // POSITION UPDATE // //
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        nebula.position.x += enemyNebula_Velocity * delta_time;                     // Update the enemy nebula position on the X axis

        nebula2.position.x += enemyNebula_Velocity * delta_time;                    // Update the second enemy nebula position on the X axis

        playerAnimation.position.y += player_velocity * delta_time;                 // Update the player's velocity by 10 every frame on the Y axis



        // // ANIMATION // //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
            playerAnimation.runningTime += delta_time;

            //Update the player's animation
            // Check if running time has surpassed update time
            if(playerAnimation.runningTime >= playerAnimation.updateTime)
            {
                playerAnimation.runningTime = 0.0f;

                playerAnimation.spriteRectangle.x = playerAnimation.frame * playerAnimation.spriteRectangle.width;
                playerAnimation.frame++; // Go to the next frame
            
                // Reset the frame to avoid exceeding the spritesheet's width
                if(playerAnimation.frame > 5)
                {
                    playerAnimation.frame = 0;
                }
            }
        }


        // Update the Enemy Nebula Animation
        nebula.runningTime += delta_time;
        if(nebula.runningTime >= nebula.updateTime)
        {
            nebula.runningTime = 0.0f;

            nebula.spriteRectangle.x = nebula.frame * nebula.spriteRectangle.width;
            nebula.frame++;

            if(nebula.frame > 7)
            {
                nebula.frame = 0;
            }
        }

        // Update the second enemy Nebula Animation
        nebula2.runningTime += delta_time;
        if(nebula2.runningTime >= nebula2.updateTime)
        {
            nebula2.runningTime = 0.0f;

            nebula2.spriteRectangle.x = nebula2.frame * nebula2.spriteRectangle.width;
            nebula2.frame++;

            if(nebula2.frame > 7)
            {
                nebula2.frame = 0;
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Draw the enemy Nebula
        DrawTextureRec(nebulaSpritesheet, nebula.spriteRectangle, nebula.position, WHITE);

        // Draw the second enemy Nebula
        DrawTextureRec(nebulaSpritesheet, nebula2.spriteRectangle, nebula2.position, RED);

        // Draw the player on the screen
        DrawTextureRec(playerSpritesheet, playerAnimation.spriteRectangle, playerAnimation.position, WHITE);

        // Close the frame buffer
        EndDrawing();

    }

    // // GAME LOOP END // //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Unload the texture that is loaded in the Texture buffer.
    // Everytime in Raylib we load a texture at the start of the program,
    // we need to unload the texture at the end of the program.
    UnloadTexture(playerSpritesheet);
    UnloadTexture(nebulaSpritesheet);
    // Close the window that we have initialized before the program ends //
    CloseWindow();

}