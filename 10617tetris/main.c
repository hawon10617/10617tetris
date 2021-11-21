#include "Define.h"

void init(struct Board* board, int queue[2], int* DebugMode);
void update(struct Board* board, struct Piece* piece);
void render(struct Board board, struct Piece piece, int queue1, int DebugMode);
void release(struct Board board);

const char TETRIS[7][100] = {
		"#### ##  ### ###  #### ##  ### ##     ####    ## ##",
		"# ## ##   ##  ##  # ## ##   ##  ##     ##    ##   ##",
		"  ##      ##        ##      ##  ##     ##    ####",
		"  ##      ## ##     ##      ## ##      ##     #####",
		"  ##      ##        ##      ## ##      ##        ###",
		"  ##      ##  ##    ##      ##  ##     ##    ##   ##",
		" ####    ### ###   ####    #### ##    ####    ## ##"
};

int main() {
	const int fps = 120;
	const int frameDelay = 1000 / fps;
	system("mode con cols=95 lines=50 | title TETRIS");
	//=========================
	struct Piece piece = { 0 }, p = { 0 }; //p means point, to test whether the piece is in the border.
	struct Board board = { 0 };
	int built = 0;
	int queue[2] = { 0 };
	int DebugMode = 0;
	//To activate Debug mode, press [D].
	createBuffer();
	clearBuffer();
	init(&board, queue, &DebugMode);

	for (; board.height < 20;) {
		spawn(&piece, queue);
		while (!(built)) {
			clearBuffer();
			update(&board, &piece);
			render(board, piece, queue[1], DebugMode);
			moveDown(&board, &p, &piece, &built, DebugMode);
			moves(&board, &p, &piece, &built);
			checkHeight(&board);
			flipBuffer();
			Sleep(frameDelay);
		}
		built = 0;
	}
	
	release(board);
	releaseBuffer();

	return 0;
}

void init(struct Board* board, int queue[2], int* DebugMode) {
	char c;
	int a = 1;
	while (1) {
		clearBuffer();
		for (int i = 0; i < 7; i++) {
			draw(10, i + 8, TETRIS[i]);
		}
		if (a % 2 == 1) {
			draw(17, LINE + 8, "Press [Enter] to Start...");
		}
		if (_kbhit()) {
			c = _getch();
			if (c == ENTER)
				break;
			else if (c == 100) { //'d'
				*DebugMode = 1;
				break;
			}
		}
		Sleep(800);
		a++;
		flipBuffer();
	}

	board->height = 0;
	board->level = 1;
	board->line = 0;
	board->score = 0;
	queue[1] = random(1, 7);
	flipBuffer();
}
void update(struct Board* board, struct Piece* piece) {
	for (int i = 0; i < piece->size; i++) {
		for (int j = 0; j < piece->size; j++) {
			if (piece->shape[i][j] == 2) {
				board->grid[i + piece->y][j + piece->x] = piece->shape[i][j];
			}
		}
	}
}
void render(struct Board board, struct Piece piece, int queue1, int DebugMode) {
	draw(0, 31, "Press [Esc] / [Enter] to <Pause / Resume>");
	for (int i = 0; i < 7; i++) {
		draw(0, i, TETRIS[i]);
	}
	char str[20];
	for (int i = 0; i < 20; i++) {
		draw(0, i + LINE, "▩");
		draw(11, i + LINE, "▩");
		for (int j = 0; j < 10; j++) {
			//sprintf_s(str, sizeof(str), "%d ", board.grid[i][j]);
			//draw(j + 1, i + LINE, str);
			switch (board.grid[i][j]) {
				case 0:
					draw(j + 1, i + LINE, "  ");
					break;
				case 1:
					draw(j + 1, i + LINE, "□");
					break;
				case 2:
					draw(j + 1, i + LINE, "■");
					break;
			}
		}
	}
	for (int i = 0; i < 12; i++) {
		draw(i, 20 + LINE, "▩");
	}
	//===== queue preview
	int qArr[4][4] = { 0 };
	char qstr[4][4];
	queuePiece(queue1, qArr);
	draw(15, LINE, "NEXT");
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (i == 0 || i == 5 || j == 0 || j == 5) {
				draw(i + 15, j + LINE + 1, "▩");
			}
			else {
				switch (qArr[i - 1][j - 1]) {
				case 0:
					draw(j + 15, i + LINE + 1, "  ");
					break;
				case 2:
					draw(j + 15, i + LINE + 1, "■");
					break;
				}
				if (queue1 == 1) {
					draw(j + 15, 10, "  ");
				}
			}
		}
	}
	//=====
	if (DebugMode == 1) {
		char str1[20];
		draw(25, LINE, "Debug");
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				sprintf_s(str1, sizeof(str1), "%d ", board.grid[i][j]);
				draw(j + 25, i + LINE + 1, str1);
			}
		}
	}
	//=====
	char score[20];
	char level[20];
	char line[20];
	sprintf_s(score, sizeof(score), "score : %d", board.score);
	sprintf_s(level, sizeof(level), "level : %d", board.level);
	sprintf_s(line, sizeof(line), "line : %d", board.line);
	draw(15, 20, score);
	draw(15, 21, level);
	draw(15, 22, line);
}
void release(struct Board board) {
	char c;
	while (1) {
		clearBuffer();
		for (int i = 0; i < 7; i++) {
			draw(10, i + 8, TETRIS[i]);
		}
		draw(15, LINE + 8, "Press [SPACE] to Re-Start...");
		draw(15, LINE + 9, "or Press Any Key to Shut Down...");
		char score[20];
		char level[20];
		char line[20];
		sprintf_s(score, sizeof(score), "score : %d", board.score);
		sprintf_s(level, sizeof(level), "level : %d", board.level);
		sprintf_s(line, sizeof(line), "line : %d", board.line);
		draw(19, LINE + 15, score);
		draw(19, LINE + 16, level);
		draw(19, LINE + 17, line);
		if (_kbhit()) {
			c = _getch();
			if (c == SPACE) {
				main();
			}
			break;
		}
		flipBuffer();
	}
}
