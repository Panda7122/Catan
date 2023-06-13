#include "bot.h"

#include <math.h>

#include "structure.h"
extern node corner[54];
extern side edge[72];
extern int nextdevelopCard;
int botOption(int state, player *Players, int index, piece *lands) {
    // draw
    if (state == 1) {
        printf("1\n");
        return 1;
    }
    // use card
    if (Players[index].card->size) {
        for (int i = 0; i < Players[index].card->size; ++i) {
            if (Players[index].card->data[i] <= 4) {
                printf("%d\n", state == 1 ? 2 : 5);
                return state == 1 ? 2 : 5;
            }
        }
    }
    // build city

    if (testBuildCity(Players, index)) {
        printf("3\n");
        return 3;
    }
    // build swttlement
    if (testBuildSwttlement(Players, index)) {
        printf("2\n");
        return 2;
    }
    // build road
    if (testBuildRoad(Players, index)) {
        printf("1\n");
        return 1;
    }
    // draw develop card

    if (Players[index].resource[WOOL] >= 1 &&
        Players[index].resource[WHEAT] >= 1 &&
        Players[index].resource[METAL] >= 1 && nextdevelopCard < 25) {
        printf("4\n");
        return 4;
    }
    // end trun
    printf("0\n");
    return 0;
}

int botRobber(piece *land, int playerID) {
    int bestID = -1;
    double bestWeights = -1;
    for (int i = 0; i < 19; ++i) {
        double nowWeights = 0;
        int peoplenearhere = 0;
        for (int j = 0; j < 6; ++j) {
            if (land[i].linkedNode[j]->belong != playerID &&
                land[i].linkedNode[j]->belong != PUBLIC) {
                peoplenearhere++;
            } else if (land[i].linkedNode[j]->belong != playerID) {
                peoplenearhere = 0;
                break;
            }
        }
        nowWeights = (6 - abs(land[i].number - 7) *
                              log(10 * (land[i].type == DESERT ? 0.1 : 1)) *
                              sqrt(peoplenearhere));
        if (nowWeights > bestWeights) {
            bestID = i;
            bestWeights = nowWeights;
        }
    }
    printf("%d\n", bestID);
    return bestID;
}
void botChooseBestRoad(piece *p, player *players, int index, int *landID,
                       int *roadID) {}

void botChooseBestSwttlement(piece *p, player *players, int index, int *landID,
                             int *roadID) {}
void botChooseBestCity(piece *p, player *players, int index, int *landID,
                       int *roadID) {}