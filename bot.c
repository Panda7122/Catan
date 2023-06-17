#include "bot.h"

#include <math.h>
#include <stdio.h>

#include "structure.h"
extern piece land[19];
extern node corner[54];
extern side edge[72];
extern int nextdevelopCard;
int botOption(int state, player *Players, int index, piece *lands,
              int nowsize) {
    // draw
    if (state == 0) {
        printf("1\n");
        return 1;
    }
    // use card
    if (nowsize && Players[index].card->size) {
        for (int i = 0; i < nowsize; ++i) {
            if (Players[index].card->data[i] < CHAPEL) {
                printf("%d\n", state == 0 ? 2 : 5);
                return state == 0 ? 2 : 5;
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
        if (land[i].type != DESERT) {
            double nowWeights = 0;
            int peoplenearhere = 0;
            for (int j = 0; j < 6; ++j) {
                if (land[i].linkedNode[j]->belong != playerID &&
                    land[i].linkedNode[j]->belong != PUBLIC) {
                    peoplenearhere++;
                } else if (land[i].linkedNode[j]->belong == playerID) {
                    peoplenearhere = 0;
                    break;
                }
            }
            nowWeights = (6 - abs(land[i].number - 7)) * sqrt(peoplenearhere);
            // printf("%d %d %lf\n", i, peoplenearhere, nowWeights);
            if (nowWeights > bestWeights) {
                bestID = i;
                bestWeights = nowWeights;
            }
        }
    }
    printf("%d\n", bestID);
    return bestID;
}
int botUseCard(player *players, int index, int nowsz) {
    printf("    %d %ld\n", nowsz, players[index].card->size);
    // getchar();
    int haveK = -1;
    int haveRes = -1;
    for (int i = 0; i < min(nowsz, players[index].card->size); ++i) {
        if (players[index].card->data[i] == KNIGHT) {
            haveK = i;
            break;
        } else if (players[index].card->data[i] < CHAPEL) {
            if (haveRes == -1) haveRes = i;
        }
    }
    // printf("%d %d\n", haveK, haveRes);
    if (haveK != -1) return haveK;
    if (haveRes != -1)
        return haveRes;
    else
        return -1;
}
void botChooseDefaultRoad(piece *p, player *players, int index, int *landID,
                          int *roadID, int swttlementlocateI,
                          int swttlementlocateJ) {
    int bestID = -1;
    double bestweight = -1;
    for (int i = 0; i < 3; ++i) {
        node *nowNode = land[swttlementlocateI]
                            .linkedNode[swttlementlocateJ]
                            ->linkedNode[i];
        if (nowNode != NULL) {
            double nowweight = 0;
            for (int j = 0; j < 3; ++j) {
                if (nowNode->linkedPiece[j] != NULL &&
                    nowNode->linkedPiece[j]->type != DESERT) {
                    nowweight += (6 - abs(nowNode->linkedPiece[j]->number - 7));
                }
            }
            if (nowweight > bestweight) {
                bestweight = nowweight;
                bestID = land[swttlementlocateI]
                             .linkedNode[swttlementlocateJ]
                             ->linkedSide[i]
                             ->index;
            }
        }
    }
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (land[i].linkedSide[j]->index == bestID) {
                *landID = i;
                *roadID = j;
                return;
            }
        }
    }
    *landID = 0;
    *roadID = 0;
}
void botChooseBestRoad(piece *p, player *players, int index, int *landID,
                       int *roadID) {
    int bestID = -1, bestWeight = -1;
    int table[54][54] = {0};
    for (int i = 0; i < 54; ++i) {
        for (int j = 0; j < 54; ++j) {
            table[i][j] = 0;
        }
    }
    for (int i = 0; i < players[index].haveSide->size; ++i) {
        table[edge[players[index].haveSide->data[i]].linkedNode[0]->index]
             [edge[players[index].haveSide->data[i]].linkedNode[1]->index] = 1;
        table[edge[players[index].haveSide->data[i]].linkedNode[1]->index]
             [edge[players[index].haveSide->data[i]].linkedNode[0]->index] = 1;
    }
    for (int rd = 0; rd < players[index].haveSide->size; ++rd) {
        side nowEdge = edge[players[index].haveSide->data[rd]];
        for (int lkNode = 0; lkNode < 2; ++lkNode) {
            // printf("%d ", lkNode);
            node *nowNode = nowEdge.linkedNode[lkNode];
            for (int lkside = 0; lkside < 3; ++lkside) {
                // printf("%d %d\n", nowNode->index, lkside);
                side *thisEdge = nowNode->linkedSide[lkside];
                if (thisEdge != NULL && thisEdge->belong == PUBLIC) {
                    // printf("%d\n", thisEdge->index);
                    table[thisEdge->linkedNode[0]->index]
                         [thisEdge->linkedNode[1]->index] = 1;
                    table[thisEdge->linkedNode[1]->index]
                         [thisEdge->linkedNode[0]->index] = 1;
                    int nowWeight = 0;
                    for (int i = 0; i < 54; ++i) {
                        int b = 0;
                        for (int j = 0; j < 3; ++j) {
                            if (corner[i].linkedSide[j] != NULL &&
                                corner[i].linkedSide[j]->belong ==
                                    players[index].type) {
                                b = 1;
                                break;
                            }
                        }

                        if (b) {
                            nowWeight =
                                max(nowWeight, getLongestPath(table, i));
                            // printf("%d %d\n", i, tmp);
                        }
                    }
                    if (nowWeight > bestWeight || bestID == -1) {
                        bestID = thisEdge->index;
                        bestWeight = nowWeight;
                    }
                    table[thisEdge->linkedNode[0]->index]
                         [thisEdge->linkedNode[1]->index] = 0;
                    table[thisEdge->linkedNode[1]->index]
                         [thisEdge->linkedNode[0]->index] = 0;
                }
            }
        }
    }

    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (land[i].linkedSide[j]->index == bestID) {
                *landID = i;
                *roadID = j;
                return;
            }
        }
    }
    *landID = 0;
    *roadID = 0;
}
void botChooseDefaultSwttlement(piece *p, player *players, int index,
                                int *landID, int *swttlementID) {
    int bestIDi, bestIDj = -1;
    double bestWeight = -1;
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (p[i].linkedNode[j]->belong == PUBLIC) {
                int check = 1;
                for (int k = 0; k < 3; ++k) {
                    if (p[i].linkedNode[j]->linkedNode[k] != NULL &&
                        p[i].linkedNode[j]->linkedNode[k]->belong != PUBLIC)
                        check = 0;
                }
                if (check) {
                    double nowWeight = 0;
                    int type = 0;
                    int havetype[6] = {0};
                    node *nowNode = p[i].linkedNode[j];
                    for (int j = 0; j < 3; ++j) {
                        if (nowNode->linkedPiece[j] != NULL &&
                            nowNode->linkedPiece[j]->type != DESERT) {
                            nowWeight +=
                                6 - abs(nowNode->linkedPiece[j]->number - 7);
                            if (!havetype[nowNode->linkedPiece[j]->type]) {
                                ++type;
                                havetype[nowNode->linkedPiece[j]->type] = 1;
                            }
                        }
                    }
                    nowWeight *= log(10 * type);
                    if (nowWeight > bestWeight || bestIDi == -1) {
                        bestWeight = nowWeight;
                        bestIDi = i;
                        bestIDj = j;
                    }
                }
            }
        }
    }

    *landID = bestIDi;
    *swttlementID = bestIDj;
}
void botChooseBestSwttlement(piece *p, player *players, int index, int *landID,
                             int *swttlementID) {
    int bestID = -1;
    double bestWeight = -1;
    for (int i = 0; i < players[index].haveSide->size; ++i) {
        for (int k = 0; k < 2; ++k) {
            node *nowNode =
                edge[players[index].haveSide->data[i]].linkedNode[k];
            int check = 1;
            if (nowNode->belong != PUBLIC) check = 0;
            if (check) {
                for (int j = 0; j < 3; ++j) {
                    if (nowNode->linkedNode[j] != NULL &&
                        nowNode->linkedNode[j]->belong != PUBLIC)
                        check = 0;
                }
            }
            if (check) {
                double nowWeight = 0;
                int type = 0;
                int havetype[6] = {0};
                for (int j = 0; j < 3; ++j) {
                    if (nowNode->linkedPiece[j] != NULL &&
                        nowNode->linkedPiece[j]->type != DESERT) {
                        nowWeight +=
                            6 - abs(nowNode->linkedPiece[j]->number - 7);
                        if (!havetype[nowNode->linkedPiece[j]->type]) {
                            ++type;
                            havetype[nowNode->linkedPiece[j]->type] = 1;
                        }
                    }
                }
                nowWeight *= log(10 * type);
                if (nowWeight > bestWeight || bestID == -1) {
                    bestWeight = nowWeight;
                    bestID = nowNode->index;
                }
            }
        }
    }
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (land[i].linkedNode[j]->index == bestID) {
                *landID = i;
                *swttlementID = j;
                return;
            }
        }
    }
    *landID = 0;
    *swttlementID = 0;
}
void botChooseBestCity(piece *p, player *players, int index, int *landID,
                       int *cityID) {
    int bestID = -1;
    double bestWeight = -1;
    for (int i = 0; i < players[index].haveNode->size; ++i) {
        double nowWeight = 0;
        node *nowNode = &corner[players[index].haveNode->data[i]];
        if (nowNode->type == SWTTLEMENT) {
            int type = 0;
            int havetype[6] = {0};
            for (int j = 0; j < 3; ++j) {
                if (nowNode->linkedPiece[j] != NULL &&
                    nowNode->linkedPiece[j]->type != DESERT) {
                    nowWeight += 6 - abs(nowNode->linkedPiece[j]->number - 7);
                    if (!havetype[nowNode->linkedPiece[j]->type]) {
                        ++type;
                        havetype[nowNode->linkedPiece[j]->type] = 1;
                    }
                }
            }
            nowWeight *= log(10 * type);
            if (nowWeight > bestWeight) {
                bestWeight = nowWeight;
                bestID = nowNode->index;
            }
        }
    }
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (land[i].linkedNode[j]->index == bestID) {
                *landID = i;
                *cityID = j;
                return;
            }
        }
    }
    *landID = 0;
    *cityID = 0;
}
int botChooseResource(player *Players, int index) {
    int less = 1;
    for (int i = 1; i <= 5; ++i) {
        if (Players[index].resource[i] < Players[index].resource[less])
            less = i;
    }
    return less;
}