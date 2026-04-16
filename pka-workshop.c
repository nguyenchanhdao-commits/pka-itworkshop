#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>

typedef struct
{
    int x; /* column index */
    int y; /* row index */
    int health;
} Player;

typedef struct
{
    int x; /* column index */
    int y; /* row index */
} Goal;

typedef enum
{
    PATROL,
    CHASE
} EnemyState;

typedef struct
{
    int x; /* column index */
    int y; /* row index */
    EnemyState state;
} Enemy;

int manhattan_distance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

void place_random_inside(int *x, int *y)
{
    *x = rand() % 18 + 1;
    *y = rand() % 8 + 1;
}

void place_random_far_away(int *x, int *y, int px, int py)
{
    do
    {
        place_random_inside(x, y);
    } while (manhattan_distance(*x, *y, px, py) < 5);
}

void init_matrix(char matrix[10][20])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (i == 0 || i == 9 || j == 0 || j == 19)
            {
                matrix[i][j] = '#';
            }
            else
            {
                matrix[i][j] = ' ';
            }
        }
    }
}

void draw_borders()
{
    attron(COLOR_PAIR(1));
    for (int i = 0; i < 20; i++)
    {
        mvaddch(0, i, '-');
        mvaddch(11, i, '-');
    }
    for (int i = 0; i < 12; i++)
    {
        mvaddch(i, 0, '|');
        mvaddch(i, 19, '|');
    }
    mvaddch(0, 0, '+');
    mvaddch(0, 19, '+');
    mvaddch(11, 0, '+');
    mvaddch(11, 19, '+');
    attroff(COLOR_PAIR(1));
}

void draw_title()
{
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(0, 1, "== MARIO ADVENTURE ==");
    attroff(COLOR_PAIR(2) | A_BOLD);
}

void draw_matrix(char matrix[10][20])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            char c = matrix[i][j];
            if (c == '#')
            {
                attron(COLOR_PAIR(1)); // Blue for walls
            }
            else if (c == 'M')
            {
                attron(COLOR_PAIR(2)); // Red for Mario
            }
            else if (c == 'F')
            {
                attron(COLOR_PAIR(3)); // Green for Flag
            }
            else if (c == 'G')
            {
                attron(COLOR_PAIR(4)); // Yellow for Goomba
            }
            else
            {
                attron(COLOR_PAIR(5)); // White for empty
            }
            mvaddch(i + 1, j, c);
        }
    }
}

void respawn_player_and_enemy(Player *p, Enemy *e, const Goal *g)
{
    place_random_inside(&p->x, &p->y);
    while (p->x == g->x && p->y == g->y)
    {
        place_random_inside(&p->x, &p->y);
    }
    place_random_far_away(&e->x, &e->y, p->x, p->y);
    while ((e->x == g->x && e->y == g->y) || (e->x == p->x && e->y == p->y))
    {
        place_random_far_away(&e->x, &e->y, p->x, p->y);
    }
    e->state = PATROL;
}

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);   // Walls
    init_pair(2, COLOR_RED, COLOR_BLACK);    // Mario
    init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Flag
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Goomba
    init_pair(5, COLOR_WHITE, COLOR_BLACK);  // Empty

    char matrix[10][20];
    srand((unsigned)time(NULL));

    Player p;
    place_random_inside(&p.x, &p.y);
    p.health = 100;

    Goal g;
    place_random_inside(&g.x, &g.y);
    while (g.x == p.x && g.y == p.y)
    {
        place_random_inside(&g.x, &g.y);
    }

    Enemy e;
    place_random_far_away(&e.x, &e.y, p.x, p.y);
    while ((e.x == g.x && e.y == g.y) || (e.x == p.x && e.y == p.y))
    {
        place_random_far_away(&e.x, &e.y, p.x, p.y);
    }
    e.state = PATROL;

    int move;

    while (1)
    {
        erase();
        init_matrix(matrix);
        matrix[p.y][p.x] = 'M';
        matrix[g.y][g.x] = 'F';
        matrix[e.y][e.x] = 'G';

        draw_title();
        draw_borders();
        draw_matrix(matrix);
        mvprintw(12, 0, "| Mario at (%2d,%d) HP:%3d", p.x, p.y, p.health);
        mvprintw(13, 0, "| Flag at  (%2d,%d)     ", g.x, g.y);
        mvprintw(14, 0, "| Goomba at (%2d,%d) [%s] ", e.x, e.y,
                 e.state == CHASE ? "CHASE" : "PATROL");
        mvprintw(15, 0, "| Move: W(Up) A(Left) S(Down) D(Right) | Q(Quit)");
        mvprintw(16, 0, "+--------------------------------------------+");
        refresh();

        move = getch();

        if (move == 'Q' || move == 'q')
        {
            erase();
            draw_title();
            mvprintw(10, 3, "Thanks for playing Mario!");
            mvprintw(12, 5, "See you next time!");
            refresh();
            getch();
            break;
        }

        switch (move)
        {
        case 'W':
        case 'w':
            if (p.y > 1)
            {
                p.y--;
            }
            break;
        case 'S':
        case 's':
            if (p.y < 8)
            {
                p.y++;
            }
            break;
        case 'A':
        case 'a':
            if (p.x > 1)
            {
                p.x--;
            }
            break;
        case 'D':
        case 'd':
            if (p.x < 18)
            {
                p.x++;
            }
            break;
        default:
            attron(COLOR_PAIR(4));
            mvprintw(17, 0, "Invalid move! Try W/A/S/D");
            attroff(COLOR_PAIR(4));
            refresh();
            getch();
            break;
        }

        if (p.x == g.x && p.y == g.y)
        {
            erase();
            draw_title();
            draw_borders();
            draw_matrix(matrix);
            mvprintw(17, 0, "+========================================+");
            mvprintw(18, 0, "| CONGRATULATIONS! Mario reached Flag! |");
            mvprintw(19, 0, "+========================================+");
            refresh();
            getch();
            break;
        }

        if (p.x == e.x && p.y == e.y)
        {
            p.health -= 10;
            erase();
            draw_title();
            draw_borders();
            draw_matrix(matrix);
            mvprintw(17, 0, "+========================================+");
            mvprintw(18, 0, "| Goomba hit! -10 Health! Respawning... |");
            mvprintw(19, 0, "+========================================+");
            refresh();
            getch();
            respawn_player_and_enemy(&p, &e, &g);
            continue;
        }

        int dist = manhattan_distance(e.x, e.y, p.x, p.y);
        e.state = dist < 4 ? CHASE : PATROL;

        if (e.state == CHASE)
        {
            int dx = p.x - e.x;
            int dy = p.y - e.y;
            if (abs(dx) > abs(dy))
            {
                e.x += (dx > 0) ? 1 : -1;
            }
            else if (dy != 0)
            {
                e.y += (dy > 0) ? 1 : -1;
            }
        }
        else
        {
            int dir = rand() % 4;
            int next_x = e.x;
            int next_y = e.y;
            switch (dir)
            {
            case 0:
                next_x++;
                break;
            case 1:
                next_x--;
                break;
            case 2:
                next_y++;
                break;
            case 3:
                next_y--;
                break;
            }
            if (next_x >= 1 && next_x <= 18 && next_y >= 1 && next_y <= 8)
            {
                e.x = next_x;
                e.y = next_y;
            }
        }

        if (e.x == p.x && e.y == p.y)
        {
            p.health -= 10;
            erase();
            draw_title();
            draw_borders();
            draw_matrix(matrix);
            mvprintw(17, 0, "+========================================+");
            mvprintw(18, 0, "| Goomba hit! -10 Health! Respawning... |");
            mvprintw(19, 0, "+========================================+");
            refresh();
            getch();
            respawn_player_and_enemy(&p, &e, &g);
            continue;
        }
    }

    endwin();
    return 0;
}
