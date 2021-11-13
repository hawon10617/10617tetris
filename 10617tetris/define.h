#define LEFT 75
#define UP 72
#define RIGHT 77
#define DOWN 80
#define SPACE 32
#define ENTER 13
#define LINE 8

#pragma once
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

void createBuffer();
void flipBuffer();
void clearBuffer();
void releaseBuffer();
void draw(int x, int y, const char* string);
int random(int from, int to);
//=====
void rmPiece(struct Board* board, struct Piece* piece);
void copyPiece(struct Piece* p, struct Piece* piece);
void moves(struct Board* board, struct Piece* p, struct Piece* piece, int* built);
int valid(struct Board* board, struct Piece* p, int mode);
void moveDown(struct Board* board, struct Piece* p, struct Piece* piece, int* built);
void checkHeight(struct Board* board);
int rotate(struct Piece* p);
void spawn(struct Piece* piece, int queue[2]);
void queuePiece(int queue1, int qArr[][4]);

struct Piece {
	int shape[4][4];
	int size; //2, 3, 4
	int x, y;
	char color;
};

struct Board {
	int grid[20][10];
	int height, score, level, line;
};
