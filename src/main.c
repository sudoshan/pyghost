#include <stdio.h>
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
char **readcolors(uint64_t size, FILE *file, int numlines) {
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

char **readopts(uint64_t size, FILE *file, int numlines, int *index) {
  int count;
  char buffline[100];
  char **buffer = malloc(size);

  if (file != NULL) {
    while (fgets(buffline, numlines, file)) {
      if (!strstr(buffline, "palette") && 
          !strstr(buffline, "background") && 
          !strstr(buffline, "foreground")) 
      {
        buffer[count] = malloc(100);
        strcpy(buffer[count], buffline);

        *index = count;
        count++;
      }
    }

    return buffer;
  }

  else {
    printf("not able to open config file\n");
  }

  free(buffer);
}

// rewrite the ghostty config file
void rewrite(FILE *confile, char **opts, int *index, char **colors, int colorlines) {
  if (confile != NULL) {
    for (int i = 0; i <= *index; i++) {
      fprintf(confile, "%s", opts[i]);
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
  char **colors = readcolors(colorsize, colorfile, colorlines); 

  FILE *rconfile = fopen(dirconf, "r");

  int value; // to initialize index pointer
  int *index = &value;

  uint64_t confsize = filesize(rconfile);
  uint64_t conflines = numlines(rconfile); 

  // stores all previous options of the config file
  char **opts = readopts(confsize, rconfile, conflines, index);

  fclose(rconfile);

  FILE *confile = fopen(dirconf, "w");
  rewrite(confile, opts, index, colors, colorlines);

  fclose(colorfile);
  fclose(confile);
}
