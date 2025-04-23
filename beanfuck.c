/**
 * beanfuck: a cursed, bean-flavored brainfuck interpreter.
 *
 * Copyright (C) 2025 ezntek <eason@ezntek.com>
 *
 * This program is licensed under the Mozilla Public License, version 2.0.
 * Refer to the LICENSE.md file in the root of the project directory for
 * details. Alternatively, visit the OSI website for an online copy.
 */

#include <errno.h>
#include <stdio.h>

#include "3rdparty/asv/a_string.h"

#define DEBUG    1
#define SRCMAX   1024
#define MAXSIZE  30000
#define MAXLOOPS 256

#define INC(x) ((x + 1) % 255)
#define DEC(x) ((x + 254) % 255) // -1 + 255

static a_string src;
static char* cells; // heap allocated
static int loops[MAXLOOPS] = {0};
int loops_end = 0;
int pc = 0;
int ptr = 0;

void add_loop(int addr) {
    loops[loops_end++] = addr;
}

void del_loop(void) {
    loops[--loops_end] = 0;
}

void dbg(void) {
    if (DEBUG) {
        printf("pc=`%d` ptr=`%d` instr=`%c`", pc, ptr, src.data[pc]);
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

int main(int argc, char** argv) {
    argc--;
    argv++;

    if (argc < 1) {
        printf("=== enter your brainfuck source code, or provide a filename "
               "===\n > ");
        a_string buf = a_string_input(NULL);

        if (exists(buf.data)) {
            src = a_string_read_file(buf.data);
        } else {
            src = a_string_trim(&buf);
        }
        a_string_free(&buf);
    }

    printf("%s\n", src.data);

    a_string_free(&src);
    return 0;
}
