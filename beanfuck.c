/**
 * beanfuck: a cursed, bean-flavored brainfuck interpreter.
 *
 * Copyright (C) 2025 ezntek <eason@ezntek.com>
 *
 * This program is licensed under the Mozilla Public License, version 2.0.
 * Refer to the LICENSE.md file in the root of the project directory for
 * details. Alternatively, visit the OSI website for an online copy.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3rdparty/asv/asv.h"

#define DEBUG    0
#define SRCMAX   1024
#define MAXSIZE  30000
#define MAXLOOPS 256

#define INC(x, N) ((x + 1) % N)
#define DEC(x, N) ((x - 1 + N) % N)

#define LICENSE                                                                \
    "This Source Code Form is subject to the terms of the Mozilla Public\n"    \
    "License, v. 2.0. If a copy of the MPL was not distributed with this\n"    \
    "file, You can obtain one at http://mozilla.org/MPL/2.0/."

#define USAGE                                                                  \
    S_BOLD                                                                     \
    "beanfuck: a cursed, bean-flavored brainfuck interpreter.\n\n" S_END       \
    "usage: ./beanfuck [help|filename|brainfuck source code]\n\n" LICENSE "\n"

static a_string src;
static char* cells; // heap allocated
static u32 loops[MAXLOOPS] = {0};
u32 loops_end = 0;
usize pc = 0;
usize ptr = 0;

void add_loop(int addr) {
    loops[loops_end++] = addr;
}

void del_loop(void) {
    loops[--loops_end] = 0;
}

int top_loop(void) {
    return loops[loops_end - 1];
}

void dbg(void) {
    if (DEBUG) {
        printf("pc=`%zu` ptr=`%zu` instr=`%c`\n", pc, ptr, src.data[pc]);
    }
}

bool exists(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return false;
    } else {
        fclose(fp);
        return true;
    }
}

i32 main(int argc, char** argv) {
    argc--;
    argv++;

    a_string buf = {0};
    if (argc < 1) {
        printf(S_DIM
               "=== enter your brainfuck source code, or provide a filename "
               "===\n" S_BOLD " > " S_END);
        buf = a_string_input(NULL);
    } else {
        buf = astr(argv[0]);
        if (!strcmp(buf.data, "help")) {
            printf(USAGE);
            goto cleanup;
        }
    }

    if (exists(buf.data)) {
        src = a_string_read_file(buf.data);
    } else {
        src = a_string_trim(&buf);
    }
    a_string_free(&buf);

    cells = calloc(MAXSIZE, 1);

    while (pc < src.len) {
        dbg();
        switch (src.data[pc]) {
            case '<': {
                ptr = DEC(ptr, MAXSIZE);
            } break;
            case '>': {
                ptr = INC(ptr, MAXSIZE);
            } break;
            case '+': {
                cells[ptr] = INC(cells[ptr], 255);
            } break;
            case '-': {
                cells[ptr] = DEC(cells[ptr], 255);
            } break;
            case '.': {
                putchar(cells[ptr]);
            } break;
            case ',': {
                cells[ptr] = getchar();
            } break;
            case '[': {
                if (cells[ptr] == 0)
                    while (src.data[pc] != ']')
                        pc++;
                else
                    add_loop(pc);
            } break;
            case ']': {
                if (loops_end == 0)
                    panic("found closing bracket without opening bracket");

                if (cells[ptr] != 0)
                    pc = top_loop();
                else
                    del_loop();
            } break;
        }
        pc++;
    }

    a_string_free(&src);
    free(cells);

cleanup:
    puts("");
    return 0;
}
