#include <stdio.h>
#include <stdlib.h>

#ifdef TUI
#include <ncurses.h>
#define ESC 27
#endif

#define ROCKFORD '@'
#define EMPTY ' '
#define DIRT '+'
#define STONE 'O'
#define DIAMOND '$'
#define EXIT 'X'

void print_board(char **board, int rows, int colums);
void gravity(char **board, int rows, int colums);
void find_position(char **board, int rows, int colums, int pos[]);
void count_diamonds(char **board, int rows, int colums, int *p_count);
void code_move(int move, int mv[]);
void make_move(char **board, int mv[], int *p_count, int pos[]);

int main(void)
{
	int rows, colums; /* liczba rzędów, kolumn */
	scanf("%d%d\n", &rows, &colums);
	/* inicjowanie dynamicznej tablicy wskaźników na tablice przechowującej planszę */
	char **board = malloc((size_t)rows * sizeof(char *));
	for (int i = 0; i < rows; i++) {
		board[i] = malloc((size_t)(colums + 1) * sizeof(char));
		for (int j = 0; j <= colums; j++)
			scanf("%c", &board[i][j]);
	}
	/* pozycja Rockforda */
	int pos[2]; 
	find_position(board, rows, colums, pos);
	/* ile diamentów zostało do zebrania */
	int count = 0; 
	int *p_count = &count;
	count_diamonds(board, rows, colums, p_count);
	/* inicjalizacja TUI */
	#ifdef TUI
	initscr();
	#endif
	/* sprowadzenie planszy do stanu stabilnego */
	gravity(board, rows, colums);
	print_board(board, rows, colums);

	int move; /* znak ruchu {w, a, s, d} */
	int mv[2]; /* ruch zakodowany w tablicy */
	#ifdef TUI
	/* główny kod gry w TUI */
	while ((move = getch()) != ESC) {		
		code_move(move, mv);
		make_move(board, mv, p_count, pos);
		gravity(board, rows, colums);
		print_board(board, rows, colums);
	}
	endwin();
	#else
	/* główny kod gry */
	while ((move = getchar()) != EOF) {
		if (move != '\n') {
			code_move(move, mv);
			make_move(board, mv, p_count, pos);
			gravity(board, rows, colums);
		}
		else
			print_board(board, rows, colums);
	}
	#endif
	/* zwolnienie planszy */
	for (int i = 0; i < rows; i++)
		free(board[i]);
	free(board); 

	return 0;
}
#ifdef TUI
/* wypisuje planszę do TUI */
void print_board(char **board, int rows, int colums)
{	
	clear();
	printw("%d %d\n", rows, colums);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j <= colums; j++)
			printw("%c", board[i][j]);
	refresh();
}
#else
/* wypisuje planszę */
void print_board(char **board, int rows, int colums)
{
	printf("%d %d\n", rows, colums);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j <= colums; j++)
			putchar(board[i][j]);
}
#endif
/* doprowadza planszę do stanu stabilnego */
void gravity(char **board, int rows, int colums)
{
	for (int i = rows - 2; i > 0; i--) 
		for (int j = 1; j < colums - 1; j++) 
			if (board[i][j] == STONE || board[i][j] == DIAMOND) 
				for (int k = 1; board[i + k][j] == EMPTY; k++) {
					board[i + k][j] = board[i + k - 1][j];
					board[i + k - 1][j] = EMPTY;
				}
}
/* znajduje pozycję Rockforda */
void find_position(char **board, int rows, int colums, int pos[]) 
{
	pos[0] = 0;
	for (int i = 1; i < rows - 1; i++) 
		for (int j = 1; j < colums - 1; j++) 
			if (board[i][j] == ROCKFORD) {
				pos[0] = i;
				pos[1] = j;
			}
}
/* znajduje liczbę diamentów na planszy */
void count_diamonds(char **board, int rows, int colums, int *p_count)
{
	for (int i = 1; i < rows - 1; i++) 
		for (int j = 1; j < colums - 1; j++) 
			if (board[i][j] == DIAMOND) 
				*p_count += 1;			
}
/* zmienia znak ruchu na bardziej przyjazną postać */
void code_move(int move, int mv[])
{
	mv[0] = 0; 
	mv[1] = 0;
	switch (move) {
		case 'w':
			mv[0] = -1;
			break;
		case 'a':
			mv[1] = -1;
			break;
		case 's':
			mv[0] = 1;
			break;
		case 'd':
			mv[1] = 1;
			break;
	}
}
/* wykonuje ruch */
void make_move(char **board, int mv[], int *p_count, int pos[])
{
	if (pos[0] != 0) {
		char *current = &board[pos[0]][pos[1]]; /* obecna pozycja */
		char *new = &board[pos[0] + mv[0]][pos[1] + mv[1]]; /* pole na które chce wejść  */
		if (*new == EMPTY || *new == DIRT) {
			*current = EMPTY;
			*new = ROCKFORD;
			pos[0] += mv[0];
			pos[1] += mv[1];
		}
		else if (*new == STONE && board[pos[0] + 2 * mv[0]][pos[1] + 2 * mv[1]] == EMPTY) {
			if (mv[0] == 0) {
				*current = EMPTY;
				*new = ROCKFORD;
				board[pos[0] + 2 * mv[0]][pos[1] + 2 * mv[1]] = STONE;
				pos[0] += mv[0];
				pos[1] += mv[1];
			}
		}
		else if (*new == DIAMOND) {
			*current = EMPTY;
			*new = ROCKFORD;
			pos[0] += mv[0];
			pos[1] += mv[1];
			*p_count -= 1;
		}
		else if (*new == EXIT && *p_count == 0) {
			*current = EMPTY;
			pos[0] = 0;
			#ifdef TUI
			flash();
			#endif
		}
	}
}
