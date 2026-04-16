#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    *x = rand() % 8 + 1;
    *y = rand() % 8 + 1;
}

void place_random_far_away(int *x, int *y, int px, int py)
{
    do
    {
        place_random_inside(x, y);
    } while (manhattan_distance(*x, *y, px, py) < 5);
}

void init_matrix(char matrix[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if ((i == 0 || i == 9) && (j == 0 || j == 9))
            {
                matrix[i][j] = '#';
            }
            else if (i == 0 || i == 9)
            {
                matrix[i][j] = '-';
            }
            else if (j == 0 || j == 9)
            {
                matrix[i][j] = '|';
            }
            else
            {
                matrix[i][j] = '.';
            }
        }
    }
}

void draw_matrix(char matrix[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
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
    char matrix[10][10];
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

    char move;

    while (1)
    {
        init_matrix(matrix);
        matrix[p.y][p.x] = 'P';
        matrix[g.y][g.x] = 'G';
        matrix[e.y][e.x] = 'E';

        draw_matrix(matrix);
        printf("Player is at col=%d, row=%d with health %d\n", p.x, p.y, p.health);
        printf("Goal is at col=%d, row=%d\n", g.x, g.y);
        printf("Enemy is at col=%d, row=%d (%s)\n", e.x, e.y,
               e.state == CHASE ? "CHASE" : "PATROL");
        printf("Enter move (W/A/S/D, Q to quit): ");

        if (scanf(" %c", &move) != 1)
        {
            break;
        }

        if (move == 'Q' || move == 'q')
        {
            printf("Quitting game.\n");
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
            if (p.x < 8)
            {
                p.x++;
            }
            break;
        default:
            printf("Invalid move '%c'. Player did not move.\n", move);
            break;
        }

        if (p.x == g.x && p.y == g.y)
        {
            printf("You reached the goal! You win!\n");
            break;
        }

        if (p.x == e.x && p.y == e.y)
        {
            p.health -= 10;
            printf("Enemy caught you! -10 health. Respawning player and enemy.\n");
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
            if (next_x >= 1 && next_x <= 8 && next_y >= 1 && next_y <= 8)
            {
                e.x = next_x;
                e.y = next_y;
            }
        }

        if (e.x == p.x && e.y == p.y)
        {
            p.health -= 10;
            printf("Enemy caught you! -10 health. Respawning player and enemy.\n");
            respawn_player_and_enemy(&p, &e, &g);
            continue;
        }

        printf("\n");
    }

    return 0;
}
