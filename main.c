// #include <stdio.h>
// #include <Windows.h>    // COORD
// #include <string.h>     // memset()
// #include <time.h>
// #include <conio.h>
// #include <stdlib.h>     // time()
//
// int width = 0, height = 0;
// #define MAP_WIDTH width
// #define MAP_HEIGHT height
// int move_count = 0;
// int stage = 0;
//
// char user[100];
//
// void print_select_user() {
//     printf("유저명을 입력하세요 : ");
//     scanf("%s", user);
// }
//
// void print_menu(int stage) {
//     char difname[6][20] = { "Easy", "Medium", "Hard", "SUPER HARD", "Admin Mode", "Ranking" };
//     system("cls");
//     //printf("Labyrinth Escape\n");
//     printf("------------------------------------------------------------------------------------------------------------------------\n");
//     printf("         _              _                   _         _    _       ______\n");
//     printf("        | |            | |                 (_)       | |  | |     |  ____|\n");
//     printf("        | |       __ _ | |__   _   _  _ __  _  _ __  | |_ | |__   | |__    ___   ___   __ _  _ __    ___\n");
//     printf("        | |      / _` || '_ \\ | | | || '__|| || '_ \\ | __|| '_ \\  |  __|  / __| / __| / _` || '_ \\  / _ \\\n");
//     printf("        | |____ | (_| || |_) || |_| || |   | || | | || |_ | | | | | |____ \\__ \\| (__ | (_| || |_) ||  __/\n");
//     printf("        |______| \\__,_||_.__/  \\__, ||_|   |_||_| |_| \\__||_| |_| |______||___/ \\___| \\__,_|| .__/  \\___|\n");
//     printf("                                __/ |                                                       | |\n");
//     printf("                               |___/                                                        |_|\n");
//     printf("------------------------------------------------------------------------------------------------------------------------\n");
//
//     for (register int i = 0; i < 6; i++)
//         printf("   %s %s\n", stage == i ? ">>" : "  ", difname[i]);
// }
//
// void print_ranking_menu(int stage) {
//     char difname[4][20] = { "Easy", "Medium", "Hard", "SUPER HARD" };
//     system("cls");
//     printf("Select a Difficulty for Ranking\n");
//     for (register int i = 0; i < 4; i++)
//         printf("   %s %s\n", stage == i ? ">>" : "  ", difname[i]);
//     printf("\nPress M to return to the main menu.\n");
// }
//
// void print_ranking(int stage) {
//     char *filename;
//     switch (stage) {
//     case 0: filename = "easy_history.txt"; break;
//     case 1: filename = "medium_history.txt"; break;
//     case 2: filename = "hard_history.txt"; break;
//     case 3: filename = "superhard_history.txt"; break;
//     default: return;
//     }
//
//     system("cls");
//     printf("Ranking for %s Stage\n", stage == 0 ? "Easy" : stage == 1 ? "Medium" : stage == 2 ? "Hard" : "SUPER HARD");
//
//     FILE *history_txt = fopen(filename, "r");
//     if (!history_txt) {
//         printf("No records found.\n");
//         printf("Press M twice to go to menu\n");
//         return;
//     }
//
//     typedef struct {
//         char name[100];
//         int score;
//     } PlayerScore;
//
//     PlayerScore scores[100];
//     int score_count = 0;
//
//     while (fscanf(history_txt, "%s %d", scores[score_count].name, &scores[score_count].score) == 2) {
//         score_count++;
//     }
//     fclose(history_txt);
//
//     if (score_count == 0) {
//         printf("No records found.\n");
//         printf("Press M twice to go to menu\n");
//         return;
//     }
//
//     for (int i = 0; i < score_count - 1; i++) {
//         for (int j = i + 1; j < score_count; j++) {
//             if (scores[i].score > scores[j].score) {
//                 PlayerScore temp = scores[i];
//                 scores[i] = scores[j];
//                 scores[j] = temp;
//             }
//         }
//     }
//
//     printf("\nTop 10 Players:\n");
//     printf("----------------------------\n");
//     printf("Rank   Name           Score\n");
//     printf("----------------------------\n");
//     for (int i = 0; i < score_count && i < 10; i++) {
//         printf("%-6d %-14s %d\n", i + 1, scores[i].name, scores[i].score);
//     }
//     printf("----------------------------\n");
//     printf("Press any key to continue.\n");
// }
//
// void setConsoleSize(int width, int height) {
//     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     COORD bufferSize = { width, height };
//     SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };
//
//     SetConsoleScreenBufferSize(hConsole, bufferSize);
//     SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
// }
//
// typedef enum _Direction {
//     DIRECTION_LEFT,
//     DIRECTION_UP,
//     DIRECTION_RIGHT,
//     DIRECTION_DOWN
// } Direction;
//
// typedef enum _MapFlag {
//     MAP_FLAG_WALL,
//     MAP_FLAG_EMPTY,
//     MAP_FLAG_VISITED,
// } MapFlag;
//
// const int DIR[4][2] = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };
//
// void shuffleArray(int array[], int size) {
//     int i, r, temp;
//     for (i = 0; i < (size - 1); ++i) {
//         r = i + (rand() % (size - i));
//         temp = array[i];
//         array[i] = array[r];
//         array[r] = temp;
//     }
// }
//
// int inRange(int y, int x) {
//     return (y < MAP_HEIGHT - 1 && y > 0) && (x < MAP_WIDTH - 1 && x > 0);
// }
//
// void generateMap(int y, int x, int map[MAP_HEIGHT][MAP_WIDTH]) {
//     int i, nx, ny;
//     int directions[4] = {
//         DIRECTION_UP,
//         DIRECTION_RIGHT,
//         DIRECTION_DOWN,
//         DIRECTION_LEFT
//     };
//
//     map[y][x] = MAP_FLAG_VISITED;
//
//     shuffleArray(directions, 4);
//
//     for (i = 0; i < 4; i++) {
//         nx = x + DIR[directions[i]][0];
//         ny = y + DIR[directions[i]][1];
//
//         if (inRange(ny, nx) && map[ny][nx] == MAP_FLAG_WALL) {
//             generateMap(ny, nx, map);
//             if (ny != y)
//                 map[(ny + y) / 2][x] = MAP_FLAG_EMPTY;
//             else
//                 map[y][(x + nx) / 2] = MAP_FLAG_EMPTY;
//             map[ny][nx] = MAP_FLAG_EMPTY;
//         }
//     }
// }
//
// COORD getRandomStartingPoint() {
//     int x = 1 + rand() % (MAP_WIDTH - 1);
//     int y = 1 + rand() % (MAP_HEIGHT - 1);
//     if (x % 2 == 0)
//         x--;
//     if (y % 2 == 0)
//         y--;
//     return (COORD) { x, y };
// }
//
//
// void printMapWithPlayer(int map[MAP_HEIGHT][MAP_WIDTH], int playerX, int playerY) {
//     system("cls");
//     for (int i = 0; i < MAP_HEIGHT; ++i) {
//         for (int j = 0; j < MAP_WIDTH; ++j) {
//             if (i == playerY && j == playerX) {
//                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
//                 printf("◆ ");
//             }
//             else if (i == MAP_WIDTH - 2  &&  j == MAP_HEIGHT - 2) {
//                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
//                 printf("▣ ");
//             }
//             else {
//                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
//                 printf("%s", map[i][j] == MAP_FLAG_WALL ? "▦ " : "  ");
//             }
//         }
//         printf("\n");
//     }
//     printf("\n%d번 움직이셨습니다\nReset Key : R\nGoto Menu : type M 2 times", move_count);
// }
//
// int main() {
//     setConsoleSize(40, 80);
//     print_select_user();
//     goto in;
//
// in:;
//     print_menu(0);
//     stage = 0;
//     move_count = 0;
//
//     while (1) {
//         int key = _getch();
//         if (key == 224) {
//             key = _getch();
//             switch (key) {
//             case 72:
//                 if (stage != 0) stage--;
//                 print_menu(stage);
//                 break;
//             case 80:
//                 if (stage != 5) stage++;
//                 print_menu(stage);
//                 break;
//             }
//         } else if (key == 13) {
//             if (stage == 5) { // Ranking 메뉴
//                 int ranking_stage = 0;
//                 while (1) {
//                     print_ranking_menu(ranking_stage);
//                     int ranking_key = _getch();
//                     if (ranking_key == 224) {
//                         ranking_key = _getch();
//                         switch (ranking_key) {
//                         case 72: if (ranking_stage != 0) ranking_stage--; break;
//                         case 80: if (ranking_stage != 3) ranking_stage++; break;
//                         }
//                     } else if (ranking_key == 13) {
//                         print_ranking(ranking_stage);
//                         _getch();
//                         break;
//                     } else if (ranking_key == 109) {
//                         goto in;
//                     }
//                 }
//             }
//             else if (stage == 4){
//
//             }
//             else {
//                 width = 21 + stage * 14;
//                 height = width;
//                 goto out;
//             }
//         } else if (key == 109) {
//             goto in;
//         }
//     }
//
// out:;
//     // 게임 플레이 코드
//     int map[MAP_HEIGHT][MAP_WIDTH];
//     COORD startPoint = getRandomStartingPoint();
//
//     int playerX = 1;
//     int playerY = 1;
//
//     srand((unsigned int)time(NULL));
//     memset(map, MAP_FLAG_WALL, sizeof(map));
//
//     generateMap(startPoint.Y, startPoint.X, map);
//
//     map[playerY][playerX] = MAP_FLAG_EMPTY;
//
//     printMapWithPlayer(map, playerX, playerY);
//
//     while (1) {
//         int key = _getch();
//         if (key == 224) {
//             key = _getch();
//             move_count++;
//             int nextX = playerX;
//             int nextY = playerY;
//
//             switch (key) {
//             case 72: nextY--; break;
//             case 80: nextY++; break;
//             case 75: nextX--; break;
//             case 77: nextX++; break;
//             }
//
//             if (map[nextY][nextX] != MAP_FLAG_WALL) {
//                 playerX = nextX;
//                 playerY = nextY;
//             } else {
//                 move_count--;
//             }
//         } else if (key == 114) {
//             playerX = 1, playerY = 1;
//             move_count = 0;
//         } else if (key == 109) {
//             goto in;
//         }
//
//         printMapWithPlayer(map, playerX, playerY);
//
//         if (playerX == MAP_WIDTH - 2 && playerY == MAP_HEIGHT - 2) {
//             char *filename;
//             switch (stage) {
//             case 0: filename = "easy_history.txt"; break;
//             case 1: filename = "medium_history.txt"; break;
//             case 2: filename = "hard_history.txt"; break;
//             case 3: filename = "superhard_history.txt"; break;
//             default: goto in;
//             }
//
//             FILE *history_txt = fopen(filename, "r");
//             if (!history_txt) {
//                 history_txt = fopen(filename, "w");
//                 fclose(history_txt);
//                 history_txt = fopen(filename, "r");
//             }
//
//             typedef struct {
//                 char name[100];
//                 int score;
//             } PlayerScore;
//
//             PlayerScore scores[100];
//             int score_count = 0;
//
//             while (fscanf(history_txt, "%s %d", scores[score_count].name, &scores[score_count].score) == 2) {
//                 score_count++;
//             }
//             fclose(history_txt);
//
//             strcpy(scores[score_count].name, user);
//             scores[score_count].score = move_count;
//             score_count++;
//
//             for (int i = 0; i < score_count - 1; i++) {
//                 for (int j = i + 1; j < score_count; j++) {
//                     if (scores[i].score > scores[j].score) {
//                         PlayerScore temp = scores[i];
//                         scores[i] = scores[j];
//                         scores[j] = temp;
//                     }
//                 }
//             }
//
//             if (score_count > 20) {
//                 score_count = 20;
//             }
//
//             history_txt = fopen(filename, "w");
//             for (int i = 0; i < score_count; i++) {
//                 fprintf(history_txt, "%s %d\n", scores[i].name, scores[i].score);
//             }
//             fclose(history_txt);
//
//             printf("\n축하합니다! 기록이 저장되었습니다.\n");
//             printf("Press any key to go to Menu");
//             _getch();
//             goto in;
//         }
//     }
//     return 0;
// }


#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

int width = 0, height = 0;
#define MAP_WIDTH width
#define MAP_HEIGHT height
int move_count = 0;
int stage = 0;

char user[100];

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void moveCursor(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void print_select_user() {
    printf("유저명을 입력하세요 : ");
    scanf("%s", user);
}

void print_menu(int stage) {
    char difname[6][20] = { "Easy", "Medium", "Hard", "SUPER HARD", "Admin Mode", "Ranking" };
    system("cls");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("         _              _                   _         _    _       ______\n");
    printf("        | |            | |                 (_)       | |  | |     |  ____|\n");
    printf("        | |       __ _ | |__   _   _  _ __  _  _ __  | |_ | |__   | |__    ___   ___   __ _  _ __    ___\n");
    printf("        | |      / _` || '_ \\ | | | || '__|| || '_ \\ | __|| '_ \\  |  __|  / __| / __| / _` || '_ \\  / _ \\\n");
    printf("        | |____ | (_| || |_) || |_| || |   | || | | || |_ | | | | | |____ \\__ \\| (__ | (_| || |_) ||  __/\n");
    printf("        |______| \\__,_||_.__/  \\__, ||_|   |_||_| |_| \\__||_| |_| |______||___/ \\___| \\__,_|| .__/  \\___|\n");
    printf("                                __/ |                                                       | |\n");
    printf("                               |___/                                                        |_|\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    for (register int i = 0; i < 6; i++)
        printf("   %s %s\n", stage == i ? ">>" : "  ", difname[i]);
}

void print_ranking_menu(int stage) {
    char difname[4][20] = { "Easy", "Medium", "Hard", "SUPER HARD" };
    system("cls");
    printf("Select a Difficulty for Ranking\n");
    for (register int i = 0; i < 4; i++)
        printf("   %s %s\n", stage == i ? ">>" : "  ", difname[i]);
    printf("\nPress M to return to the main menu.\n");
}

void print_ranking(int stage) {
    char *filename;
    switch (stage) {
    case 0: filename = "easy_history.txt"; break;
    case 1: filename = "medium_history.txt"; break;
    case 2: filename = "hard_history.txt"; break;
    case 3: filename = "superhard_history.txt"; break;
    default: return;
    }

    system("cls");
    printf("Ranking for %s Stage\n", stage == 0 ? "Easy" : stage == 1 ? "Medium" : stage == 2 ? "Hard" : "SUPER HARD");

    FILE *history_txt = fopen(filename, "r");
    if (!history_txt) {
        printf("No records found.\n");
        printf("Press M twice to go to menu\n");
        return;
    }

    typedef struct {
        char name[100];
        int score;
    } PlayerScore;

    PlayerScore scores[100];
    int score_count = 0;

    while (fscanf(history_txt, "%s %d", scores[score_count].name, &scores[score_count].score) == 2) {
        score_count++;
    }
    fclose(history_txt);

    if (score_count == 0) {
        printf("No records found.\n");
        printf("Press M twice to go to menu\n");
        return;
    }

    for (int i = 0; i < score_count - 1; i++) {
        for (int j = i + 1; j < score_count; j++) {
            if (scores[i].score > scores[j].score) {
                PlayerScore temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    printf("\nTop 10 Players:\n");
    printf("----------------------------\n");
    printf("Rank   Name           Score\n");
    printf("----------------------------\n");
    for (int i = 0; i < score_count && i < 10; i++) {
        printf("%-6d %-14s %d\n", i + 1, scores[i].name, scores[i].score);
    }
    printf("----------------------------\n");
    printf("Press any key to continue.\n");
}

void setConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { width, height };
    SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };

    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

typedef enum _Direction {
    DIRECTION_LEFT,
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN
} Direction;

typedef enum _MapFlag {
    MAP_FLAG_WALL,
    MAP_FLAG_EMPTY,
    MAP_FLAG_VISITED,
} MapFlag;

const int DIR[4][2] = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };

void shuffleArray(int array[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        int r = i + (rand() % (size - i));
        int temp = array[i];
        array[i] = array[r];
        array[r] = temp;
    }
}

int inRange(int y, int x) {
    return (y < MAP_HEIGHT - 1 && y > 0) && (x < MAP_WIDTH - 1 && x > 0);
}

void generateMap(int y, int x, int map[MAP_HEIGHT][MAP_WIDTH]) {
    int directions[4] = { DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT };
    map[y][x] = MAP_FLAG_VISITED;

    shuffleArray(directions, 4);

    for (int i = 0; i < 4; i++) {
        int nx = x + DIR[directions[i]][0];
        int ny = y + DIR[directions[i]][1];

        if (inRange(ny, nx) && map[ny][nx] == MAP_FLAG_WALL) {
            generateMap(ny, nx, map);
            if (ny != y)
                map[(ny + y) / 2][x] = MAP_FLAG_EMPTY;
            else
                map[y][(x + nx) / 2] = MAP_FLAG_EMPTY;
            map[ny][nx] = MAP_FLAG_EMPTY;
        }
    }
}

COORD getRandomStartingPoint() {
    int x = 1 + rand() % (MAP_WIDTH - 1);
    int y = 1 + rand() % (MAP_HEIGHT - 1);
    if (x % 2 == 0) x--;
    if (y % 2 == 0) y--;
    return (COORD){ x, y };
}

void printMapInitial(int map[MAP_HEIGHT][MAP_WIDTH]) {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    moveCursor(0, 0);

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (i == MAP_HEIGHT - 2 && j == MAP_WIDTH - 2) {
                SetConsoleTextAttribute(hConsole, 2);
                printf("▣ ");
            } else {
                SetConsoleTextAttribute(hConsole, 15);
                printf("%s", map[i][j] == MAP_FLAG_WALL ? "▦ " : "  ");
            }
        }
        printf("\n");
    }
}

void updatePlayerPosition(int prevX, int prevY, int newX, int newY) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD prev = { prevX * 2, prevY };
    SetConsoleCursorPosition(hConsole, prev);
    SetConsoleTextAttribute(hConsole, 15);
    printf("  ");

    COORD now = { newX * 2, newY };
    SetConsoleCursorPosition(hConsole, now);
    SetConsoleTextAttribute(hConsole, 4);
    printf("◆");
}

void printMoveCount() {
    moveCursor(0, MAP_HEIGHT + 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf("%d번 움직이셨습니다   Reset: R   Menu: M   ", move_count);
}

int main() {
    setConsoleSize(120, 50);
    hideCursor();
    print_select_user();

in:;
    print_menu(0);
    stage = 0;
    move_count = 0;

    while (1) {
        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72: if (stage != 0) stage--; print_menu(stage); break;
            case 80: if (stage != 5) stage++; print_menu(stage); break;
            }
        } else if (key == 13) {
            if (stage == 5) {
                int ranking_stage = 0;
                while (1) {
                    print_ranking_menu(ranking_stage);
                    int ranking_key = _getch();
                    if (ranking_key == 224) {
                        ranking_key = _getch();
                        switch (ranking_key) {
                        case 72: if (ranking_stage != 0) ranking_stage--; break;
                        case 80: if (ranking_stage != 3) ranking_stage++; break;
                        }
                    } else if (ranking_key == 13) {
                        print_ranking(ranking_stage);
                        _getch();
                        break;
                    } else if (ranking_key == 109) goto in;
                }
            } else if (stage == 4) {
                continue;
            } else {
                width = 21 + stage * 14;
                height = width;
                goto out;
            }
        } else if (key == 109) {
            goto in;
        }
    }

out:;
    int map[MAP_HEIGHT][MAP_WIDTH];
    COORD startPoint = getRandomStartingPoint();

    int playerX = 1, playerY = 1;

    srand((unsigned int)time(NULL));
    memset(map, MAP_FLAG_WALL, sizeof(map));
    generateMap(startPoint.Y, startPoint.X, map);
    map[playerY][playerX] = MAP_FLAG_EMPTY;

    printMapInitial(map);
    updatePlayerPosition(playerX, playerY, playerX, playerY);
    printMoveCount();

    while (1) {
        int key = _getch();
        if (key == 224) {
            key = _getch();
            int nextX = playerX, nextY = playerY;
            switch (key) {
            case 72: nextY--; break;
            case 80: nextY++; break;
            case 75: nextX--; break;
            case 77: nextX++; break;
            }

            if (map[nextY][nextX] != MAP_FLAG_WALL) {
                updatePlayerPosition(playerX, playerY, nextX, nextY);
                playerX = nextX;
                playerY = nextY;
                move_count++;
                printMoveCount();
            }
        } else if (key == 114) {
            updatePlayerPosition(playerX, playerY, 1, 1);
            playerX = 1; playerY = 1; move_count = 0;
            printMoveCount();
        } else if (key == 109) {
            goto in;
        }

        if (playerX == MAP_WIDTH - 2 && playerY == MAP_HEIGHT - 2) {
            char *filename;
            switch (stage) {
            case 0: filename = "easy_history.txt"; break;
            case 1: filename = "medium_history.txt"; break;
            case 2: filename = "hard_history.txt"; break;
            case 3: filename = "superhard_history.txt"; break;
            default: goto in;
            }

            FILE *history_txt = fopen(filename, "r");
            if (!history_txt) {
                history_txt = fopen(filename, "w");
                fclose(history_txt);
                history_txt = fopen(filename, "r");
            }

            typedef struct { char name[100]; int score; } PlayerScore;
            PlayerScore scores[100];
            int score_count = 0;
            while (fscanf(history_txt, "%s %d", scores[score_count].name, &scores[score_count].score) == 2)
                score_count++;
            fclose(history_txt);

            strcpy(scores[score_count].name, user);
            scores[score_count].score = move_count;
            score_count++;

            for (int i = 0; i < score_count - 1; i++) {
                for (int j = i + 1; j < score_count; j++) {
                    if (scores[i].score > scores[j].score) {
                        PlayerScore temp = scores[i];
                        scores[i] = scores[j];
                        scores[j] = temp;
                    }
                }
            }

            if (score_count > 20) score_count = 20;

            history_txt = fopen(filename, "w");
            for (int i = 0; i < score_count; i++)
                fprintf(history_txt, "%s %d\n", scores[i].name, scores[i].score);
            fclose(history_txt);

            moveCursor(0, MAP_HEIGHT + 3);
            printf("\n축하합니다! 기록이 저장되었습니다.\nPress any key to go to Menu");
            _getch();
            goto in;
        }
    }

    return 0;
}
