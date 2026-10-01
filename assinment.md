# Assignment: Build a 2D Windows Game in C++

## Objective

Create a small 2D game in **C++** that runs as a native **Windows desktop application**. The game should use the **Win32 API** to create and manage the application window, process keyboard input, draw game objects, and control the game loop.

You may use GDI/GDI+ for rendering. Do not use a game engine such as Unity or Unreal.

## Game Concept

Create a game called **“Dodge the Falling Blocks.”**

The player controls a small character at the bottom of the screen. Blocks continuously fall from the top of the window. The player must move left and right to avoid them and survive for as long as possible.

The game ends when the player collides with a falling block.

## Requirements

### 1. Windows Application

Your program must:

* Create a Windows desktop window using the Win32 API.
* Set an appropriate window title, such as `Dodge the Falling Blocks`.
* Have a fixed or controlled window size.
* Process the Windows message loop correctly.
* Close cleanly when the user closes the window.

### 2. Player

The player must:

* Be displayed near the bottom of the window.
* Move left and right using the **Arrow Keys** or **A/D**.
* Stay inside the boundaries of the game window.
* Have a clearly defined collision area.

### 3. Falling Obstacles

The game must:

* Create falling blocks at random horizontal positions.
* Move the blocks downward continuously.
* Remove blocks once they leave the bottom of the screen.
* Generate new blocks during the game.

The falling speed should gradually increase as the player survives longer.

### 4. Collision Detection

Implement collision detection between the player and falling blocks.

When a collision occurs:

* The game should stop.
* A **Game Over** message should be displayed.
* The player's final score should be shown.
* The player should be able to restart the game.

### 5. Score

The player receives points for surviving.

For example:

* +1 point every second survived, or
* +10 points for every obstacle successfully avoided.

The current score must be visible in the game window.

### 6. Game Loop

Implement a game loop that:

1. Processes Windows messages.
2. Reads player input.
3. Updates object positions.
4. Checks collisions.
5. Updates the score.
6. Redraws the game.

Use an appropriate Windows timing mechanism such as `GetTickCount`, `GetTickCount64`, or a timer.

### 7. Rendering

Draw at least:

* A background.
* The player.
* Falling obstacles.
* The current score.
* A Game Over screen.

The graphics do not need to be advanced. Simple rectangles, circles, or basic GDI shapes are acceptable.

## Suggested Classes

Your program should use object-oriented C++.

A possible structure is:

```cpp
class Player
{
public:
    int x;
    int y;
    int width;
    int height;
    int speed;

    void update();
    void draw(HDC hdc);
};

class Obstacle
{
public:
    int x;
    int y;
    int width;
    int height;
    int speed;

    void update();
    void draw(HDC hdc);
};

class Game
{
public:
    void initialize();
    void update();
    void draw(HDC hdc);
    void restart();
    bool checkCollision();

private:
    Player player;
    std::vector<Obstacle> obstacles;
    int score;
    bool gameOver;
};
```

You may use a different class structure if it provides the same functionality.

## Keyboard Controls

| Key             | Action                  |
| --------------- | ----------------------- |
| Left Arrow / A  | Move left               |
| Right Arrow / D | Move right              |
| R               | Restart after Game Over |
| Escape          | Exit the game           |

## Minimum Technical Requirements

Your project must demonstrate:

* C++ classes and objects.
* `std::vector` or another STL container.
* Functions and appropriate encapsulation.
* Win32 window creation.
* A Windows message procedure (`WndProc`).
* Keyboard input handling.
* Basic 2D rendering.
* Collision detection.
* Random number generation.
* A game/update loop.

## Optional Extensions

For additional marks, implement one or more of the following:

* A start menu.
* Multiple obstacle types.
* Increasing difficulty.
* High-score tracking.
* Sound effects.
* Background music.
* Power-ups.
* Lives or health.
* A pause system.
* Different game levels.
* A graphical main menu.
* Saving the high score to a file.

## Deliverables

Submit:

1. All `.cpp` and `.h` source files.
2. The Visual Studio project/solution.
3. A compiled `.exe`.
4. A short README explaining:

    * How to compile the project.
    * How to play the game.
    * The controls.
    * The main classes used.
    * Any additional features you implemented.

## Assessment

| Category                                 |  Points |
| ---------------------------------------- | ------: |
| Windows application and message handling |      15 |
| Game loop and timing                     |      15 |
| Player movement                          |      10 |
| Obstacle generation and movement         |      15 |
| Collision detection                      |      15 |
| Score and Game Over system               |      10 |
| Object-oriented C++ design               |      10 |
| Code quality and documentation           |      10 |
| **Total**                                | **100** |

## Challenge

Your final game should feel like a complete, playable Windows application rather than a collection of separate programming exercises.

The main goal is to demonstrate that you can combine **C++ programming, object-oriented design, Windows API programming, input handling, graphics, and basic game logic** into one working application.
