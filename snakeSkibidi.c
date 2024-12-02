#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 50
#define HEIGHT 20

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[100];
    int length;
    char direction; // 'w', 'a', 's', 'd'
} Snake;

Point food;
Snake snake;

void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    snake.length = 3;
    snake.body[0] = (Point){WIDTH / 2, HEIGHT / 2};
    snake.body[1] = (Point){WIDTH / 2 - 1, HEIGHT / 2};
    snake.body[2] = (Point){WIDTH / 2 - 2, HEIGHT / 2};
    snake.direction = 'd';

    srand(time(NULL));
    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;
}

void draw_game() {
    clear();

    for (int i = 0; i < WIDTH; i++) mvprintw(0, i, "#");
    for (int i = 0; i < WIDTH; i++) mvprintw(HEIGHT - 1, i, "#");
    for (int i = 1; i < HEIGHT - 1; i++) mvprintw(i, 0, "#");
    for (int i = 1; i < HEIGHT - 1; i++) mvprintw(i, WIDTH - 1, "#");

    for (int i = 0; i < snake.length; i++) {
        mvprintw(snake.body[i].y, snake.body[i].x, "🚽");
    }

    mvprintw(food.y, food.x, "🤖");

    refresh();
}

void update_snake() {
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }

    switch (snake.direction) {
        case 'w': snake.body[0].y--; break;
        case 's': snake.body[0].y++; break;
        case 'a': snake.body[0].x--; break;
        case 'd': snake.body[0].x++; break;
    }

    if (snake.body[0].x == 0 || snake.body[0].x == WIDTH - 1 ||
        snake.body[0].y == 0 || snake.body[0].y == HEIGHT - 1) {
        endwin();
        printf("Game Over! Robots win!\n");
        exit(0);
    }

    for (int i = 1; i < snake.length; i++) {
        if (snake.body[0].x == snake.body[i].x &&
            snake.body[0].y == snake.body[i].y) {
            endwin();
            printf("Game Over! Robots win!\n");
            exit(0);
        }
    }

    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        snake.length++;
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
    }
}

void handle_input() {
    int ch = getch();
    if (ch == 'w' && snake.direction != 's') snake.direction = 'w';
    if (ch == 's' && snake.direction != 'w') snake.direction = 's';
    if (ch == 'a' && snake.direction != 'd') snake.direction = 'a';
    if (ch == 'd' && snake.direction != 'a') snake.direction = 'd';
}

int main() {
    init_game();

    while (1) {
        draw_game();
        handle_input();
        update_snake();
        napms(100); 
    }

    endwin();
    return 0;
}
