#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// get directory of the color file
char *colorfolder(const char *home) {
  if (home == NULL) {
    puts("error, home");
  }

  const char *folder = "/.cache/wal/colors";
  const int size = strlen(home) + strlen(folder) + 1;

  char *dircolor = malloc(size);

  if (dircolor == NULL) {
      puts("malloc failed");
      return NULL;
  }

  snprintf(dircolor, size, "%s%s", home, folder);
  return dircolor;
}

// get directory of the config file
char *confolder(const char *home) {
  if (home == NULL) {
    puts("error, home");
  }

  const char *folder = "/.config/ghostty/config";
  const int size = strlen(home) + strlen(folder) + 1;

  char *dirconf = malloc(size);

  if (dirconf == NULL) {
      puts("malloc failed");
      return NULL;
  }

  snprintf(dirconf, size, "%s%s", home, folder);
  return dirconf;
}

// reads the size of the file given
uint64_t filesize(FILE *file) {
  if (file != NULL) {
    if (fseek(file, 0, SEEK_END) != 0) {
      puts("error");
      return 0;
    }
    uint64_t size = ftell(file);
    if (fseek(file, 0, SEEK_SET) != 0) {
      puts("error");
      return 0;
    }
    return size;
  }

  else {
    puts("not able to read size of color file");
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
    puts("not able to open colors file");
  }

  free(buffer);
}

char **readopts(uint64_t size, FILE *file, int numlines, int *index) {
  int count = 0;

  char buffline[100];
  char **buffer = malloc(size);

  if (buffer == NULL) {
    puts("malloc failed");
  }

  if (file != NULL) {
    while (fgets(buffline, numlines, file)) {
      if (!strstr(buffline, "palette =") && 
          !strstr(buffline, "background =") && 
          !strstr(buffline, "foreground =")) 
      {
        buffer[count] = malloc(100);

        if (buffer[count] == NULL) {
          puts("malloc failed");
        }

        strcpy(buffer[count], buffline);

        *index = count;
        count++;
      }
    }

    return buffer;
  }

  else {
    puts("not able to open config file");
  }
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
    puts("not able to open ghostty config file");
  }
}

void main() {
  const char *home = getenv("HOME");

  char *dircolor = colorfolder(home);
  char *dirconf = confolder(home);
  
  FILE *colorfile = fopen(dircolor, "r"); 

  uint64_t colorsize = filesize(colorfile); 
  uint64_t colorlines = numlines(colorfile); 

  char **colors = readcolors(colorsize, colorfile, colorlines); 

  FILE *rconfile = fopen(dirconf, "r");

  // initialize the pointer
  int value; 
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
