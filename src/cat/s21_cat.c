#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // ??сломанная тема исправить

#define MAX_LINE_LENGTH 1024

void process_file(FILE *file, int b_flag, int n_flag, int s_flag, int t_flag,
                  int e_flag) {
  char line[MAX_LINE_LENGTH];
  int line_num = 1;
  int prev_empty = 0;

  while (fgets(line, sizeof(line), file)) {
    // для сжатия пустых стрко
    if (s_flag && strlen(line) == 1) {
      if (prev_empty) continue;
      prev_empty = 1;
    } else {
      prev_empty = 0;
    }

    // нумерация без пустых
    if (b_flag && strlen(line) > 1) {
      printf("%6d\t", line_num++);
    } else if (n_flag) {
      printf("%6d\t", line_num++);
    }

    // замена табуляции
    if (t_flag) {
      for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == '\t') {
          printf("^I");
        } else {
          putchar(line[i]);
        }
      }
    } else {
      fputs(line, stdout);
    }

    // конец строки
    if (e_flag) {
      size_t len = strlen(line);
      if (line[len - 1] == '\n') {
        printf("$");
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int b_flag = 0, n_flag = 0, s_flag = 0, t_flag = 0, e_flag = 0;
  int opt;

  while ((opt = getopt(argc, argv, "bnste")) != -1) {
    switch (opt) {
      case 'b':
        b_flag = 1;
        break;
      case 'n':
        n_flag = 1;
        break;
      case 's':
        s_flag = 1;
        break;
      case 't':
        t_flag = 1;
        break;
      case 'e':
        e_flag = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [-bnste] [file...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (optind < argc) {
    for (int i = optind; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
        perror(argv[i]);
        exit(EXIT_FAILURE);
      }
      process_file(file, b_flag, n_flag, s_flag, t_flag, e_flag);
      fclose(file);
    }
  } else {
    process_file(stdin, b_flag, n_flag, s_flag, t_flag, e_flag);
  }

  return 0;
}
