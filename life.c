#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include "universe.h"

#define OPTIONS "tsn:i:o:"

#define DELAY 50000

void display_universe(Universe *u) {
        for (uint32_t r = 0; r < uv_rows(u); r++) {
                for (uint32_t c = 0; c < uv_cols(u); c++) {
                        if (uv_get_cell(u, r, c)) {
                                // Display 'o' if cell is alive
                                mvprintw(r, c, "o");
                        } else {
                                // Display ' ' if cell is dead
                                mvprintw(r, c, " ");
                        }
                }
        }
}

void perform_generation(Universe *A, Universe *B) {
        uint32_t live_cells;

        for (uint32_t r = 0; r < uv_rows(A); r++) {
                for (uint32_t c = 0; c < uv_cols(A); c++) {
                        live_cells = uv_census(A, r, c);

			// If cell is alive
			if (uv_get_cell(A, r, c)) {
                                if (live_cells == 2 || live_cells == 3) {
                                        uv_live_cell(B, r, c);
                                } else {
                                        uv_dead_cell(B, r, c);
                                }
                        // If cell is dead
                        } else {
				if (live_cells == 3) {
                                	uv_live_cell(B, r, c);
				} else {
                                	uv_dead_cell(B, r, c);
				}
                        }
		}
	}
}

void swap_universes(Universe *A, Universe *B) {
	Universe *temp;
        temp = A;

        // Swap Universe pointers
        A = B;
        B = temp;
}

int main(int argc, char **argv) {

	uint32_t opt = 0;
	uint32_t generations = 100; // default generations
	uint32_t rows;
	uint32_t cols;
	bool toroidal = false; // default state (non-toroidal)
	bool silence_ncurses = false; // default mode: print to screen
	Universe *A, *B;
	FILE *input_file = stdin; // default input file
	FILE *output_file = stdout; // default output file

	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
		case 't': // Make Universes toroidal
			toroidal = true;

			break;
		case 's': // Don't display to screen
			silence_ncurses = true;

			break;
		case 'n': // Set generations
			generations = strtoul(optarg, NULL, 10);

			break;
		case 'i': // Set input file
			input_file = fopen(optarg, "r");

			break;
		case 'o': // Set output file
			output_file = fopen(optarg, "w");

			break;

		default: // Print help message during invalid run
			printf("./life_amd64: invalid option -- 'H'\n");
			printf("SYNOPSIS\n");
			printf("\tConway's Game of Life\n");
			printf("USAGE\n");
			printf("\t./life_amd64 tsn:i:o:h\n");
			printf("OPTIONS\n");
			printf("\t-t             Create your universe as a toroidal\n");
			printf("\t-s             Silent - do not use animate the evolution\n");
			printf("\t-n {number}    Number of generations [default: 100]\n");
			printf("\t-i {file}      Input file [default: stdin]\n");
			printf("\t-o {file}      Output file [default: stdout]\n");
			return 1;

			break;
		}

	}

	// Gets size of the Universe from input file
	fscanf(input_file, "%u %u", &rows, &cols);

	// Create Universes A and B
	A = uv_create(rows, cols, toroidal);
	B = uv_create(rows, cols, toroidal);

	// Populate Universe A
	if (!uv_populate(A, input_file)) {
	        printf("Universe failed to be populated");
	}

	// Setting ncurses screen
	initscr();
	curs_set(FALSE);

	for (uint32_t gen = 0; gen < generations; gen++) {
		if (!silence_ncurses) {
			clear();
			display_universe(A);
			refresh();
			usleep(DELAY);
		}

		perform_generation(A, B);
		swap_universes(A, B);

	}

	endwin();

	// Write Universe A (once Universe B) to output file
	uv_print(A, output_file);

	// Deallocate Universes A and B
	uv_delete(A);
	uv_delete(B);

	// Close input and output files
	fclose(input_file);
	fclose(output_file);

	return 0;
}
