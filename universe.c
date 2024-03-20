#include "universe.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct Universe {
	uint32_t rows;
	uint32_t cols;
	bool **grid;
	bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
	Universe *u = (Universe *) malloc(sizeof(Universe));
	u->rows = rows;
	u->cols = cols;
	u->toroidal = toroidal;

	// Allocate memory for Universe
	u->grid = (bool **) calloc(rows, sizeof(bool *));
	for (uint32_t r = 0; r < rows; r ++) {
		u->grid[r] = (bool *) calloc(cols, sizeof(bool));
	}

	return u;
}

void uv_delete(Universe *u) {
	// Deallocate memory of Universe
	for (uint32_t r = 0; r < u->rows; r += 1) {
		free(u->grid[r]);
	}
	free(u->grid);
	free(u);

	u = NULL;
}

uint32_t uv_rows(Universe *u) {
	return u->rows; // Return # of rows in given Universe
}

uint32_t uv_cols(Universe *u) {
	return u->cols; // Return # of columns in given Universe
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
	if (r < u->rows || c < u->cols) {
		u->grid[r][c] = true; // Set specified cell to alive
	}
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
	if (r < u->rows || c < u->cols) {
        	u->grid[r][c] = false; // Set specified cell to dead
        }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
	if (r < u->rows || c < u->cols) {
		return u->grid[r][c]; // Return state of cell
	}
	return false;
}

bool uv_populate(Universe *u, FILE *infile) {
	uint32_t row;
	uint32_t col;

	if (infile == NULL) {
		return false; // Return if file is empty
	}

	while(fscanf(infile, "%u %u", &row, &col) == 2) {
		if (row >= u->rows || col >= u->cols) {
			return false;
		} else {
			u->grid[row][col] = true;
		}
	}

	return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
	uint32_t live_cells = 0;

	uint32_t row_translation;
	uint32_t col_translation;

	for (int row_num = r - 1; row_num < r + 2; row_num++) {
		for (int col_num = c - 1; col_num < c + 2; col_num++) {

			// Skip current cell
			if (row_num != 0 && col_num != 0) {

				// Wrap if specified Universe is toroidal
				if (u->toroidal) {
					row_translation = (u->rows + r + row_num) % u->rows;
					col_translation = (u->cols + c + col_num) % u->cols;
				} else {
					row_translation = r + row_num;
					col_translation = c + col_num;
				}

				// Check if cell is alive
				if (row_translation < u->rows && col_translation < u->cols) {
					if (uv_get_cell(u, row_translation, col_translation)) {
						live_cells++;
					}
				}
			}
		}
	}

	return live_cells;
}

void uv_print(Universe *u, FILE *outfile) {
	for (uint32_t r = 0; r < u->rows; r++) {
		for (uint32_t c = 0; c < u->cols; c++) {
			if (u->grid[r][c]) {
				// Print 'o' if cell is alive
				fputc('o', outfile);
			} else {
				// Print '.' if cell is dead
				fputc('.', outfile);
			}
		}
		fprintf(outfile, "\n");
	}
}
