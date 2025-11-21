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
int timer_on = 0;
char user[100];

DWORD WINAPI GraphCalcThread(LPVOID lpParam);
int bfsGraphShortestPath(int map[MAP_HEIGHT][MAP_WIDTH], int sx, int sy, int ex, int ey);
int shortestPathResult = -1;
int graphCalcFinished = 0;
double graphCalcTime = 0.0;

DWORD WINAPI GraphCalcThread(LPVOID lpParam) {
    clock_t start = clock();   // 시간 측정 시작

    int (*map)[MAP_WIDTH] = (int(*)[MAP_WIDTH])lpParam;

    shortestPathResult = bfsGraphShortestPath(
        map,
        1, 1,
        MAP_WIDTH - 2, MAP_HEIGHT - 2
    );

    clock_t end = clock();     // 시간 측정 종료
    graphCalcTime = (double)(end - start) / CLOCKS_PER_SEC;

    graphCalcFinished = 1;
    return 0;
}


// ? 콘솔 커서 숨기기
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// ? 콘솔 커서 이동
void moveCursor(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ? 콘솔 폰트 크기 변경 함수 (추가됨)
void setConsoleFontSize(int sizeY) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };

    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    cfi.dwFontSize.Y = sizeY;        // 세로 폰트 크기
    cfi.dwFontSize.X = sizeY / 2;    // 가로 비율 (가독성 유지)
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

void print_select_user() {
    printf("유저명을 입력하세요 : ");
    scanf("%s", user);
}

void print_menu(int stage) {
    char difname[6][120] = { "Easy", "Medium", "Hard", "SUPER HARD", "IQ test(for fun, not Verified, Don\'t believe it)", "Ranking" };
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

void print_time_menu(int stage) {
    char difname[4][20] = { "Easy", "Medium", "Hard", "SUPER HARD" };
    system("cls");
    printf("Select a Difficulty for IQ Test\n");
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
                printf("%s", map[i][j] == MAP_FLAG_WALL ? "■ " : "  ");
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
    moveCursor(0, MAP_HEIGHT + 2);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf("%d번 움직이셨습니다   Reset: R   Menu: M   ", move_count);
}

// 그래프 기반 BFS 최단 경로 탐색 (0.3초 텀 적용 버전)
int bfsGraphShortestPath(int map[MAP_HEIGHT][MAP_WIDTH], int sx, int sy, int ex, int ey) {

    int totalNodes = MAP_HEIGHT * MAP_WIDTH;

    // 인접 리스트
    int *adj[totalNodes];
    int adjSize[totalNodes];

    // 초기화
    for (int i = 0; i < totalNodes; i++) {
        adj[i] = (int*)malloc(sizeof(int) * 4);
        adjSize[i] = 0;
    }

    // 그래프 구성
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == MAP_FLAG_WALL) continue;

            int id = y * MAP_WIDTH + x;

            int dx[4] = { 1, -1, 0, 0 };
            int dy[4] = { 0, 0, 1, -1 };

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && ny >= 0 && nx < MAP_WIDTH && ny < MAP_HEIGHT) {
                    if (map[ny][nx] != MAP_FLAG_WALL) {
                        int nid = ny * MAP_WIDTH + nx;
                        adj[id][adjSize[id]++] = nid;
                    }
                }
            }
        }
    }

    // BFS 준비
    int startID = sy * MAP_WIDTH + sx;
    int endID   = ey * MAP_WIDTH + ex;

    int *queue = (int*)malloc(sizeof(int) * totalNodes);
    int front = 0, rear = 0;

    int visited[totalNodes];
    int dist[totalNodes];
    memset(visited, 0, sizeof(visited));
    memset(dist, -1, sizeof(dist));

    queue[rear++] = startID;
    visited[startID] = 1;
    dist[startID] = 0;

    while (front < rear) {

        // ------------------------------
        // ? 탐색 딜레이 추가
        // ------------------------------
        Sleep(15);
        // ------------------------------

        int cur = queue[front++];

        if (cur == endID) {
            for (int i = 0; i < totalNodes; i++) free(adj[i]);
            free(queue);
            return dist[cur];
        }

        for (int i = 0; i < adjSize[cur]; i++) {
            int next = adj[cur][i];
            if (!visited[next]) {
                visited[next] = 1;
                dist[next] = dist[cur] + 1;
                queue[rear++] = next;
            }
        }
    }

    for (int i = 0; i < totalNodes; i++) free(adj[i]);
    free(queue);

    return -1;
}


int main() {
    time_t startTime = 0;
    setConsoleSize(120, 50);
    hideCursor();
    print_select_user();


in:;
    setConsoleFontSize(4); // ? 메뉴 진입 시 폰트 크기 복원
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
                        timer_on = 0;
                        _getch();
                        break;
                    } else if (ranking_key == 109) goto in;
                }
            } else if (stage == 4) {
                int time_stage = 0;
                while (1) {
                    print_time_menu(time_stage);
                    int time_key = _getch();
                    if (time_key == 224) {
                        time_key = _getch();
                        switch (time_key) {
                            case 72: if (time_stage != 0) time_stage--; break;
                            case 80: if (time_stage != 3) time_stage++; break;
                        }
                    } else if (time_key == 13) {
                        width = 21 + time_stage * 14;
                        height = width;
                        timer_on = 1;
                        goto out;
                    } else if (time_key == 109) goto in;
                }
                continue;
            } else {
                // ? 난이도별 폰트 크기 조정
                if (stage == 0) setConsoleFontSize(16);
                else if (stage == 1) setConsoleFontSize(12);
                else if (stage == 2) setConsoleFontSize(6);
                else if (stage == 3) setConsoleFontSize(4);

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
    srand((unsigned int)time(NULL));
    COORD startPoint = getRandomStartingPoint();

    if (timer_on == 1)
        startTime = time(NULL);

    int playerX = 1, playerY = 1;

    memset(map, MAP_FLAG_WALL, sizeof(map));
    generateMap(startPoint.Y, startPoint.X, map);
    map[playerY][playerX] = MAP_FLAG_EMPTY;

    printMapInitial(map);
    ///////////////////////////
    graphCalcFinished = 0;

    CreateThread(
        NULL,
        0,
        GraphCalcThread,
        map,
        0,
        NULL
    );

    moveCursor(0, MAP_HEIGHT + 5);
    printf("그래프 기반 최단 경로 계산 중...\n");

    updatePlayerPosition(playerX, playerY, playerX, playerY);
    printMoveCount();
    int seconds = 0;

    while (1) {
    // 1?? 키 입력 논블로킹 확인
    if (_kbhit()) {
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
        }
        else if (key == 114) {  // Reset
            updatePlayerPosition(playerX, playerY, 1, 1);
            playerX = 1; playerY = 1; move_count = 0;
            printMoveCount();
        }
        else if (key == 109) {  // 메뉴로 이동
            goto in;
        }
    }

    // 2?? 타이머 업데이트 (키 입력과 상관없이 항상 실행)
    if (timer_on) {
        time_t now = time(NULL);
        int seconds = (int)difftime(now, startTime);
        moveCursor(0, MAP_HEIGHT + 1);
        printf("%02d:%02d", seconds / 60, seconds % 60);
        fflush(stdout);
    }

    // 3?? 도착 시 기록 저장
    if (playerX == MAP_WIDTH - 2 && playerY == MAP_HEIGHT - 2) {

        if (timer_on == 1) {

            time_t now = time(NULL);
            int playTime = (int)difftime(now, startTime);

            system("cls");

            // 재미용 IQ 계산 공식
            double efficiency = (double)shortestPathResult / (playTime + 1);
            double IQ = 100 + (efficiency * 15) - (playTime * 0.4);

            if (IQ < 50) IQ = 50;
            if (IQ > 160) IQ = 160;

            printf("\n\n");
            printf("============================================================\n");
            printf("               ? IQ TEST RESULT (BETA) ?\n");
            printf("============================================================\n\n");

            printf(" ▶ 플레이어 클리어 시간 : %d초\n", playTime);
            printf(" ▶ 알고리즘 최단 거리   : %d칸\n", shortestPathResult);
            printf(" ▶ 알고리즘 계산 시간   : %.4f초\n\n", graphCalcTime);

            printf("------------------------------------------------------------\n");
            printf("   당신의 IQ는 ... 계산 중...\n");
            Sleep(1500);  // 연출용
            printf("   당신의 IQ는 *** %.1f *** 입니다!\n", IQ);
            printf("------------------------------------------------------------\n\n");

            printf("Press any key to return to the Menu.\n");
            _getch();
            goto in;
        }

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

    // 4?? CPU 점유율 줄이기
    Sleep(30);

        if (graphCalcFinished == 1) {
            graphCalcTime = graphCalcTime * 5;
            graphCalcFinished = 2;
            moveCursor(0, MAP_HEIGHT + 6);
            printf(
                "그래프 최단 경로 계산 완료! 최단 거리: %d, 계산 시간: %.4f초\n",
                shortestPathResult,
                graphCalcTime
            );
        }

    }

    return 0;
}
