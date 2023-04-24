#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include "panda.h"
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
void initGame(piece *p, node *n, side *s);
int rollDice();
void shuffle(piece *p, int n, int times);
void shufflePort(port *p, int n, int times);
void shuffleInt(int *p, int n, int times);
void printMap(const piece *p, int n, const port *t);
#endif