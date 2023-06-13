#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include "panda.h"
#include "vectorInt.h"
struct _node;
struct _side;
struct _piece;
struct _port;

enum TEAM {
    PUBLIC,
    PLAYER1,
    PLAYER2,  // com1
    PLAYER3,  // com2
    PLAYER4,  // com3
    PLAYER5,  // com3
    PLAYER6,  // com3
};

enum nodeType { NONE, SWTTLEMENT, CITY };
enum developType {
    KNIGHT,
    MONOPOLY,
    YEAROFPLENTY,
    ROADBUILDING,
    CHAPEL,
    GREATHALL,
    MARKET,
    LIBRARY,
    UNIVERSITY
};
enum sideType { None, ROAD };
enum pieceType { DESERT, FOREST, HILL, FARM, ARABLE, MINING };
enum resourceType { WOOD = 1, BRICKS, WOOL, WHEAT, METAL };

typedef struct _node {
    struct _node *linkedNode[3];
    struct _side *linkedSide[3];
    struct _piece *linkedPiece[3];
    struct _port *nearPort;
    enum nodeType type;
    enum TEAM belong;
    int index;
} node;
void initNode(node *n);
typedef struct _side {
    struct _node *linkedNode[2];
    enum sideType type;
    enum TEAM belong;
    int index;
} side;
void initSide(side *n);

typedef struct _piece {
    enum pieceType type;
    int robber;
    struct _side *linkedSide[6];
    struct _node *linkedNode[6];
    int number;
} piece;
void initPiece(piece *n);
typedef struct _port {
    int type;
    int request;
} port;
typedef struct _player {
    vectorInt *haveNode;
    vectorInt *haveSide;
    int Score;
    int knight;
    int road;
    enum TEAM type;
    vectorInt *card;
    bool bot;
    int resource[6];
} player;
void initGame(piece *p, node *n, side *s);
int rollDice();
void shuffle(piece *p, int n, int times);
void shufflePort(port *p, int n, int times);
void shuffleInt(int *p, int n, int times);
void printMap(const piece *p, int n, const port *t, int size, int space);
void initPlayer(player *p);
void freePlayer(player *p);
void shufflePlayer(player *p, int n, int times);
void giveResource(piece *land, int index, player *p, int playerNum);
void robber(piece *land, int *robberLoc, int locate);
void chooseRobber(player *p, int index);
void useDevlopCard(player *Players, int index);
void trade(player *Players, int index);
bool checkWin(player *Players, int index);
void updateLongestRoad(player *Players, int index);
bool testBuildRoad(player *Players, int index);
bool testBuildSwttlement(player *Players, int index);
bool testBuildCity(player *Players, int index);
#endif