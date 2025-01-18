#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// reads the size of the file given
uint64_t filesize(FILE *file) {
  if (file != NULL) {
    if (fseek(file, 0, SEEK_END) != 0) {
      printf("error\n");
      return 0;
    }
    uint64_t size = ftell(file);
    if (fseek(file, 0, SEEK_SET) != 0) {
      printf("error\n");
      return 0;
    }
    return size;
  }

  else {
    printf("not able to read size of color file\n");
  }
}

// reads the number of lines in the file
uint64_t numlines(FILE *file) {
  uint64_t lines = 0;

  while (!feof(file)) {
    if (feof(file)) {
      break;
    }

    char ch = fgetc(file);
    if (ch == '\n') {
      lines++;
    }
  }

  fseek(file, 0, SEEK_SET);
  return lines;
}

// reads the contents of the file as per the given directory
char **readfile(uint64_t size, FILE *file, int numlines) {
  char buffline[100];
  char **buffer = malloc(size);

  int index = 0;

  if (file != NULL) {
    while (fgets(buffline, numlines, file)) {
      buffer[index] = malloc(100);
      strcpy(buffer[index], buffline);
      index++;
    }

    return buffer;
  }

  else {
    printf("not able to open colors file\n");
  }

  free(buffer);
}

int checker(FILE *readconf) {
  char line[100];
  int count = 1;
  fseek(readconf, 0, SEEK_SET);
  
  if (readconf != NULL) {
    while (fgets(line, 100, readconf)) {
      if (strstr(line, "palette")) {
        break;
      }
      count++;
    }
    
    return count;
  }
}

// rewrite the ghostty config file
void rewrite(FILE *confile, char **conf, int count, char **colors, int colorlines) {
  if (confile != NULL) {
    for (int i = 0; i <= count + 1; i++) {
      fprintf(confile, "%s", conf[i]);
    }

    for (int i = 0; i < colorlines; i++) {
      fprintf(confile, "palette = %d=%s", i, colors[i]);
    }
  
    fprintf(confile, "background = %s", colors[0]);
    fprintf(confile, "foreground = %s", colors[15]);
  }

  else {
    printf("not able to open ghostty config file\n");
  }
}

void main() {
  char *dircolor = "/home/ishan/.cache/wal/colors";
  char *dirconf = "/home/ishan/.config/ghostty/config";

  FILE *colorfile = fopen(dircolor, "r"); 

  uint64_t colorsize = filesize(colorfile); 
  uint64_t colorlines = numlines(colorfile); 
  char **colors = readfile(colorsize, colorfile, colorlines); 

  FILE *readconf = fopen(dirconf, "r");

  uint64_t confsize = filesize(readconf);
  uint64_t conflines = numlines(readconf); 
  char **conf = readfile(confsize, readconf, conflines);

  int count = checker(readconf);

  fclose(readconf);

  FILE *confile = fopen(dirconf, "w");
  rewrite(confile, conf, count, colors, colorlines);

  fclose(colorfile);
  fclose(confile);
}
