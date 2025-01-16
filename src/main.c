#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// reads the size of the file given
uint64_t colfilesize(FILE *colorfile) {
  if (colorfile != NULL) {
    if (fseek(colorfile, 0, SEEK_END) != 0) {
      fprintf(stderr, "error");
      return 0;
    }
    uint64_t size = ftell(colorfile);
    if (fseek(colorfile, 0, SEEK_SET) != 0) {
      fprintf(stderr, "error");
      return 0;
    }
    return size;
  }
}

// reads the colors (contents) of the file as per the given directory
char **readcolors(uint64_t size, FILE *colorfile, int linenum) {
  char line[linenum];
  char **colors = malloc(size);
  int index = 0;

  if (colorfile != NULL) {
    while (fgets(line, linenum, colorfile)) {
      colors[index] = malloc(size);
      strcpy(colors[index], line);
      index++;
    }

    return colors;
  }

  else {
    printf("not able to open file\n");
  }
}

// appends the ghostty config file
void append(FILE *config, char **colors, int linenum) {
  char string[] = "palette"; // searches for initial palette definitions
  
  for (int i = 0; i < linenum; i++) {
    fprintf(config, "palette = %d=%s", i, colors[i]);
  }
  
  fprintf(config, "background = %s", colors[0]);
  fprintf(config, "foreground = %s", colors[15]);
}

void main() {
  char *dircolor = "/home/ishan/.cache/wal/colors";
  char *dirconfig = "/home/ishan/.config/ghostty/config";

  FILE *colorfile = fopen(dircolor, "r"); 
  FILE *config = fopen(dirconfig, "a");

  uint64_t size = colfilesize(colorfile); 
  int linenum = size / 8; 

  char **colors = readcolors(size, colorfile, linenum); 

  append(config, colors, linenum);

  fclose(colorfile);
  fclose(config);
}
