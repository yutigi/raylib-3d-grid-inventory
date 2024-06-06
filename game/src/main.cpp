#include <cmath>

#include "camera.h"
#include "raylib.h"
#include "camera.h"


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    constexpr Vector3 default_camera_position{ 10.0f, 10.0f, 10.0f };
    camera.position = default_camera_position; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.f, 1.f, 0.f };
    Vector3 cubeSize = { 2.f, 2.f, 2.f };
    
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

        // ray hit
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!collision.hit)
            {
                ray = GetScreenToWorldRay(GetMousePosition(), camera);

                // Check collision between ray and box
                collision = GetRayCollisionBox(ray,
                                               BoundingBox{
                                                   Vector3{
                                                       cubePosition.x - cubeSize.x / 2, cubePosition.y - cubeSize.y / 2,
                                                       cubePosition.z - cubeSize.z / 2
                                                   },
                                                   Vector3{
                                                       cubePosition.x + cubeSize.x / 2, cubePosition.y + cubeSize.y / 2,
                                                       cubePosition.z + cubeSize.z / 2
                                                   }
                                               });
            }
            else collision.hit = false;
        }

        // drop box
        Ray MouseRay = GetScreenToWorldRay(GetMousePosition(), camera);
        RayCollision GridCollision = GetRayCollisionBox(MouseRay,
                           BoundingBox{
                               Vector3{
                                   -500, 0, -500
                               },
                               Vector3{
                                   1000, 1, 1000
                               }
                           });
        

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                if (collision.hit)
                {
                    DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
                    DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);

                    DrawCubeWires(cubePosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
                }
        
                // Grid mouse trace visualize
                if(GridCollision.hit)
                {
                    const Vector3 snap_grid_pos = Vector3{
                        ceil(GridCollision.point.x),
                        ceil(GridCollision.point.y),
                        ceil(GridCollision.point.z)
                    };
                    DrawCube(snap_grid_pos, cubeSize.x, cubeSize.y, cubeSize.z, WHITE);
                    DrawCubeWires(snap_grid_pos, cubeSize.x, cubeSize.y, cubeSize.z, BLACK);
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
