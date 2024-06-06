#include "raylib.h"
#include "camera.h"
#include <cmath>
#include <vector>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

#define BOX_SIZE 2.f

typedef struct FBox
{
    Vector3 Position;
    float Scale;

    FBox(float x,float y,float z)
    {
        Position = Vector3{x,y,z};
        Scale = BOX_SIZE;
    }
    ~FBox()
    {
        
    }
    Vector3 GetPosition() const
    {
        return Position;
    }
    Vector3 GetScale() const
    {
        return Vector3{Scale,Scale,Scale};
    }
    float GetScalef() const
    {
        return Scale;
    }
    void SetPosition(const Vector3 NewPosition)
    {
        Position = NewPosition;
    }
    BoundingBox GetBoundingBox() const
    {
        return BoundingBox{
            Vector3{
                Position.x - Scale / 2, Position.y - Scale / 2,
                Position.z - Scale / 2
            },
            Vector3{
                Position.x + Scale / 2, Position.y + Scale / 2,
                Position.z + Scale / 2
            }
        };
    }
} FBox;

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

                // ray hit
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if (!collision.hit)
                    {
                        ray = GetScreenToWorldRay(GetMousePosition(), camera);
                        // Check collision between ray and box
                        const size_t MAX = Boxes.size();
                        for (int i = 0; i < MAX; ++i)
                        {
                            collision = GetRayCollisionBox(ray,Boxes[i].GetBoundingBox());
                            if (collision.hit)
                            {
                                DrawCube(Boxes[i].GetPosition(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), RED);
                                DrawCubeWires(Boxes[i].GetPosition(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), Boxes[i].GetScalef(), MAROON);

                                DrawCubeWires(Boxes[i].GetPosition(), Boxes[i].GetScalef() + 0.2f, Boxes[i].GetScalef() + 0.2f, Boxes[i].GetScalef() + 0.2f, GREEN);
                            }
                        }
                    }
                    else collision.hit = false;
                }
        
                
                // drop box
                Ray MouseRay = GetScreenToWorldRay(GetMousePosition(), camera);
                constexpr int GridSize = 1000;
                RayCollision GridCollision = GetRayCollisionBox(MouseRay,
                                                                BoundingBox{
                                                                    Vector3{
                                                                        -(GridSize/2), 0, -(GridSize/2)
                                                                    },
                                                                    Vector3{
                                                                        GridSize, 1, GridSize
                                                                    }
                                                                });
                // Grid mouse trace visualize
                if(GridCollision.hit && !IsCursorHidden())
                {
                    const Vector3 snap_grid_pos = Vector3{
                        ceil(GridCollision.point.x),
                        ceil(GridCollision.point.y),
                        ceil(GridCollision.point.z)
                    };
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        FBox NewBox(snap_grid_pos.x,snap_grid_pos.y,snap_grid_pos.z);
                        Boxes.emplace_back(NewBox);
                    }

                    DrawCube(snap_grid_pos, BOX_SIZE, BOX_SIZE, BOX_SIZE, WHITE);
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
