#include <iostream>
#include <raylib.h>

#define ballRadius 20

bool checkGameOver(int ballY, int screenHeight) {
    return (ballY + ballRadius >= screenHeight);
}

bool checkVictory(bool tiles[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (tiles[i][j]) {
                return false;  
            }
        }
    }
    return true;  
}

void checkCollisionTile(int ballX, int ballY, int &dirX, int &dirY, int tileX, int tileY, int tileWidth, int tileHeight, bool &isActive) {
    if (isActive && ballX + ballRadius >= tileX && ballX - ballRadius <= tileX + tileWidth &&
        ballY + ballRadius >= tileY && ballY - ballRadius <= tileY + tileHeight) {
        dirY = -dirY; 
        isActive = false; 
    }
}

void checkColisionRect(int ballX, int ballY, int &rectX, int &rectY, int &dirX, int &dirY, int rectWidth) {
    if (ballX + ballRadius >= rectX && ballX - ballRadius <= rectX + rectWidth &&
        ballY + ballRadius >= rectY && ballY - ballRadius <= rectY + 20) {

        int colisionPoint = ballX - rectX;

        if (colisionPoint < rectWidth / 3) {
            dirX = -1; 
        } else if (colisionPoint > (rectWidth * 2 / 3)) {
            dirX = 1;   
        }

        dirY = -dirY; 
    }
}

void drawTiles(int colunas, int &ballX, int &ballY, int &dirX, int &dirY, bool tiles[5][5]) {
    int tileWidth = 176;
    int tileHeight = 20;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < colunas; j++) {
            int tileX = (i * 220) + 10;
            int tileY = (j * 30) + 5;

            if (tiles[i][j]) { 
                DrawRectangle(tileX, tileY, tileWidth, tileHeight, WHITE);
                checkCollisionTile(ballX, ballY, dirX, dirY, tileX, tileY, tileWidth, tileHeight, tiles[i][j]);
            }
        }
    }
}

void CheckCollision(int &ballX, int &ballY, int screenWidth, int screenHeight, int &dirX, int &dirY) {
    if (ballY - ballRadius <= 0) {
        ballY = ballRadius;
        dirY = 1;
    }
    
    if (ballX - ballRadius <= 0) {
        ballX = ballRadius;
        dirX = 1;  
    }
    if (ballX + ballRadius >= screenWidth) {
        ballX = screenWidth - ballRadius;
        dirX = -1; 
    }
}

int main() {
    const int screenWidth = 1080;
    const int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "Bola");
    SetTargetFPS(60);

    int ballX = screenWidth / 2;
    int ballY = screenHeight / 2;
    int dirX = 0; 
    int dirY = 1;  
    int ballSpeed = 5;  

    int rectX = screenWidth / 2 - 60;
    int rectY = (screenHeight / 2) + 100;

    bool tiles[5][5];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tiles[i][j] = true; 
        }
    }

    bool isGameOver = false;
    bool isWin = false;
    bool ballStarted = false;  

    while (!WindowShouldClose()) {
        if (isGameOver || isWin) {
            BeginDrawing();
            ClearBackground(GRAY);
            DrawRectangle(screenWidth / 2 - 400, screenHeight / 2 - 200 , 800, 400, BLACK);

            if (isWin) {
                DrawText("WIN!", screenWidth / 2 - 200 + 120, screenHeight / 2 - 100 + 50, 80, GREEN);
            } else {
                DrawText("GameOver", screenWidth / 2 - 200 + 20, screenHeight / 2 - 100 + 50, 80, GREEN);
            }

            DrawText("Press R to restart", screenWidth / 2 - 200 + 10, screenHeight / 2 - 100 + 180, 40, RED);

            if (IsKeyDown(KEY_R)) {
                ballX = screenWidth / 2;
                ballY = screenHeight / 2;
                dirX = 0;
                dirY = 1;
                rectX = screenWidth / 2 - 60;
                rectY = (screenHeight / 2) + 100;
                ballStarted = false;  

                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 5; j++) {
                        tiles[i][j] = true;
                    }
                }

                isGameOver = false;
                isWin = false;  
            }

            EndDrawing();
        } else {

            if (IsKeyDown(KEY_D)) {
                rectX += 10;
                ballStarted = true;  
            }
            if (IsKeyDown(KEY_A)) {
                rectX -= 10;
                ballStarted = true;  
            }

            if (rectX < 0) {
                rectX = 0;
            }
            if (rectX >= screenWidth - 120) {
                rectX = screenWidth - 120;
            }

            if (ballStarted) {
                ballX += dirX * ballSpeed;
                ballY += dirY * ballSpeed;
            }

            CheckCollision(ballX, ballY, screenWidth, screenHeight, dirX, dirY);
            checkColisionRect(ballX, ballY, rectX, rectY, dirX, dirY, 120);

            if (checkGameOver(ballY, screenHeight)) {
                isGameOver = true;
            }

            if (checkVictory(tiles)) {
                isWin = true;
            }

            BeginDrawing();
            ClearBackground(GRAY);

            drawTiles(5, ballX, ballY, dirX, dirY, tiles);
            DrawRectangle(rectX, rectY, 120, 20, WHITE);
            DrawCircle(ballX, ballY, ballRadius, WHITE);

            EndDrawing();
        }
    }

    CloseWindow();

    return 0;
}
