# Mario Adventure Game - Raylib Edition

A graphical implementation of the classic Mario adventure game using Raylib, featuring smooth graphics, animations, and modern C programming techniques.

## 🎮 Game Overview

**Mario Adventure** is a 2D grid-based game where you control Mario to reach a flag while avoiding Goombas. The game features:

- **Mario (Player)**: Classic Mario character with red hat, mustache, blue overalls, and brown shoes
- **Flag (Goal)**: Green objective to reach
- **Goomba (Enemy)**: Yellow enemy that patrols and chases
- **Health System**: Start with 100 HP, lose 10 when hit
- **AI Enemy**: Goomba switches between patrol and chase modes

## 📋 Prerequisites

### Required Software

- **Visual Studio** (with C++ build tools) or **MSVC compiler**
- **Raylib 5.0** for Windows (64-bit)

### Installing Raylib

1. **Download Raylib**:

   ```bash
   # Download the Windows 64-bit MSVC version
   Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_msvc16.zip' -OutFile raylib-5.0.zip
   ```

2. **Extract Raylib**:

   ```bash
   Expand-Archive -Path raylib-5.0.zip -DestinationPath raylib
   ```

3. **Verify Structure**:
   ```
   raylib/
   └── raylib-5.0_win64_msvc16/
       ├── include/     # Header files
       └── lib/         # Library files
   ```

## 🛠️ Build Instructions

### Step 1: Prepare Source Files

Ensure you have the source file `pka-workshop-raylib.c` in your project directory.

### Step 2: Compile with MSVC

Use the following command in Command Prompt or PowerShell:

```bash
cl.exe /Zi /EHsc /MD /nologo /I raylib\raylib-5.0_win64_msvc16\include pka-workshop-raylib.c /link /LIBPATH:raylib\raylib-5.0_win64_msvc16\lib raylib.lib opengl32.lib user32.lib gdi32.lib shell32.lib winmm.lib /out:pka-workshop-raylib.exe
```

#### Command Breakdown:

- `cl.exe` - Microsoft C/C++ compiler
- `/Zi` - Generate debug information
- `/EHsc` - Enable C++ exception handling (structured)
- `/MD` - Use multithreaded DLL runtime library
- `/nologo` - Suppress copyright banner
- `/I raylib\...` - Include path for raylib headers
- `pka-workshop-raylib.c` - Source file
- `/link` - Linker options
- `/LIBPATH:raylib\...` - Library search path
- `raylib.lib opengl32.lib user32.lib gdi32.lib shell32.lib winmm.lib` - Required libraries
- `/out:pka-workshop-raylib.exe` - Output executable name

### Step 3: Verify Build

Check that `pka-workshop-raylib.exe` was created:

```bash
dir pka-workshop-raylib.exe
```

## 🎯 How to Run

1. **Open Command Prompt** (recommended for best graphics support)
2. **Navigate to project directory**:
   ```bash
   cd path\to\your\project
   ```
3. **Run the game**:
   ```bash
   pka-workshop-raylib.exe
   ```

### Controls

- **W** - Move Up
- **A** - Move Left
- **S** - Move Down
- **D** - Move Right
- **Q** - Quit Game

## 📁 Project Structure

```
your-project/
├── pka-workshop-raylib.c    # Main source code
├── raylib/                   # Raylib library
│   └── raylib-5.0_win64_msvc16/
│       ├── include/
│       │   └── raylib.h
│       └── lib/
│           └── raylib.lib
├── pka-workshop-raylib.exe  # Compiled executable
└── README.md               # This documentation
```

## 🔧 Code Structure & Key Concepts

### Game Components

#### 1. Data Structures

```c
typedef struct {
    int x, y;        // Position coordinates
    int health;      // Player health
} Player;

typedef struct {
    int x, y;        // Goal position
} Goal;

typedef enum {
    PATROL,          // Enemy patrol mode
    CHASE            // Enemy chase mode
} EnemyState;

typedef struct {
    int x, y;        // Enemy position
    EnemyState state;// Current AI state
} Enemy;
```

#### 2. Game Constants

```c
#define SCREEN_WIDTH 1000    // Window width
#define SCREEN_HEIGHT 600   // Window height
#define GRID_WIDTH 20       // Game grid width
#define GRID_HEIGHT 10      // Game grid height
#define CELL_SIZE 40        // Size of each grid cell
```

#### 3. Core Functions

**Game Logic:**

- `place_random_inside()` - Randomly place entities within playable area
- `place_random_far_away()` - Place enemy away from player
- `manhattan_distance()` - Calculate distance for AI decisions

**Graphics:**

- `draw_game_board()` - Render the game grid and entities
- `draw_ui()` - Draw status information and UI elements

**Main Loop:**

```c
while (!WindowShouldClose() && game_state == 0) {
    // Handle input
    // Update game logic
    // Render graphics
}
```

### Raylib Concepts Learned

#### Window Management

```c
InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Title");
SetTargetFPS(60);  // Smooth 60 FPS gameplay
```

#### Input Handling

```c
if (IsKeyPressed(KEY_W)) {
    // Move up
}
```

#### Drawing Functions

```c
BeginDrawing();
ClearBackground(WHITE);

// Draw shapes
DrawRectangle(x, y, width, height, color);
DrawCircle(x, y, radius, color);
DrawText("Hello", x, y, fontSize, color);

EndDrawing();
```

#### Color Usage

```c
Color red = RED;           // Predefined colors
Color custom = {255, 0, 0, 255};  // RGBA values
```

## 🐛 Troubleshooting

### Common Build Errors

#### 1. "Cannot open include file: 'raylib.h'"

**Solution**: Check include path

```bash
# Ensure correct path
/I raylib\raylib-5.0_win64_msvc16\include
```

#### 2. "Unresolved external symbol"

**Solution**: Missing libraries

```bash
# Add all required libraries
raylib.lib opengl32.lib user32.lib gdi32.lib shell32.lib winmm.lib
```

#### 3. "Library machine type conflict"

**Solution**: Use correct architecture

- For 64-bit: Use `raylib-5.0_win64_msvc16`
- For 32-bit: Use `raylib-5.0_win32_msvc16`

### Runtime Issues

#### Game Window Doesn't Open

- Ensure graphics drivers are up to date
- Try running from Command Prompt instead of VS Code terminal

#### Poor Performance

- Check that `SetTargetFPS(60)` is called
- Ensure no infinite loops in game logic

## 🚀 Advanced Enhancements

### Ideas for Improvement

1. **Add sound effects** using Raylib's audio functions
2. **Implement sprite animations** for character movement
3. **Add particle effects** for collisions
4. **Create multiple levels** with different layouts
5. **Add power-ups** and collectibles

### Learning Resources

- [Raylib Official Documentation](https://www.raylib.com/)
- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Raylib Examples](https://github.com/raysan5/raylib/tree/master/examples)

## 📝 Development Notes

### Why Raylib?

- **Simple API**: Easy to learn and use
- **Cross-platform**: Works on Windows, Linux, macOS
- **No dependencies**: Just include the header and link the library
- **Active community**: Well-maintained and documented

### Game Design Decisions

- **Grid-based movement**: Simple but effective gameplay
- **AI state machine**: Enemy switches between patrol and chase
- **Health system**: Adds challenge and replayability
- **Visual feedback**: Clear win/lose states

### Performance Considerations

- **60 FPS target**: Smooth gameplay experience
- **Efficient rendering**: Only redraw when necessary
- **Simple collision detection**: Manhattan distance for AI

## 🎉 Conclusion

This project demonstrates how to create a complete 2D game using Raylib in C. You learned:

- Setting up a Raylib project
- Basic game loop structure
- Input handling and collision detection
- Simple AI implementation
- Graphics rendering and UI design

The game serves as a foundation for more complex projects. Experiment with the code, add new features, and build upon these concepts!

---

**Happy coding!** 🎮✨
