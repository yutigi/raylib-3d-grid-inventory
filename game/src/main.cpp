#pragma once

#include "raylib.h"
#include "camera.h"
#include "Data.h"

#include <cmath>
#include <vector>

#include "MathLibrary.h"
#include "raymath.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib 3d-grid-inventory");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    constexpr Vector3 default_camera_position{ 10.0f, 10.0f, 10.0f };
    camera.position = default_camera_position; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
    std::vector<FBox> Boxes{
        FBox(0.f, 1.f, 0.f),
    };
    // Vector3 cubePosition = { 0.f, 1.f, 0.f };
    // Vector3 cubeSize = { 2.f, 2.f, 2.f };
    
    Ray ray = { 0 };                    // Picking line ray
    RayCollision collision = { 0 };     // Ray collision hit info

    //DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        UpdateCameraControl(camera, default_camera_position);
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                const size_t MaxBoxes = Boxes.size();
                for (int i = 0; i < MaxBoxes; ++i)
                {
                    DrawCube(Boxes[i].GetPosition(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), RED);
                    DrawCubeWires(Boxes[i].GetPosition(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), MAROON);
                }
        
                // drop box
                Ray MouseRay = GetScreenToWorldRay(GetMousePosition(), camera);
                constexpr int GridSize = 1000;
                RayCollision GridCollision;
                // check grid
                GridCollision = GetRayCollisionBox(MouseRay,
                                                   BoundingBox{
                                                       Vector3{
                                                           -(GridSize / 2), -1, -(GridSize / 2)
                                                       },
                                                       Vector3{
                                                           GridSize, 1, GridSize
                                                       }
                                                   });
                // check remaining boxes
                constexpr float OffsetUp = 100.f;
                Vector3 MouseRayFromAbovePosition{
                    GridCollision.point.x, GridCollision.point.y + OffsetUp, GridCollision.point.z
                };
                Ray MouseRayFromAbove = Ray{
                                MouseRayFromAbovePosition,
                                Vector3{0,-1,0}
                };
                Vector3 DebugBoxPos = Vector3{MouseRayFromAbove.position.x, MouseRayFromAbove.position.y, MouseRayFromAbove.position.z};
                DrawCube(DebugBoxPos,
                         BOX_SIZE*0.2, BOX_SIZE + OffsetUp,BOX_SIZE*0.2,SKYBLUE);
                
                bool bHitAnyBox = false;
                const size_t MAX = Boxes.size();
                for (int i = 0; i < MAX; ++i)
                {
                    // check boxes collision
                    GridCollision = GetRayCollisionBox(MouseRayFromAbove,Boxes[i].GetBoundingBox());
                    if (GridCollision.hit)
                    {
                        bHitAnyBox = true;
                        GridCollision.point.y += BOX_SIZE;
                        DrawCube(SnapGrid(GridCollision.point,-1.f,BOX_SIZE),BOX_SIZE,BOX_SIZE,BOX_SIZE,GREEN);
                        break;
                    }
                }
                if(!bHitAnyBox)
                {
                    // check grid
                    GridCollision = GetRayCollisionBox(MouseRay,
                                                       BoundingBox{
                                                           Vector3{
                                                               -(GridSize / 2), -1, -(GridSize / 2)
                                                           },
                                                           Vector3{
                                                               GridSize, 1, GridSize
                                                           }
                                                       });
                }
                // Grid mouse trace visualize
                if(GridCollision.hit && !IsCursorHidden())
                {
                    const Vector3 snap_grid_pos = SnapGrid(GridCollision.point,-1.f,BOX_SIZE);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        FBox NewBox(snap_grid_pos.x,snap_grid_pos.y,snap_grid_pos.z);
                        Boxes.emplace_back(NewBox);
                    }
                    // debug
                    // DrawCube(snap_grid_pos, BOX_SIZE, BOX_SIZE, BOX_SIZE, WHITE);
                    DrawCubeWires(snap_grid_pos, BOX_SIZE, BOX_SIZE, BOX_SIZE, BLACK);
                    
                }
        
                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawRectangle( 10, 10, 320, 93, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 320, 93, BLUE);

            DrawText("Right click to rotate the camera", 20, 20, 10, BLACK);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
