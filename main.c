#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "panda.h"
#include "bot.h"
#include "structure.h"
#include "vectorInt.h"
#define SEASIZE 13
#define MAPSIZE 4
extern const int ORDER[19];
extern const int NUMBER[18];
extern const int PIECECOLOR[6];
// const int TEAMCOLOR[5] = {255, 9, 75, 82, 196};
extern int TEAMCOLOR[5];
extern char resourceStr[6][10];

extern int dicePiece[13][2];
extern int robberLoc;
extern node corner[54];
extern side edge[72];
extern port tradePort[9];
extern player gamePlayer[6];
extern piece land[19];
extern int playerNumber;
extern int developCard[25];
extern int nextdevelopCard;
void setUpGame() {
    while (1) {
        // ask player number
        printf("how mamy player?");
        scanf("%d", &playerNumber);
        if (playerNumber <= 4 && playerNumber >= 2) {  // in 2~4 people
            break;
        }
        printf("it shoud in [2,4]\n");
    }
    // setup develop card
    for (int i = 0; i < 14; ++i) developCard[i] = KNIGHT;
    for (int i = 14; i < 20; i += 2)
        developCard[i] = developCard[i + 1] = (i >> 1) - 6;
    for (int i = 20; i < 25; ++i) developCard[i] = i - 16;
    shuffleInt(developCard, 25, 1000);
    // set up piece
    for (int i = 2; i <= 12; ++i) {
        dicePiece[i][0] = -1;
        dicePiece[i][1] = -1;
    }
    for (int i = 0; i < 19; ++i) {
        initPiece(&(land[i]));
        if (i < 4)
            land[i].type = FOREST;
        else if (i < 8)
            land[i].type = ARABLE;
        else if (i < 12)
            land[i].type = FARM;
        else if (i < 15)
            land[i].type = HILL;
        else if (i < 18) {
            land[i].type = MINING;
        } else {
            land[i].type = DESERT;
        }
    }
    shuffle(land, 19, 1000);
    // set up piece resources
    int cnt = 0;
    for (int i = 0; i < 19; ++i) {
        if (land[ORDER[i]].type == DESERT) {
            robberLoc = ORDER[i];
            land[ORDER[i]].number = 7;
            land[ORDER[i]].robber = 1;
        } else {
            if (dicePiece[NUMBER[cnt]][0] == -1)
                dicePiece[NUMBER[cnt]][0] = ORDER[i];
            else
                dicePiece[NUMBER[cnt]][1] = ORDER[i];
            land[ORDER[i]].number = NUMBER[cnt];
            ++cnt;
        }
    }
    // initialize each corner and edge of land
    for (int i = 0; i < 54; ++i) {
        initNode(&(corner[i]));
        corner[i].index = i;
    }
    for (int i = 0; i < 72; ++i) {
        initSide(&(edge[i]));
        edge[i].index = i;
    }
    // initialize port
    for (int i = 0; i < 4; ++i) {
        tradePort[i].type = 0;
        tradePort[i].request = 3;
    }
    for (int i = 1; i <= 5; ++i) {
        tradePort[i + 3].type = i;
        tradePort[i + 3].request = 2;
    }
    shufflePort(tradePort, 9, 1000);
    corner[0].nearPort = &(tradePort[0]);
    corner[1].nearPort = &(tradePort[0]);
    corner[3].nearPort = &(tradePort[1]);
    corner[4].nearPort = &(tradePort[1]);
    corner[14].nearPort = &(tradePort[2]);
    corner[15].nearPort = &(tradePort[2]);
    corner[7].nearPort = &(tradePort[3]);
    corner[17].nearPort = &(tradePort[3]);
    corner[26].nearPort = &(tradePort[5]);
    corner[37].nearPort = &(tradePort[5]);
    corner[28].nearPort = &(tradePort[4]);
    corner[38].nearPort = &(tradePort[4]);
    corner[45].nearPort = &(tradePort[7]);
    corner[46].nearPort = &(tradePort[7]);
    corner[47].nearPort = &(tradePort[6]);
    corner[48].nearPort = &(tradePort[6]);
    corner[50].nearPort = &(tradePort[8]);
    corner[51].nearPort = &(tradePort[8]);
    // combind all corner, edge and land
    initGame(land, corner, edge);
    for (int i = 0; i < playerNumber; ++i) {
        initPlayer(&(gamePlayer[i]));
        gamePlayer[i].type = i + 1;
        if (i)
            gamePlayer[i].bot = 1;
        else
            gamePlayer[i].bot = 0;
    }
    shufflePlayer(gamePlayer, playerNumber, 100);
    int i, j;
    // set up initial swttlement and road
    for (int k = 0; k < playerNumber; ++k) {
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your first swttlement:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,corner)\n"
                "0/  1 \\2\n"
                " |land| \n"
                "3\\  4 /5\n");
            scanf("%d%d", &i, &j);
            // put first swttlement
            if (land[i].linkedNode[j]->belong == PUBLIC) {
                int check = 1;
                for (int l = 0; l < 3; ++l) {
                    if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                        land[i].linkedNode[j]->linkedNode[l]->belong !=
                            PUBLIC) {
                        check = 0;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedNode[j]->belong = gamePlayer[k].type;
                    land[i].linkedNode[j]->type = SWTTLEMENT;

                    break;
                } else {
                    printf("there had swttlement near here, input again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
        // put forst road
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your first road:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,edge)\n"
                "0/    \\1\n"
                "2|land|3\n"
                "4\\    /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedSide[j]->belong == PUBLIC) {
                int check = 0;
                for (int l = 0; l < 2; ++l) {
                    if (land[i].linkedSide[j]->linkedNode[l] != NULL &&
                        land[i].linkedSide[j]->linkedNode[l]->belong ==
                            gamePlayer[k].type) {
                        check = 1;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedSide[j]->belong = gamePlayer[k].type;
                    land[i].linkedSide[j]->type = ROAD;

                    break;
                } else {
                    printf(
                        "there had no your swttlement near here, input "
                        "again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
    }
    for (int k = playerNumber - 1; k >= 0; --k) {
        // put second swttlement
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your second swttlement:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,corner)\n"
                "0/  1 \\2\n"
                " |land| \n"
                "3\\  4 /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedNode[j]->belong == PUBLIC) {
                int check = 1;
                for (int l = 0; l < 3; ++l) {
                    if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                        land[i].linkedNode[j]->linkedNode[l]->belong !=
                            PUBLIC) {
                        check = 0;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedNode[j]->belong = gamePlayer[k].type;
                    land[i].linkedNode[j]->type = SWTTLEMENT;
                    printf("you got:");
                    for (int l = 0; l < 3; ++l) {
                        enum pieceType tmp =
                            land[i].linkedNode[j]->linkedPiece[l]->type;
                        if (tmp != DESERT) {
                            printf(" %s", resourceStr[tmp]);
                            gamePlayer[k].resource[tmp]++;
                        }
                    }
                    printf("\n");
                    break;
                } else {
                    printf("there had swttlement near here, input again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
        // put second road
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your second road:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,edge)\n"
                "0/    \\1\n"
                "2|land|3\n"
                "4\\    /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedSide[j]->belong == PUBLIC) {
                int check = 0;
                for (int l = 0; l < 2; ++l) {
                    if (land[i].linkedSide[j]->linkedNode[l] != NULL &&
                        land[i].linkedSide[j]->linkedNode[l]->belong ==
                            gamePlayer[k].type) {
                        check = 1;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedSide[j]->belong = gamePlayer[k].type;
                    land[i].linkedSide[j]->type = ROAD;
                    break;
                } else {
                    printf(
                        "there had no your swttlement near here, input "
                        "again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
    }
}
int main() {
    setUpGame();
    // main part
    for (int i = 0; i < playerNumber; ++i) {
        int state = 0;
        int step = 0;
        int number;
        int haveK = 0;
        for (int j = 0; j < gamePlayer[i].card->size; ++i) {
            if (gamePlayer[i].card->get(gamePlayer[i].card, j) == KNIGHT) {
                ++haveK;
            }
        }
        while (1) {
            if (state == 0) {  // draw
                printf("1.roll dice\n");
                if (haveK) {
                    printf("2. use Knight Card\n");
                }
                printf("your step:");
                if (gamePlayer[i].bot)
                    state = botOption(state, gamePlayer, i, land);
                else
                    scanf("%d", &step);

                if (step == 1) {
                    number = rollDice();
                    if (number == 7) {
                        chooseRobber(gamePlayer, i);
                    } else {
                        for (int j = 0; j < 19; ++j) {
                            if (!land[j].robber && land[j].number == number)
                                giveResource(land, j, gamePlayer, i);
                        }
                    }
                    state = 1;
                } else if (step == 2 && haveK) {
                    chooseRobber(gamePlayer, i);
                }
            } else if (state == 2) {
                printf("0.end turn\n");
                printf("1.put road\n");
                printf("2.put swttlement\n");
                printf("3.put city\n");
                printf("4.draw develop card\n");
                printf("5.use develop card\n");
                printf("6.trade\n");
                printf("7.building cost table\n");
                printf("your step:");
                if (gamePlayer[i].bot)
                    state = botOption(state, gamePlayer, i, land);
                else
                    scanf("%d", &step);
                if (step == 0) {
                    state = 1;
                    break;
                } else if (step == 1) {
                    if (testBuildRoad(gamePlayer, i)) {
                    } else {
                        printf("you can not put road\n");
                    }
                } else if (step == 2) {
                    if (testBuildSwttlement(gamePlayer, i)) {
                    } else {
                        printf("you can not put road\n");
                    }
                } else if (step == 3) {
                    if (testBuildCity(gamePlayer, i)) {
                    } else {
                        printf("you can not put road\n");
                    }
                } else if (step == 4) {
                    if (gamePlayer[i].resource[WOOL] >= 1 &&
                        gamePlayer[i].resource[WHEAT] >= 1 &&
                        gamePlayer[i].resource[METAL] >= 1 &&
                        nextdevelopCard < 25) {
                    } else {
                        printf("you can not draw card\n");
                    }
                } else if (step == 5) {
                    int can = 0;
                    if (gamePlayer[i].card->size == 0)
                        can = 0;
                    else {
                        for (int i = 0; i < gamePlayer[i].card->size; ++i) {
                            if (gamePlayer[i].card->data[i] <= 4) {
                                can = 1;
                                break;
                            }
                        }
                    }
                    if (can) {
                    } else {
                        printf("you don't have develop card can use\n");
                    }

                } else if (step == 6) {
                } else if (step == 7) {
                    printf("Building Cost Table:\n");
                    printf(
                        "1 \e[38;5;%dmwood\e[0m 1 \e[38;5;%dmbrick\e[0m -> "
                        "road (0 point)\n",
                        PIECECOLOR[WOOD], PIECECOLOR[BRICKS]);
                    printf(
                        "1 \e[38;5;%dmwood\e[0m 1 \e[38;5;%dmbrick\e[0m 1 "
                        "\e[38;5;%dmwheat\e[0m 1 \e[38;5;%dmmetal\e[0m -> "
                        "swttlement (1 point)\n",
                        PIECECOLOR[WOOD], PIECECOLOR[BRICKS], PIECECOLOR[WHEAT],
                        PIECECOLOR[METAL]);
                    printf(
                        "2 \e[38;5;%dmwheats\e[0m 3 \e[38;5;%dmmetals\e[0m -> "
                        "city (2 points)\n",
                        PIECECOLOR[WHEAT], PIECECOLOR[METAL]);
                    printf(
                        "1 \e[38;5;%dmwheat\e[0m 1 \e[38;5;%dmwool\e[0m 1 "
                        "\e[38;5;%dmmetal\e[0m -> "
                        "swttlement (? point)\n",
                        PIECECOLOR[WHEAT], PIECECOLOR[WOOL], PIECECOLOR[METAL]);
                }
            }
        }
    }
    // rollDice();
}