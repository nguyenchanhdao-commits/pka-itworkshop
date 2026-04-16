/*
 * Mario Adventure Game - Raylib Edition
 *
 * A 2D grid-based game where Mario must reach a flag while avoiding Goombas.
 * Features graphical rendering with Raylib, AI enemy behavior, and health system.
 *
 * Controls: W/A/S/D to move, Q to quit
 * Goal: Reach the green flag without losing all health
 */

// Include Raylib for graphics and input handling
#include "raylib.h"

// Standard C libraries for utilities
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()
#include <math.h>   // abs()

// Game configuration constants
#define SCREEN_WIDTH 1000 // Window width in pixels
#define SCREEN_HEIGHT 600 // Window height in pixels
#define GRID_WIDTH 20     // Number of cells horizontally
#define GRID_HEIGHT 10    // Number of cells vertically
#define CELL_SIZE 40      // Size of each grid cell in pixels

// Game entity structures

// Player (Mario) structure
typedef struct
{
    int x;      // X coordinate on the grid (1-18)
    int y;      // Y coordinate on the grid (1-8)
    int health; // Player health points (starts at 100)
} Player;

// Goal (Flag) structure - the objective to reach
typedef struct
{
    int x; // X coordinate on the grid
    int y; // Y coordinate on the grid
} Goal;

// Enemy AI states for behavior switching
typedef enum
{
    PATROL, // Enemy moves randomly around the map
    CHASE   // Enemy actively pursues the player
} EnemyState;

// Enemy (Goomba) structure
typedef struct
{
    int x;            // X coordinate on the grid
    int y;            // Y coordinate on the grid
    EnemyState state; // Current AI behavior state
} Enemy;

// Utility functions for game logic

/*
 * Calculate Manhattan distance between two points
 * Used for AI decision making and entity placement
 */
int manhattan_distance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

/*
 * Place an entity randomly within the playable area
 * Avoids placing entities on the border walls
 */
void place_random_inside(int *x, int *y)
{
    *x = rand() % 18 + 1; // Random X between 1-18
    *y = rand() % 8 + 1;  // Random Y between 1-8
}

/*
 * Place an entity far away from a reference point
 * Ensures minimum distance for fair gameplay
 */
void place_random_far_away(int *x, int *y, int px, int py)
{
    do
    {
        place_random_inside(x, y);
    } while (manhattan_distance(*x, *y, px, py) < 5); // Minimum distance of 5
}

/*
 * Respawn player and enemy after collision
 * Ensures they don't spawn on the goal or each other
 */
void respawn_player_and_enemy(Player *p, Enemy *e, const Goal *g)
{
    // Place player randomly, avoiding goal position
    place_random_inside(&p->x, &p->y);
    while (p->x == g->x && p->y == g->y)
    {
        place_random_inside(&p->x, &p->y);
    }

    // Place enemy far from player, avoiding goal and player positions
    place_random_far_away(&e->x, &e->y, p->x, p->y);
    while ((e->x == g->x && e->y == g->y) || (e->x == p->x && e->y == p->y))
    {
        place_random_far_away(&e->x, &e->y, p->x, p->y);
    }

    // Reset enemy to patrol mode
    e->state = PATROL;
}

/*
 * Render the entire game board with all entities
 * Draws grid, walls, flag, Mario, and Goomba
 */
void draw_game_board(Player *p, Enemy *e, Goal *g)
{
    // Calculate offset to center the game board on screen
    int offset_x = 100;
    int offset_y = 100;

    // Draw decorative border around the game area
    DrawRectangle(offset_x - 10, offset_y - 10,
                  GRID_WIDTH * CELL_SIZE + 20,
                  GRID_HEIGHT * CELL_SIZE + 20, BLUE);
    DrawRectangle(offset_x - 5, offset_y - 5,
                  GRID_WIDTH * CELL_SIZE + 10,
                  GRID_HEIGHT * CELL_SIZE + 10, WHITE);

    // Draw grid lines for visual reference
    for (int i = 0; i <= GRID_HEIGHT; i++)
    {
        DrawLine(offset_x, offset_y + i * CELL_SIZE,
                 offset_x + GRID_WIDTH * CELL_SIZE, offset_y + i * CELL_SIZE, LIGHTGRAY);
    }
    for (int j = 0; j <= GRID_WIDTH; j++)
    {
        DrawLine(offset_x + j * CELL_SIZE, offset_y,
                 offset_x + j * CELL_SIZE, offset_y + GRID_HEIGHT * CELL_SIZE, LIGHTGRAY);
    }

    // Draw walls around the playable area (borders)
    DrawRectangle(offset_x, offset_y, GRID_WIDTH * CELL_SIZE, CELL_SIZE, DARKBLUE);
    DrawRectangle(offset_x, offset_y + (GRID_HEIGHT - 1) * CELL_SIZE, GRID_WIDTH * CELL_SIZE, CELL_SIZE, DARKBLUE);
    DrawRectangle(offset_x, offset_y, CELL_SIZE, GRID_HEIGHT * CELL_SIZE, DARKBLUE);
    DrawRectangle(offset_x + (GRID_WIDTH - 1) * CELL_SIZE, offset_y, CELL_SIZE, GRID_HEIGHT * CELL_SIZE, DARKBLUE);

    // Draw the flag (goal) - green rectangle with darker top
    int flag_x = offset_x + g->x * CELL_SIZE + CELL_SIZE / 2;
    int flag_y = offset_y + g->y * CELL_SIZE + CELL_SIZE / 2;
    DrawRectangle(flag_x - 10, flag_y - 15, 20, 25, GREEN);
    DrawRectangle(flag_x - 8, flag_y - 12, 16, 3, DARKGREEN);

    // Draw Mario - classic Mario character design with hat, mustache, overalls, and shoes
    int mario_x = offset_x + p->x * CELL_SIZE + CELL_SIZE / 2;
    int mario_y = offset_y + p->y * CELL_SIZE + CELL_SIZE / 2;

    // Mario's red hat
    DrawRectangle(mario_x - 8, mario_y - 15, 16, 8, RED);
    DrawRectangle(mario_x - 6, mario_y - 13, 12, 4, RED);

    // "M" on the hat
    DrawText("M", mario_x - 3, mario_y - 14, 6, WHITE);

    // Mario's face (beige skin)
    DrawEllipse(mario_x, mario_y - 3, 7, 6, (Color){255, 218, 185, 255});

    // Mario's eyes
    DrawCircle(mario_x - 2, mario_y - 5, 1, BLACK);
    DrawCircle(mario_x + 2, mario_y - 5, 1, BLACK);

    // Mario's mustache
    DrawEllipse(mario_x, mario_y - 2, 3, 1, BLACK);

    // Mario's red shirt
    DrawRectangle(mario_x - 6, mario_y + 3, 12, 8, RED);

    // Mario's blue overalls
    DrawRectangle(mario_x - 8, mario_y + 11, 16, 10, BLUE);

    // Overall straps
    DrawRectangle(mario_x - 7, mario_y + 3, 2, 8, BLUE);
    DrawRectangle(mario_x + 5, mario_y + 3, 2, 8, BLUE);

    // Buttons on overalls
    DrawCircle(mario_x - 5, mario_y + 15, 1, YELLOW);
    DrawCircle(mario_x + 5, mario_y + 15, 1, YELLOW);

    // Mario's arms
    DrawRectangle(mario_x - 10, mario_y + 5, 4, 6, (Color){255, 218, 185, 255});
    DrawRectangle(mario_x + 6, mario_y + 5, 4, 6, (Color){255, 218, 185, 255});

    // Mario's brown shoes
    DrawRectangle(mario_x - 8, mario_y + 21, 6, 4, BROWN);
    DrawRectangle(mario_x + 2, mario_y + 21, 6, 4, BROWN);

    // Draw Goomba (enemy) - yellow ellipse with black eyes
    int goomba_x = offset_x + e->x * CELL_SIZE + CELL_SIZE / 2;
    int goomba_y = offset_y + e->y * CELL_SIZE + CELL_SIZE / 2;
    DrawEllipse(goomba_x, goomba_y, 12, 10, YELLOW);  // Body
    DrawCircle(goomba_x - 5, goomba_y - 4, 2, BLACK); // Left eye
    DrawCircle(goomba_x + 5, goomba_y - 4, 2, BLACK); // Right eye
}

/*
 * Draw the user interface elements
 * Shows title, status information, and controls
 */
void draw_ui(Player *p, Enemy *e, Goal *g, int game_state)
{
    // Draw game title at the top
    DrawText("MARIO ADVENTURE", 350, 20, 40, RED);

    // Draw status bar background with border
    DrawRectangle(100, 500, 800, 80, LIGHTGRAY);
    DrawRectangle(100, 500, 800, 1, BLACK); // Top border
    DrawRectangle(100, 580, 800, 1, BLACK); // Bottom border
    DrawRectangle(100, 500, 1, 80, BLACK);  // Left border
    DrawRectangle(899, 500, 1, 80, BLACK);  // Right border

    // Display player information
    DrawText(TextFormat("Mario Health: %d", p->health), 120, 515, 20, BLACK);
    DrawText(TextFormat("Mario Position: (%d, %d)", p->x, p->y), 120, 540, 20, BLACK);

    // Display game object information
    DrawText(TextFormat("Flag Position: (%d, %d)", g->x, g->y), 500, 515, 20, BLACK);
    DrawText(TextFormat("Goomba State: %s", e->state == CHASE ? "CHASING" : "PATROLLING"), 500, 540, 20, BLACK);

    // Display control instructions
    DrawText("Controls: W/A/S/D = Move | Q = Quit", 120, 570, 16, DARKGRAY);
}

/*
 * Main game function - entry point
 * Initializes game, handles main loop, and cleanup
 */
int main()
{
    // Initialize Raylib window and graphics
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mario Adventure - Raylib Edition");
    SetTargetFPS(60); // Target 60 frames per second

    // Seed random number generator for game randomness
    srand((unsigned)time(NULL));

    // Initialize Player (Mario)
    Player p;
    place_random_inside(&p.x, &p.y);
    p.health = 100;

    // Initialize Goal (Flag) - ensure it doesn't spawn on player
    Goal g;
    place_random_inside(&g.x, &g.y);
    while (g.x == p.x && g.y == p.y)
    {
        place_random_inside(&g.x, &g.y);
    }

    // Initialize Enemy (Goomba) - ensure it doesn't spawn on player or goal
    Enemy e;
    place_random_far_away(&e.x, &e.y, p.x, p.y);
    while ((e.x == g.x && e.y == g.y) || (e.x == p.x && e.y == p.y))
    {
        place_random_far_away(&e.x, &e.y, p.x, p.y);
    }
    e.state = PATROL; // Start in patrol mode

    // Game state variables
    int game_state = 0;  // 0 = playing, 1 = won, 2 = quit
    int frame_count = 0; // Frame counter for timing enemy AI updates

    // Main game loop - runs while window is open and game is active
    while (!WindowShouldClose() && game_state == 0)
    {
        // ===== INPUT HANDLING =====
        // Process player movement input (WASD keys)
        if (IsKeyPressed(KEY_W)) // Move up
        {
            if (p.y > 1) // Check boundary (avoid top wall)
                p.y--;
        }
        if (IsKeyPressed(KEY_S)) // Move down
        {
            if (p.y < 8) // Check boundary (avoid bottom wall)
                p.y++;
        }
        if (IsKeyPressed(KEY_A)) // Move left
        {
            if (p.x > 1) // Check boundary (avoid left wall)
                p.x--;
        }
        if (IsKeyPressed(KEY_D)) // Move right
        {
            if (p.x < 18) // Check boundary (avoid right wall)
                p.x++;
        }
        if (IsKeyPressed(KEY_Q)) // Quit game
        {
            game_state = 2;
        }

        // ===== GAME LOGIC =====
        // Check win condition - player reached the flag
        if (p.x == g.x && p.y == g.y)
        {
            game_state = 1; // Set to win state
        }

        // Check collision with enemy - player takes damage
        if (p.x == e.x && p.y == e.y)
        {
            p.health -= 10;                       // Reduce health by 10
            respawn_player_and_enemy(&p, &e, &g); // Respawn both entities
        }

        // ===== ENEMY AI =====
        // Update enemy behavior every 30 frames (twice per second at 60 FPS)
        frame_count++;
        if (frame_count % 30 == 0)
        {
            // Calculate distance to player to decide AI state
            int dist = manhattan_distance(e.x, e.y, p.x, p.y);
            e.state = dist < 4 ? CHASE : PATROL; // Switch to chase if close

            if (e.state == CHASE)
            {
                // Chase mode: move directly toward player
                int dx = p.x - e.x; // X distance to player
                int dy = p.y - e.y; // Y distance to player

                // Move in the direction with greater distance
                if (abs(dx) > abs(dy))
                {
                    e.x += (dx > 0) ? 1 : -1; // Move horizontally toward player
                }
                else if (dy != 0)
                {
                    e.y += (dy > 0) ? 1 : -1; // Move vertically toward player
                }
            }
            else
            {
                // Patrol mode: move randomly in one of 4 directions
                int dir = rand() % 4; // Random direction (0-3)
                int next_x = e.x;
                int next_y = e.y;

                // Choose direction: 0=right, 1=left, 2=down, 3=up
                switch (dir)
                {
                case 0:
                    next_x++;
                    break; // Right
                case 1:
                    next_x--;
                    break; // Left
                case 2:
                    next_y++;
                    break; // Down
                case 3:
                    next_y--;
                    break; // Up
                }

                // Only move if new position is within bounds
                if (next_x >= 1 && next_x <= 18 && next_y >= 1 && next_y <= 8)
                {
                    e.x = next_x;
                    e.y = next_y;
                }
            }

            // Check for collision after enemy movement
            if (e.x == p.x && e.y == p.y)
            {
                p.health -= 10;
                respawn_player_and_enemy(&p, &e, &g);
            }
        }

        // ===== RENDERING =====
        // Begin drawing frame
        BeginDrawing();
        ClearBackground(WHITE); // Clear screen with white background

        // Draw game elements
        draw_game_board(&p, &e, &g);
        draw_ui(&p, &e, &g, game_state);

        // End drawing frame
        EndDrawing();
    }

    // ===== END GAME SCREEN =====
    // Display win/lose screen after game ends
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if (game_state == 1) // Win condition
        {
            DrawText("CONGRATULATIONS!", 250, 200, 60, GREEN);
            DrawText("Mario reached the flag!", 200, 300, 40, BLACK);
            DrawText("Press any key to exit", 300, 400, 30, GRAY);
        }
        else // Quit condition
        {
            DrawText("Thanks for playing!", 300, 200, 60, RED);
            DrawText("See you next time!", 350, 300, 40, BLACK);
        }

        EndDrawing();

        // Exit on any key press or window close
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || WindowShouldClose())
        {
            break;
        }
    }

    // ===== CLEANUP =====
    CloseWindow(); // Close Raylib window and cleanup resources
    return 0;
}
