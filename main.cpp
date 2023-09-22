/******************************************************************** 
* Clone of the Classic Game Pong, (originally created by Allan Alcorn.)
* Developed with reverance and love, at 2 am during a bout of insomnia
* by a tired person, occasionally going by the name of Althea Hicks.
********************************************************************/

#include "raylib.h"

struct Ball {
    
	float x, y;
	float speed_x, speed_y;
	float radius;

	void Draw(){
		DrawCircle( (int) x, (int) y, radius, RAYWHITE);
	}
};

struct Paddle {
    
	float x, y;
	float speed;
	float width = 10; 
	float height = 100;

	Rectangle GetRect() {

		return Rectangle{ x - width / 2, y - height / 2, width, height };
	}

	void Draw() {
		DrawRectangleRec( GetRect(), RAYWHITE );
	}
};

int main(void) {

	InitWindow(800, 600, "Pong");

	SetWindowState(FLAG_VSYNC_HINT);	// sets framerate to your monitor's refresh rate

	Ball pong_ball;
	pong_ball.x = GetScreenWidth() / 2;
	pong_ball.y = GetScreenHeight() / 2;
	pong_ball.radius = 10;
	pong_ball.speed_x = 200;
	pong_ball.speed_y = 200;

	Paddle left_paddle;
	left_paddle.x = 50;
	left_paddle.y = GetScreenHeight() / 2;
	left_paddle.speed = 500;

	Paddle right_paddle;
	right_paddle.x = GetScreenWidth() - 50;
	right_paddle.y = GetScreenHeight() / 2;
	right_paddle.speed = 500;

	const char* winner_text = nullptr;
    const char *space_bar_restart_text = nullptr;
    space_bar_restart_text = "Press Space Bar to Play Again";
    int space_bar_restart_length = MeasureText(space_bar_restart_text, 25);  
    
    int MAX_MENU_ITEMS = 2;
    const char* menuItems[MAX_MENU_ITEMS] = {
    "Start Game",
    "Exit"
    };
    
    int selectedItem = 0;
    bool start_game = 0;


	// main game loop
	while(!WindowShouldClose()) {

        // Input handling for start menu
        if (IsKeyPressed(KEY_DOWN)) {
            selectedItem = (selectedItem + 1) % MAX_MENU_ITEMS;
        }
        else if (IsKeyPressed(KEY_UP)) {
            selectedItem = (selectedItem - 1 + MAX_MENU_ITEMS) % MAX_MENU_ITEMS;
        }	
        else if (IsKeyPressed(KEY_ENTER)) {
            // Execute the selected menu item's action
            switch (selectedItem) {
                case 0:
                    // Start Game
                    start_game = 1;
                    break;
                case 1:
                    start_game = 0;
                    CloseWindow();
                    break;
            }
        }
       
        if(start_game == 1){

            pong_ball.x += pong_ball.speed_x * GetFrameTime();
            pong_ball.y += pong_ball.speed_y * GetFrameTime();

            // ball movement
            if (pong_ball.y > GetScreenHeight()) {
                pong_ball.y = GetScreenHeight();		// setting the position prevents a bug that causes ball to get stuck at side of screen
                pong_ball.speed_y *= -1;
            }
            if (pong_ball.y < 1) {
                pong_ball.y = 0;
                pong_ball.speed_y *= -1;
            }

            // controlling paddle
            if (IsKeyDown(KEY_W)) {
                left_paddle.y -= left_paddle.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_S)) {
                left_paddle.y += left_paddle.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_UP)) {
                right_paddle.y -= left_paddle.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_DOWN)) {
                right_paddle.y += left_paddle.speed * GetFrameTime();
            }
            
            // Creating top and bottom border for paddles
            if(right_paddle.y < 0){
                right_paddle.y = 0;
            }
            if(left_paddle.y < 0){
                left_paddle.y = 0;
            }
            if(right_paddle.y > GetScreenHeight()){
                right_paddle.y  = GetScreenHeight();
            }
            if(left_paddle.y > GetScreenHeight()){
                left_paddle.y  = GetScreenHeight();
            }

            // Collison Detection
            if (CheckCollisionCircleRec(Vector2{ pong_ball.x,pong_ball.y }, pong_ball.radius, left_paddle.GetRect())) {

                if (pong_ball.speed_x < 0) {		// check to make sure the ball is going left, preventing bug where it rubs against side of paddle
                    pong_ball.speed_x *= -1.07f;
                    pong_ball.speed_y = (pong_ball.y - left_paddle.y) / (left_paddle.height / 2) * pong_ball.speed_x;
                }
            }
            if (CheckCollisionCircleRec(Vector2{ pong_ball.x,pong_ball.y }, pong_ball.radius, right_paddle.GetRect())) {

                if (pong_ball.speed_x > 0) {
                    pong_ball.speed_x *= -1.07f;
                    pong_ball.speed_y = (pong_ball.y - right_paddle.y) / (right_paddle.height / 2) * -pong_ball.speed_x;
                }
            }

            if (pong_ball.x < 0) {
                winner_text = "Right Player Wins!";
            }
            if (pong_ball.x > GetScreenWidth()) {
                winner_text = "Left Player Wins!";
            }
            
            // Reset Game if space bar is pressed
            if(winner_text && IsKeyPressed(KEY_SPACE)){
                
                pong_ball.x = GetScreenWidth() / 2;
                pong_ball.y = GetScreenHeight() / 2;
                pong_ball.speed_x = 200;
                pong_ball.speed_y = 200;
                winner_text = nullptr;
                start_game = 0;
                
            }
            if(start_game == 1){
                
                BeginDrawing();
                    ClearBackground(DARKGRAY);

                    pong_ball.Draw();
                    left_paddle.Draw();
                    right_paddle.Draw();

                    if (winner_text) {
                        DrawText(winner_text, GetScreenWidth()/ 2 - MeasureText(winner_text,60)/ 2, GetScreenHeight() / 2 - 30, 60, SKYBLUE);
                        DrawText(space_bar_restart_text, GetScreenWidth()/ 2 - space_bar_restart_length/ 2, GetScreenHeight()/ 2 + 45, 25, GRAY);
                    }

                    DrawFPS(10, 10);
                EndDrawing();
            }
        }

        if(start_game == 0){
            BeginDrawing();
                // Render menu items
                ClearBackground(DARKGRAY);
                DrawText("PONG", GetScreenWidth()/ 2 - MeasureText("PONG",80)/ 2, GetScreenHeight() / 2 - 150, 80, RAYWHITE);
                for (int i = 0; i < MAX_MENU_ITEMS; i++) {
                    if (i == selectedItem) {
                        DrawText(menuItems[i],  GetScreenWidth()/ 2 - MeasureText("PONG",80)/ 2, 260 + i * 40, 20, SKYBLUE);
                    }
                    else {
                        DrawText(menuItems[i],  GetScreenWidth()/ 2 - MeasureText("PONG",80)/ 2, 260 + i * 40, 20, BLACK);
                    }
                }
            EndDrawing();
        }
    }

	CloseWindow();
	return 0;
}