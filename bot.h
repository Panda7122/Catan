#ifndef _BOT_H
#define _BOT_H
#include "structure.h"
#include "vectorInt.h"
int botOption(int state, player *Players, int index, piece *lands);
int botRobber(piece *land, int playerID);
void botChooseBestRoad(piece *p, player *players, int index, int *landID,
                       int *roadID);
void botChooseBestSwttlement(piece *p, player *players, int index, int *landID,
                             int *roadID);
void botChooseBestCity(piece *p, player *players, int index, int *landID,
                       int *roadID);
int botUseCard(player *players, int index, int nowsz);
int botChooseResource(player *Players, int index);
void botChooseDefaultSwttlement(piece *p, player *players, int index,
                                int *landID, int *swttlementID);
void botChooseDefaultRoad(piece *p, player *players, int index, int *landID,
                          int *roadID, int swttlementlocateI,
                          int swttlementlocateJ);
#endif