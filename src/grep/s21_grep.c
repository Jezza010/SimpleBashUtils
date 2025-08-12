#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void process_file(const char *filename, const char *pattern, int flags[],
                  int *total_count);

int main(int argc, char *argv[]) {
  int opt;
  int flags[6] = {0};  // флаги e i v c l n
  char *pattern = NULL;
  char patterns[MAX_LINE_LENGTH] = {0};  // для хранения нескольких e

  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        flags[0] = 1;
        if (pattern) {
          strcat(patterns, "|");
        }
        strcat(patterns, optarg);
        break;
      case 'i':
        flags[1] = 1;
        break;
      case 'v':
        flags[2] = 1;
        break;
      case 'c':
        flags[3] = 1;
        break;
      case 'l':
        flags[4] = 1;
        break;
      case 'n':
        flags[5] = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [-e pattern] [-ivcln] [file...]\n", argv[0]);
        return EXIT_FAILURE;
    }
  }

  if (!flags[0]) {
    if (optind < argc) {
      pattern = argv[optind++];
    } else {
      fprintf(stderr, "grep: No pattern provided\n");
      return EXIT_FAILURE;
    }
  } else {
    pattern = patterns;
  }

  int total_count = 0;
  if (optind == argc) {
    process_file("/dev/stdin", pattern, flags, &total_count);
  } else {
    for (int i = optind; i < argc; i++) {
      process_file(argv[i], pattern, flags, &total_count);
    }
  }

  return EXIT_SUCCESS;
}

void process_file(const char *filename, const char *pattern, int flags[],
                  int *total_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
    return;
  }

  char line[MAX_LINE_LENGTH];
  regex_t regex;
  int regex_flags = REG_EXTENDED | REG_NOSUB;
  if (flags[1]) {  // -i
    regex_flags |= REG_ICASE;
  }

  if (regcomp(&regex, pattern, regex_flags) != 0) {
    fprintf(stderr, "grep: Invalid regular expression: %s\n", pattern);
    fclose(file);
    return;
  }

  int line_num = 0, match_count = 0;
  while (fgets(line, sizeof(line), file)) {
    line_num++;
    int match = (regexec(&regex, line, 0, NULL, 0) == 0);
    if (flags[2]) {  // -v
      match = !match;
    }
    if (match) {
      match_count++;
      if (!flags[3] && !flags[4]) {  // без -c и -l
        if (flags[5]) {              // -n
          printf("%d:", line_num);
        }
        printf("%s", line);
      }
    }
  }

  if (flags[3]) {  // -c
    printf("%d\n", match_count);
  }

  if (flags[4] && match_count > 0) {  // -l
    printf("%s\n", filename);
  }

  regfree(&regex);
  fclose(file);
  *total_count += match_count;
}
