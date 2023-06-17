#include "structure.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bot.h"
#include "panda.h"
#include "vectorInt.h"
const int ORDER[19] = {16, 17, 18, 15, 11, 6, 2, 1, 0, 3,
                       7,  12, 13, 14, 10, 5, 4, 8, 9};
const int NUMBER[18] = {5, 2, 6,  3, 8, 10, 9, 12, 11,
                        4, 8, 10, 9, 4, 5,  6, 3,  11};
const int TEAMCOLOR[7] = {255, 93, 75, 82, 196, 204, 208};
const int PIECECOLOR[6] = {11, 28, 202, 145, 94, 237};
const int PORTCOLOR[6] = {0, 28, 202, 145, 94, 237};
const char PORTTEXT[6] = {'?', 'l', 'b', 'w', 'h', 'm'};
const char NODETYPE[3] = {' ', 's', 'c'};
const int NUMSHOW[10][7] = {
    {1, 1, 1, 0, 1, 1, 1}, {0, 0, 1, 0, 0, 1, 0}, {1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 0, 1, 1}, {0, 1, 1, 1, 0, 1, 0}, {1, 1, 0, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1}, {1, 0, 1, 0, 0, 1, 0}, {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1},
};
const char resourceStr[6][10] = {"None", "wood",  "bricks",
                                 "wool", "wheat", "metal"};
const char cardtoString[10][16] = {
    "Knight",    "Monopoly", "Year Of Plenty", "Road Building", "Chapel",
    "Greathall", "Market",   "Library",        "University",    "None"};
// int dicePiece[13][2] = {0};
int robberLoc = -1;
node corner[54];
side edge[72];
port tradePort[9];
player gamePlayer[6];
piece land[19];
int playerNumber = 0;
int developCard[25];
int nextdevelopCard = 0;
int longestPerson = -1;
int strongestPerson = -1;
void initGame(piece *p, node *n, side *s) {
    // corner bind
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 3; ++j) {
            p[i].linkedNode[j] = &(n[(i << 1) + (i >= 3) + (i >= 7) +
                                     ((i >= 12) << 1) + (i >= 16) * 3 + j]);
            p[18 - i].linkedNode[j + 3] = &(n[51 -
                                              (i * 2 + (i >= 3) + (i >= 7) +
                                               (i >= 12) * 2 + (i >= 16) * 3) +
                                              j]);
        }
    }
    // side bind
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 2; ++j) {
            p[i].linkedSide[j] = &(s[i * 2 + (i >= 3) * 4 + (i >= 7) * 5 +
                                     (i >= 12) * 7 + (i >= 16) * 7 + j]);
            p[18 - i].linkedSide[j + 4] =
                &(s[70 -
                    (i * 2 + (i >= 3) * 4 + (i >= 7) * 5 + (i >= 12) * 7 +
                     (i >= 16) * 7) +
                    j]);
            p[i].linkedSide[j + 2] =
                &(s[i + j + 6 + (i >= 3) * 9 + (i >= 7) * 11 + (i >= 12) * 11 +
                    (i >= 16) * 9]);
        }
    }
    // bind node and node | side and side | node and piece
    for (int i = 0; i < 19; ++i) {
        p[i].linkedNode[0]->linkedNode[1] = p[i].linkedNode[1];
        p[i].linkedNode[0]->linkedNode[2] = p[i].linkedNode[3];
        p[i].linkedNode[1]->linkedNode[1] = p[i].linkedNode[0];
        p[i].linkedNode[1]->linkedNode[2] = p[i].linkedNode[2];
        p[i].linkedNode[2]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedNode[2]->linkedNode[2] = p[i].linkedNode[5];
        p[i].linkedNode[3]->linkedNode[0] = p[i].linkedNode[0];
        p[i].linkedNode[3]->linkedNode[2] = p[i].linkedNode[4];
        p[i].linkedNode[4]->linkedNode[0] = p[i].linkedNode[3];
        p[i].linkedNode[4]->linkedNode[1] = p[i].linkedNode[5];
        p[i].linkedNode[5]->linkedNode[0] = p[i].linkedNode[2];
        p[i].linkedNode[5]->linkedNode[1] = p[i].linkedNode[4];
        p[i].linkedNode[0]->linkedSide[1] = p[i].linkedSide[0];
        p[i].linkedNode[0]->linkedSide[2] = p[i].linkedSide[2];
        p[i].linkedNode[1]->linkedSide[1] = p[i].linkedSide[0];
        p[i].linkedNode[1]->linkedSide[2] = p[i].linkedSide[1];
        p[i].linkedNode[2]->linkedSide[0] = p[i].linkedSide[1];
        p[i].linkedNode[2]->linkedSide[2] = p[i].linkedSide[3];
        p[i].linkedNode[3]->linkedSide[0] = p[i].linkedSide[2];
        p[i].linkedNode[3]->linkedSide[2] = p[i].linkedSide[4];
        p[i].linkedNode[4]->linkedSide[0] = p[i].linkedSide[4];
        p[i].linkedNode[4]->linkedSide[1] = p[i].linkedSide[5];
        p[i].linkedNode[5]->linkedSide[0] = p[i].linkedSide[3];
        p[i].linkedNode[5]->linkedSide[1] = p[i].linkedSide[5];
        p[i].linkedSide[0]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedSide[0]->linkedNode[1] = p[i].linkedNode[0];
        p[i].linkedSide[1]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedSide[1]->linkedNode[1] = p[i].linkedNode[2];
        p[i].linkedSide[2]->linkedNode[0] = p[i].linkedNode[0];
        p[i].linkedSide[2]->linkedNode[1] = p[i].linkedNode[3];
        p[i].linkedSide[3]->linkedNode[0] = p[i].linkedNode[2];
        p[i].linkedSide[3]->linkedNode[1] = p[i].linkedNode[5];
        p[i].linkedSide[4]->linkedNode[0] = p[i].linkedNode[3];
        p[i].linkedSide[4]->linkedNode[1] = p[i].linkedNode[4];
        p[i].linkedSide[5]->linkedNode[0] = p[i].linkedNode[5];
        p[i].linkedSide[5]->linkedNode[1] = p[i].linkedNode[4];

        for (int j = 0; j < 6; ++j) {
            p[i].linkedNode[j]->linkedPiece[2 - (int)(j / 2)] = &(p[i]);
        }
    }
}

void initPlayer(player *p) {
    p->card = create_vector_vectorInt();
    for (int i = 1; i <= 5; ++i) p->resource[i] = 0;
    p->knight = p->road = p->Score = 0;
    p->haveNode = create_vector_vectorInt();
    p->haveSide = create_vector_vectorInt();
    p->type = 0;
    p->kingofKnight = 0;
    p->kingofRoad = 0;
}
void freePlayer(player *p) {
    p->haveNode->free(p->haveNode);
    p->haveSide->free(p->haveSide);
    p->card->free(p->card);
    free(p);
}
void shufflePlayer(player *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        player tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void initNode(node *n) {
    for (int i = 0; i < 3; ++i) {
        n->linkedNode[i] = NULL;
        n->linkedPiece[i] = NULL;
        n->linkedPiece[i] = NULL;
    }
    n->nearPort = NULL;
    n->belong = 0;
    n->type = 0;
}
void initSide(side *n) {
    n->linkedNode[0] = NULL;
    n->linkedNode[1] = NULL;
    n->belong = 0;
    n->type = 0;
}
void initPiece(piece *n) {
    for (int i = 0; i < 6; ++i) {
        n->linkedNode[i] = NULL;
        n->linkedSide[i] = NULL;
    }
    n->robber = 0;
    n->type = 0;
    n->number = 7;
}
int rollDice() {
    srand(time(NULL));
    int a, b;
    a = rand() % 6 + 1;
    b = rand() % 6 + 1;
    printf("roll %d and %d => %d\n", a, b, a + b);
    return a + b;
}

void shuffle(piece *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        piece tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void shufflePort(port *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        port tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void shuffleInt(int *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        int tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
static char numberofPiece(int x, int y, int num) {
    // printf("%d %d:%d\n", x, y, num);
    if (x == 0) {
        if (y == 0) {
            return ' ';
        } else if (y == 1) {
            return NUMSHOW[num][0] ? '_' : ' ';
        } else {
            return ' ';
        }
    } else if (x == 1) {
        if (y == 0) {
            return NUMSHOW[num][1] ? '|' : ' ';

        } else if (y == 1) {
            return NUMSHOW[num][3] ? '_' : ' ';
        } else {
            return NUMSHOW[num][2] ? '|' : ' ';
        }
    } else {
        if (y == 0) {
            return NUMSHOW[num][4] ? '|' : ' ';
        } else if (y == 1) {
            return NUMSHOW[num][6] ? '_' : ' ';
        } else {
            return NUMSHOW[num][5] ? '|' : ' ';
        }
    }
}
static void printpart(const piece *p, int l, int r, int t, int tl, int size,
                      int space) {
    /* / \*/
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
        for (int k = l; k <= r; ++k) {
            if (k == 0)
                for (int j = 0; j < (size)*3 * 2; ++j)
                    printf("\e[48;5;17m \e[0m");
            if (k == 3 || k == 16)
                for (int j = 0; j < (size)*3; ++j) printf("\e[48;5;17m \e[0m");
            if (k == 16 || k == 12) {
                if (i == 0) {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[0]
                                         ->linkedPiece[0]
                                         ->linkedNode[3]
                                         ->belong],
                           NODETYPE[p[k].linkedNode[0]
                                        ->linkedPiece[0]
                                        ->linkedNode[3]
                                        ->type]);
                }
                for (int j = 0; j < 1 + i * 3 - (i == 0); ++j)
                    printf("\e[48;5;17m \e[0m");
                printf("\e[38;5;%dm＼\e[0m", TEAMCOLOR[p[k].linkedNode[0]
                                                           ->linkedPiece[0]
                                                           ->linkedSide[4]
                                                           ->belong]);
                for (int j = 0; j < (size - i - 1) * 3; ++j)
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[0]->linkedPiece[0]->type]);
            }

            if (size - 1 == i)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[k].linkedNode[0]->belong],
                       NODETYPE[p[k].linkedNode[0]->type]);
            for (int j = 0; j < 1 + (size - i - 1) * 3 - (size - 1 == i); ++j) {
                if (p[k].linkedNode[1]->linkedPiece[0] != NULL) {
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[0]->linkedPiece[0]->type]);
                } else
                    printf("\e[48;5;17m \e[0m");
            }
            printf("\e[38;5;%dm／\e[0m", TEAMCOLOR[p[k].linkedSide[0]->belong]);

            for (int j = 0; j < 1 + 2 * i * 3; ++j)
                if (i)
                    printf("\e[48;5;%dm \e[0m", PIECECOLOR[p[k].type]);
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[1]->belong],
                           NODETYPE[p[k].linkedNode[1]->type]);
                }
            printf("\e[38;5;%dm＼\e[0m", TEAMCOLOR[p[k].linkedSide[1]->belong]);
            if (size - 1 == i && k == r)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[k].linkedNode[2]->belong],
                       NODETYPE[p[k].linkedNode[2]->type]);
            for (int j = 0; j < (size - i - 1) * 3 - (size - 1 == i) + (k == r);
                 ++j)
                if (p[k].linkedNode[1]->linkedPiece[1] != NULL) {
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[1]->linkedPiece[1]->type]);
                } else
                    printf("\e[48;5;17m \e[0m");
            if (k == 18 || k == 15) {
                for (int j = 0; j < (size - i - 1) * 3; ++j)
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[2]->linkedPiece[0]->type]);
                printf("\e[38;5;%dm／\e[0m", TEAMCOLOR[p[k].linkedNode[2]
                                                           ->linkedPiece[0]
                                                           ->linkedSide[5]
                                                           ->belong]);
                if (i == 0) {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[2]
                                         ->linkedPiece[0]
                                         ->linkedNode[5]
                                         ->belong],
                           NODETYPE[p[k].linkedNode[2]
                                        ->linkedPiece[0]
                                        ->linkedNode[5]
                                        ->type]);
                }
                for (int j = 0; j < 1 + i * 3 - (i == 0); ++j)
                    printf("\e[48;5;17m \e[0m");
            }
            if (k == r) {
                if (r == 2)
                    for (int j = 0; j < (size)*6; ++j)
                        printf("\e[48;5;17m \e[0m");
                if (r == 6 || r == 18)
                    for (int j = 0; j < (size)*3; ++j)
                        printf("\e[48;5;17m \e[0m");
                ;
            }
        }
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");

        printf("\n");
    }
    /*| |*/
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
        for (int k = l; k <= r; ++k) {
            if (k == 0 || k == 16)
                for (int j = 0; j < (size)*3 * 2; ++j)
                    printf("\e[48;5;17m \e[0m");
            if (k == 3 || k == 12)
                for (int j = 0; j < (size)*3; ++j) {
                    if (k == 3 && j >= size * 3 - 13 && j < size * 3 - 2 &&
                        i < size && i >= size - 3) {
                        printf("\e[48;5;%dm", PORTCOLOR[tradePort[3].type]);
                        if (j - size * 3 + 13 < 2 || j - size * 3 + 13 > 8) {
                            printf("%c", PORTTEXT[tradePort[3].type]);
                        } else {
                            if (j - size * 3 + 13 < 5) {
                                printf("%c",
                                       numberofPiece(i - size + 3,
                                                     j - size * 3 + 13 - 2,
                                                     tradePort[3].request));
                            } else if (j - size * 3 + 13 > 5) {
                                printf("%c", numberofPiece(
                                                 i - size + 3,
                                                 j - size * 3 + 12 - 5 - 1, 1));
                            } else {
                                printf("%c", i - size + 3 == 1 ? ':' : ' ');
                            }
                        }
                        printf("\e[0m");
                    } else if (k == 12 && j >= size * 3 - 13 &&
                               j < size * 3 - 2 && i < size && i >= size - 3) {
                        printf("\e[48;5;%dm", PORTCOLOR[tradePort[4].type]);
                        if (j - size * 3 + 13 < 2 || j - size * 3 + 13 > 8) {
                            printf("%c", PORTTEXT[tradePort[4].type]);
                        } else {
                            if (j - size * 3 + 13 < 5)
                                printf("%c",
                                       numberofPiece(i - size + 3,
                                                     j - size * 3 + 13 - 2,
                                                     tradePort[4].request));
                            else if (j - size * 3 + 13 > 5)
                                printf("%c", numberofPiece(
                                                 i - size + 3,
                                                 j - size * 3 + 12 - 5 - 1, 1));
                            else {
                                printf("%c", i - size + 3 == 1 ? ':' : ' ');
                            }
                        }
                        printf("\e[0m");
                    } else
                        printf("\e[48;5;17m \e[0m");
                }
            printf("\e[38;5;%dm|\e[0m", TEAMCOLOR[p[k].linkedSide[2]->belong]);

            for (int j = 0; j < 2 * (size)*3 - 1; ++j) {
                if (j == 0 && i == 0) {
                    printf("\e[48;5;%dm%d\e[0m", PIECECOLOR[p[k].type], k / 10);
                } else if (j == 1 && i == 0) {
                    printf("\e[48;5;%dm%d\e[0m", PIECECOLOR[p[k].type], k % 10);
                } else if (j > (size)*3 - 1 && j < (size)*3 + 3 && i < 3) {
                    // printf("%d", p[k].number / 10);
                    printf("\e[48;5;%dm%c\e[0m", PIECECOLOR[p[k].type],
                           numberofPiece(i, j - size * 3, p[k].number / 10));
                } else if (j > (size)*3 + 3 && j < (size)*3 + 7 && i < 3) {
                    // printf("%d", p[k].number % 10);
                    printf(
                        "\e[48;5;%dm%c\e[0m", PIECECOLOR[p[k].type],
                        numberofPiece(i, j - size * 3 - 4, p[k].number % 10));
                } else {
                    if (j < (size)*3 - 3 && j >= (size)*3 - 5 && i >= 1 &&
                        i <= 2 && p[k].robber) {
                        printf("\e[48;5;240m \e[0m");
                    } else {
                        printf("\e[48;5;%dm \e[0m", PIECECOLOR[p[k].type]);
                    }
                }
            }
            if (k == r) {
                printf("\e[38;5;%dm|\e[0m",
                       TEAMCOLOR[p[k].linkedSide[3]->belong]);
                if (r == 2 || r == 18)
                    for (int j = 0; j < (size)*6; ++j) {
                        if (r == 2 && j <= 12 && j >= 2 && i < size &&
                            i >= size - 3) {
                            printf("\e[48;5;%dm", PORTCOLOR[tradePort[2].type]);
                            if (j < 4 || j > 10) {
                                printf("%c", PORTTEXT[tradePort[2].type]);
                            } else {
                                if (j < 7)
                                    printf("%c",
                                           numberofPiece(i - size + 3, j - 4,
                                                         tradePort[2].request));
                                else if (j > 7)
                                    printf("%c", numberofPiece(i - size + 3,
                                                               j - 7 - 1, 1));
                                else {
                                    printf("%c", i - size + 3 == 1 ? ':' : ' ');
                                }
                            }
                            printf("\e[0m");
                        } else if (r == 18 && j <= 12 && j >= 2 && i < size &&
                                   i >= size - 3) {
                            printf("\e[48;5;%dm", PORTCOLOR[tradePort[7].type]);
                            if (j < 4 || j > 10) {
                                printf("%c", PORTTEXT[tradePort[7].type]);
                            } else {
                                if (j < 7)
                                    printf("%c",
                                           numberofPiece(i - size + 3, j - 4,
                                                         tradePort[7].request));
                                else if (j > 7)
                                    printf("%c", numberofPiece(i - size + 3,
                                                               j - 7 - 1, 1));
                                else {
                                    printf("%c", i - size + 3 == 1 ? ':' : ' ');
                                }
                            }
                            printf("\e[0m");
                        } else
                            printf("\e[48;5;17m \e[0m");
                    }
                if (r == 6 || r == 15)
                    for (int j = 0; j < (size)*3; ++j)
                        printf("\e[48;5;17m \e[0m");
            }
        }
        for (int j = 0; j < space; ++j) {
            if (r == 11 && j <= 12 && j >= 2 && i < size && i >= size - 3) {
                printf("\e[48;5;%dm", PORTCOLOR[tradePort[5].type]);
                if (j < 4 || j > 10) {
                    printf("%c", PORTTEXT[tradePort[5].type]);
                } else {
                    if (j < 7)
                        printf("%c", numberofPiece(i - size + 3, j - 4,
                                                   tradePort[5].request));
                    else if (j > 7)
                        printf("%c", numberofPiece(i - size + 3, j - 7 - 1, 1));
                    else {
                        printf("%c", i - size + 3 == 1 ? ':' : ' ');
                    }
                }
                printf("\e[0m");
            } else {
                printf("\e[48;5;17m \e[0m");
            }
        }

        printf("\n");
    }
    /* \ / */
    if (l == 16) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
            for (int j = 0; j < 6 * size; ++j) printf("\e[48;5;17m \e[0m");
            for (int k = l; k <= r; ++k) {
                if (i)
                    for (int j = 0; j < 3 * i + 1; ++j)
                        printf("\e[48;5;17m \e[0m");
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[3]->belong],
                           NODETYPE[p[k].linkedNode[3]->type]);
                }
                printf("\e[38;5;%dm＼\e[0m",
                       TEAMCOLOR[p[k].linkedSide[4]->belong]);
                if (i != size - 1)
                    for (int j = 0; j < (size - i - 1) * 6 + 1; ++j)
                        printf("\e[48;5;%dm \e[0m",
                               PIECECOLOR
                                   [p[k].linkedNode[4]->linkedPiece[0]->type]);
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[4]->belong],
                           NODETYPE[p[k].linkedNode[4]->type]);
                }
                printf("\e[38;5;%dm／\e[0m",
                       TEAMCOLOR[p[k].linkedSide[5]->belong]);
                // if (i)
                for (int j = 0; j < 3 * i; ++j) printf("\e[48;5;17m \e[0m");
            }
            if (i == 0)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[r].linkedNode[5]->belong],
                       NODETYPE[p[r].linkedNode[5]->type]);
            else
                printf("\e[48;5;17m \e[0m");
            if (r == 18)
                for (int j = 0; j < (size)*6; ++j) printf("\e[48;5;17m \e[0m");
            for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
            printf("\n");
        }
    }
}
void printMap(const piece *p, int n, const port *t, int size, int space) {
    for (int c = 0; c < space >> 1; ++c) {
        for (int i = 0; i < 10 * 3 * size + 1 + 2 * space; ++i) {
            if ((space >> 1) - c <= 3 && i > space + 3 * 2 * size - 6 &&
                i < space + 3 * 2 * size + 6) {
                printf("\e[48;5;%dm", PORTCOLOR[tradePort[0].type]);
                if (i < space + 3 * 2 * size - 3 ||
                    i > space + 3 * 2 * size + 3)
                    printf("%c", PORTTEXT[tradePort[0].type]);
                else {
                    if (i < space + 3 * 2 * size) {
                        printf("%c",
                               numberofPiece(3 - (space >> 1) + c,
                                             i - (space + 3 * 2 * size - 3),
                                             tradePort[0].request));
                    } else if (i > space + 3 * 2 * size) {
                        printf("%c", numberofPiece(
                                         3 - (space >> 1) + c,
                                         i - (space + 3 * 2 * size) - 1, 1));
                    } else {
                        if (3 - (space >> 1) + c == 1)
                            printf(":");
                        else
                            printf(" ");
                    }
                }
                printf("\e[0m");
            } else if ((space >> 1) - c <= 3 &&
                       i > space + 3 * 6 * size - size - 6 &&
                       i < space + 6 * 3 * size - size + 6) {
                printf("\e[48;5;%dm", PORTCOLOR[tradePort[1].type]);
                if (i < space + 3 * 6 * size - size - 3 ||
                    i > space + 3 * 6 * size - size + 3)
                    printf("%c", PORTTEXT[tradePort[1].type]);
                else {
                    if (i < space + 3 * 6 * size - size) {
                        printf("%c", numberofPiece(
                                         3 - (space >> 1) + c,
                                         i - (space + 3 * 6 * size - size - 3),
                                         tradePort[1].request));
                    } else if (i > space + 3 * 6 * size - size) {
                        printf("%c",
                               numberofPiece(
                                   3 - (space >> 1) + c,
                                   i - (space + 3 * 6 * size - size) - 1, 1));
                    } else {
                        if (3 - (space >> 1) + c == 1)
                            printf(":");
                        else
                            printf(" ");
                    }
                }
                printf("\e[0m");
            } else
                printf("\e[48;5;17m \e[0m");
        }
        printf("\n");
    }
    printpart(p, 0, 2, 5, t[3].type, size, space);
    printpart(p, 3, 6, 3, t[2].type, size, space);
    printpart(p, 7, 11, 1, t[5].type, size, space);
    printpart(p, 12, 15, 3, t[4].type, size, space);
    printpart(p, 16, 18, 5, t[7].type, size, space);
    for (int c = 0; c < space >> 1; ++c) {
        for (int i = 0; i < 10 * 3 * size + 1 + 2 * space; ++i) {
            if (c < 3 && i > space + 3 * 2 * size - 6 &&
                i < space + 3 * 2 * size + 6) {
                printf("\e[48;5;%dm", PORTCOLOR[tradePort[6].type]);
                if (i < space + 3 * 2 * size - 3 ||
                    i > space + 3 * 2 * size + 3)
                    printf("%c", PORTTEXT[tradePort[6].type]);
                else {
                    if (i < space + 3 * 2 * size) {
                        printf("%c",
                               numberofPiece(c, i - (space + 3 * 2 * size - 3),
                                             tradePort[6].request));
                    } else if (i > space + 3 * 2 * size) {
                        printf("%c", numberofPiece(
                                         c, i - (space + 3 * 2 * size) - 1, 1));
                    } else {
                        if (c == 1)
                            printf(":");
                        else
                            printf(" ");
                    }
                }
                printf("\e[0m");
            } else if (c < 3 && i > space + 3 * 6 * size - size - 6 &&
                       i < space + 6 * 3 * size - size + 6) {
                printf("\e[48;5;%dm", PORTCOLOR[tradePort[8].type]);
                if (i < space + 3 * 6 * size - size - 3 ||
                    i > space + 3 * 6 * size - size + 3)
                    printf("%c", PORTTEXT[tradePort[8].type]);
                else {
                    if (i < space + 3 * 6 * size - size) {
                        printf("%c",
                               numberofPiece(
                                   c, i - (space + 3 * 6 * size - size - 3),
                                   tradePort[8].request));
                    } else if (i > space + 3 * 6 * size - size) {
                        printf(
                            "%c",
                            numberofPiece(
                                c, i - (space + 3 * 6 * size - size) - 1, 1));
                    } else {
                        if (c == 1)
                            printf(":");
                        else
                            printf(" ");
                    }
                }
                printf("\e[0m");
            } else
                printf("\e[48;5;17m \e[0m");
        }
        printf("\n");
    }
}
void robber(piece *land, int *robberLoc, int locate) {
    land[*robberLoc].robber = 0;
    land[locate].robber = 1;
    *robberLoc = locate;
}
void giveResource(piece *land, int index, player *p, int playerNum) {
    for (int i = 0; i < 6; ++i) {
        if (land[index].linkedNode[i]->belong) {
            for (int j = 0; j < playerNum; ++j) {
                // printf("%d: %d %d", i, p[j].type,
                //        land[index].linkedNode[i]->belong);
                if (p[j].type == land[index].linkedNode[i]->belong) {
                    if (land[index].linkedNode[i]->type == SWTTLEMENT) {
                        printf("give \e[38;5;%dmPlayer %d\e[0m a %s\n",
                               TEAMCOLOR[p[j].type], p[j].type,
                               resourceStr[land[index].type]);
                        p[j].resource[land[index].type]++;
                    } else if (land[index].linkedNode[i]->type == CITY) {
                        printf("give \e[38;5;%dmPlayer %d\e[0m two %s\n",
                               TEAMCOLOR[p[j].type], p[j].type,
                               resourceStr[land[index].type]);
                        p[j].resource[land[index].type] += 2;
                    }
                    break;
                }
            }
        }
    }
}
void chooseRobber(player *p, int index) {
    printf("Robber!\n");
    int locate;
    printf("which land you want to robber?");
    if (p[index].bot) {
        locate = botRobber(land, p[index].type);
    } else {
        while (1) {
            scanf("%d", &locate);
            if (locate < 19 && locate >= 0 && !land[locate].robber) break;
            printf("locate should in [0,18] and not been robber");
        }
    }
    robber(land, &robberLoc, locate);
}
void stoleResource(player *p, int index) {
    int player[6] = {0};
    int cnt = 0;
    for (int i = 0; i < 6; ++i) {
        player[i] = land[robberLoc].linkedNode[i]->belong;
        if (land[robberLoc].linkedNode[i]->belong != PUBLIC) ++cnt;
    }
    if (cnt) {
        int choose = 0;
        printf("which player you want steal resource?");
        if (p[index].bot) {
            int choosescore = 0;
            for (int i = 0; i < 6; ++i) {
                if (player[i] != p[index].type) {
                    if (player[i] != PUBLIC) {
                        int j = 0;
                        for (j = 0; j < playerNumber; ++j) {
                            if (p[j].type == player[i]) break;
                        }
                        if (p[j].Score > choosescore) {
                            choosescore = p[j].Score;
                            choose = p[j].type;
                        }
                    }
                }
            }
            printf("%d\n", choose);
        } else {
            while (1) {
                scanf("%d", &choose);
                int check = 0;
                for (int i = 0; i < 6; ++i)
                    if (player[i] == choose && player[i] != p[index].type)
                        check = 1;
                if (check) break;
                printf("Player %d is not on this land\n", choose);
            }
        }
        int j = 0;
        for (j = 0; j < playerNumber; ++j) {
            if (p[j].type == choose) break;
        }
        // printf("test%d %d\n", j, p[j].type);
        int arr[5] = {1, 2, 3, 4, 5};
        shuffleInt(arr, 5, 10);
        int idx = 0;
        for (int i = 0; i < 5; ++i) {
            int rec = arr[i];
            if (p[j].resource[rec] > 0) {
                p[j].resource[rec]--;
                printf("stole one %s\n", resourceStr[rec]);
                p[index].resource[rec]++;
                return;
            }
        }
        printf("no any resource can steal\n");
    }
}
bool testBuildRoad(player *Players, int index) {
    if (Players[index].resource[BRICKS] >= 1 &&
        Players[index].resource[WOOD] >= 1 &&
        Players[index].haveSide->size < 15) {
        for (int i = 0; i < Players[index].haveSide->size; ++i) {
            side nowedge = edge[Players[index].haveSide->data[i]];
            for (int j = 0; j < 2; ++j) {
                node *nownode = nowedge.linkedNode[j];
                for (int k = 0; k < 3; ++k) {
                    side *nearSide = nownode->linkedSide[k];
                    if (nearSide != NULL && nearSide->belong == PUBLIC) {
                        // printf("\n%d:%d %d\n", Players[index].type,
                        //        nearSide->index, nearSide->belong);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
bool testBuildSwttlement(player *Players, int index) {
    if (Players[index].resource[BRICKS] >= 1 &&
        Players[index].resource[WOOD] >= 1 &&
        Players[index].resource[WHEAT] >= 1 &&
        Players[index].resource[METAL] >= 1) {
        int count = 0;
        for (int i = 0; i < Players[index].haveNode->size; ++i) {
            if (corner[Players[index].haveNode->data[i]].type == SWTTLEMENT)
                ++count;
        }
        if (count >= 5) return 0;
        for (int i = 0; i < Players[index].haveSide->size; ++i) {
            int can = 1;
            side nowedge = edge[Players[index].haveSide->data[i]];
            for (int j = 0; j < 2; ++j) {
                node *nownode = nowedge.linkedNode[j];
                if (nownode->belong == PUBLIC) {
                    can = 1;
                    for (int k = 0; k < 3; ++k) {
                        node *nearNode = nownode->linkedNode[k];
                        if (nearNode != NULL && nearNode->belong != PUBLIC) {
                            can = 0;
                            break;
                        }
                    }
                    if (can) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
bool testBuildCity(player *Players, int index) {
    if (Players[index].resource[METAL] >= 3 &&
        Players[index].resource[WHEAT] >= 2) {
        int count = 0;
        for (int i = 0; i < Players[index].haveNode->size; ++i) {
            if (corner[Players[index].haveNode->data[i]].type == CITY) ++count;
        }
        if (count >= 4) return 0;
        for (int i = 0; i < Players[index].haveNode->size; ++i) {
            if (corner[Players[index].haveNode->data[i]].type == SWTTLEMENT) {
                return 1;
            }
        }
    }
    return 0;
}

int useDevlopCard(player *Players, int index, int nowsz) {
    printf("your card:\n");
    for (int i = 0; i < Players[index].card->size; ++i) {
        if (i >= nowsz) {
            printf("\e[38;5;1m");
        }
        printf("%d.%s-\n", i, cardtoString[Players[index].card->data[i]]);
        switch (Players[index].card->data[i]) {
            case 0:
                printf(" lets the player move the robber\n");
                break;
            case 1:
                printf(
                    " player can claim all resource cards of a specific "
                    "declared type\n");
                break;
            case 2:
                printf(
                    " the player can draw 2 resource cards of their choice "
                    "from the bank\n");
                break;
            case 3:
                printf(
                    " player can place 2 roads as if they just built them\n");
                break;
            default:
                printf(
                    " 1 additional Victory Point is added to the owners total "
                    "and doesn't need to be played to win.\n");
                break;
        }
        if (i >= nowsz) {
            printf("\e[0m");
        }
    }
    printf("which card you want to use?(-1 to cancel)\n");
    int choose;
    if (Players[index].bot) {
        choose = botUseCard(Players, index, nowsz);
        printf("%d\n", choose);
    } else
        scanf("%d", &choose);
    if (choose >= 0 && choose < nowsz) {
        if (Players[index].card->data[choose] == KNIGHT) {
            chooseRobber(Players, index);
            stoleResource(Players, index);
            // Players[index].card->remove(Players[index].card, choose);
            Players[index].knight++;

            if (Players[index].knight >= 3) {
                if (strongestPerson == -1) {
                    strongestPerson = Players[index].type;
                    Players[index].Score += 2;
                    printf("you now are the strongest player\n");
                } else {
                    int id = 0;
                    while (1) {
                        if (strongestPerson == gamePlayer[id].type) break;
                        ++id;
                    }
                    if (Players[index].knight > gamePlayer[id].knight) {
                        gamePlayer[id].Score -= 2;
                        Players[index].Score += 2;
                        printf("you now are the strongest player\n");
                        strongestPerson = Players[index].type;
                    }
                }
            }
            Players[index].card->remove(Players[index].card, choose);
            return 1;
        } else if (Players[index].card->data[choose] == MONOPOLY) {
            int Rec;
            printf("your resource:\n");
            for (int i = 1; i <= 5; ++i) {
                printf("%d.%s:%d\n", i, resourceStr[i],
                       Players[index].resource[i]);
            }
            printf("choose a resource\n");
            if (Players[index].bot) {
                Rec = botChooseResource(Players, index);
                printf("%d\n", Rec);
            } else
                scanf("%d", &Rec);
            int total = 0;
            for (int i = 0; i < playerNumber; ++i) {
                total += Players[i].resource[Rec];
                Players[i].resource[Rec] = 0;
            }
            Players[index].resource[Rec] = total;
            Players[index].card->remove(Players[index].card, choose);
            return 1;
        } else if (Players[index].card->data[choose] == YEAROFPLENTY) {
            for (int k = 0; k < 2; ++k) {
                int Rec;
                printf("your resource:\n");
                for (int i = 1; i <= 5; ++i) {
                    printf("%d.%s:%d\n", i, resourceStr[i],
                           Players[index].resource[i]);
                }
                printf("choose a resource\n");
                if (Players[index].bot)
                    Rec = botChooseResource(Players, index);
                else
                    scanf("%d", &Rec);
                Players[index].resource[Rec]++;
            }
            Players[index].card->remove(Players[index].card, choose);
            return 1;
        } else if (Players[index].card->data[choose] == ROADBUILDING) {
            int landID, roadID;
            for (int k = 0; k < 2; ++k) {
                if (testBuildRoad(Players, index)) {
                    while (1) {
                        printf("choose a road\n");
                        if (Players[index].bot) {
                            botChooseBestRoad(land, Players, index, &landID,
                                              &roadID);
                            printf("%d %d\n", landID, roadID);
                        } else {
                            scanf("%d%d", &landID, &roadID);
                        }
                        if (land[landID].linkedSide[roadID]->belong == PUBLIC) {
                            int check = 0;
                            for (int l = 0; l < 2; ++l) {
                                for (int k = 0; k < 3; ++k) {
                                    if (land[landID]
                                                .linkedSide[roadID]
                                                ->linkedNode[l] != NULL &&
                                        land[landID]
                                                .linkedSide[roadID]
                                                ->linkedNode[l]
                                                ->linkedSide[k] != NULL &&
                                        land[landID]
                                                .linkedSide[roadID]
                                                ->linkedNode[l]
                                                ->linkedSide[k]
                                                ->belong ==
                                            Players[index].type) {
                                        check = 1;
                                        break;
                                    }
                                }
                                if (check) break;
                            }
                            if (check) {
                                land[landID].linkedSide[roadID]->belong =
                                    Players[index].type;
                                land[landID].linkedSide[roadID]->type = ROAD;
                                Players[index].haveSide->push(
                                    Players[index].haveSide,
                                    land[landID].linkedSide[roadID]->index);
                                updateLongestRoad(Players, index);

                                if (Players[index].road >= 5) {
                                    if (longestPerson == -1) {
                                        longestPerson = Players[index].type;
                                    } else {
                                        int id = 0;
                                        while (1) {
                                            if (longestPerson ==
                                                gamePlayer[id].type)
                                                break;
                                            ++id;
                                        }
                                        if (Players[index].road >
                                            gamePlayer[id].road) {
                                            gamePlayer[id].Score -= 2;
                                            Players[index].Score += 2;
                                            longestPerson = Players[index].type;
                                        }
                                    }
                                }
                                break;
                            } else {
                                printf(
                                    "there had no your swttlement near "
                                    "here,input again\n");
                            }
                        } else {
                            printf("there had people here, input again\n");
                        }
                    }
                } else {
                    printf(
                        "there have no place can build, stop card building\n");
                }
            }
            Players[index].card->remove(Players[index].card, choose);
            return 1;
        }
    }
    return 0;
}
void trade(player *Players, int index) {
    if (Players[index].bot) return;
    printf("your resources:\n");
    for (int i = 1; i <= 5; ++i) {
        printf("%d.%s:%d\n", i, resourceStr[i], Players[index].resource[i]);
    }
    printf("ratio(?:1):\n");
    int have[6] = {0};
    for (int i = 0; i < Players[index].haveNode->size; ++i) {
        if (corner[Players[index].haveNode->data[i]].nearPort != NULL) {
            have[corner[Players[index].haveNode->data[i]].nearPort->type] = 1;
        }
    }
    int ratio[6] = {4, 4, 4, 4, 4, 4};
    for (int i = 1; i <= 5; ++i) {
        if (have[0]) ratio[i] = 3;
        if (have[i]) ratio[i] = 2;
        printf("%s:%d\n", resourceStr[i], ratio[i]);
    }
    printf("which resource you want use to trade?");
    int cost;
    scanf("%d", &cost);
    if (Players[index].resource[cost] >= ratio[cost]) {
        printf("which resource you want to buy?");
        int buy;
        scanf("%d", &buy);
        Players[index].resource[cost] -= ratio[cost];
        Players[index].resource[buy]++;
    } else {
        printf("you don't have enough %s", resourceStr[cost]);
    }
}
bool checkWin(player *Players, int index) {
    int score = Players[index].Score;
    for (int i = 0; i < Players[index].card->size; ++i) {
        if (Players[index].card->data[i] >= CHAPEL) {
            ++score;
        }
    }
    return score >= 10;
}
int getLongestPath(int g[54][54], int nowNode) {
    int ret = 0;
    for (int i = 0; i < 54; ++i) {
        if (g[nowNode][i]) {
            g[nowNode][i] = 0;
            g[i][nowNode] = 0;
            ret = max(ret, getLongestPath(g, i) + 1);
            g[nowNode][i] = 1;
            g[i][nowNode] = 1;
        }
    }
    return ret;
}
void updateLongestRoad(player *Players, int index) {
    int table[54][54] = {0};
    for (int i = 0; i < 54; ++i) {
        for (int j = 0; j < 54; ++j) {
            table[i][j] = 0;
        }
    }
    for (int i = 0; i < Players[index].haveSide->size; ++i) {
        table[edge[Players[index].haveSide->data[i]].linkedNode[0]->index]
             [edge[Players[index].haveSide->data[i]].linkedNode[1]->index] = 1;
        table[edge[Players[index].haveSide->data[i]].linkedNode[1]->index]
             [edge[Players[index].haveSide->data[i]].linkedNode[0]->index] = 1;
    }
    int ans = 0;
    for (int i = 0; i < 54; ++i) {
        int b = 0;
        for (int j = 0; j < 3; ++j) {
            if (corner[i].linkedSide[j] != NULL &&
                corner[i].linkedSide[j]->belong == Players[index].type) {
                b = 1;
                break;
            }
        }
        if (b) ans = max(ans, getLongestPath(table, i));
    }
    Players[index].road = ans;
}
void throwHalfResource(player *Players, int index) {
    printf("your resources:\n");
    int cnt = 0;
    for (int i = 1; i <= 5; ++i) {
        printf("%d.%s:%d\n", i, resourceStr[i], Players[index].resource[i]);
        cnt += Players[index].resource[i];
    }
    printf("you need throw %d resources\n", cnt / 2 + (cnt % 2));
    for (int i = 0; i < cnt / 2 + (cnt % 2); ++i) {
        printf("%d. choose a resource to throw away\n", i + 1);
        int choose;
        if (Players[index].bot) {
            int most = 1;
            for (int i = 1; i <= 5; ++i) {
                if (Players[index].resource[i] > Players[index].resource[most])
                    most = i;
            }
            choose = most;
        } else {
            scanf("%d", &choose);
        }
        if (Players[index].resource[choose] <= 0) {
            printf("this resource is not enough\n");
            --i;
        } else {
            Players[index].resource[choose]--;
        }
    }
}