#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

// get directory of the color file
char *colorfolder(const char *home) {
  const char *dir = "/.cache/wal/colors";
  const int size = strlen(home) + strlen(dir) + 1;

  char *dircolor = malloc(size);

  if (dircolor == NULL) {
      puts("malloc failed");
      return NULL;
  }

  snprintf(dircolor, size, "%s%s", home, dir);
  return dircolor;
}

// get directory of the config file
char *confolder(const char *home) {
  const char *dir = "/.config/ghostty/config";
  const int size = strlen(home) + strlen(dir) + 1;

  char *dirconf = malloc(size);

  if (dirconf == NULL) {
      puts("malloc failed");
      return NULL;
  }

  snprintf(dirconf, size, "%s%s", home, dir);
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
    char ch = fgetc(file);

    if (ch == '\n') {
      lines++;
    }
  }

  fseek(file, 0, SEEK_SET);
  return lines;
}

// reads the contents of the file as per the given directory
char **readcolors(uint64_t size, FILE *file) {
  char buffline[1024];
  char **buffer = malloc(size);

  int index = 0;

  if (file != NULL) {
    while (fgets(buffline, sizeof(buffline), file)) {
      buffer[index] = malloc(sizeof(buffline));
      strcpy(buffer[index], buffline);

      index++;
    }

    return buffer;
  }

  else {
    puts("not able to open colors file");
  }

  free(buffer);
  return NULL;
}

char **readoptions(FILE *file, uint64_t size, uint64_t numlines, int *index) {
  size_t count = 0;

  // temporarily stores the file line  
  char store[1024];
  char line[numlines][1024];

  char buffline[numlines][1024];
  char **buffer = malloc(size);

  while (fgets(line[count], sizeof(line), file)) {
    count++;
  }

  for (size_t i = 0; i < count; i++) {
    char *src = line[i];
    char *dst = line[i];

    while (*src) {
      if (!isspace(*src)) {
        *dst++ = *src;
      }
      src++;
    }

    *dst = '\0';
  }

  for (size_t i = 0; i < count; i++) {
    if (!strstr(line[i], "palette=") && 
        !strstr(line[i], "background=") && 
        !strstr(line[i], "foreground=")) 
    {
      strcpy(buffline[i], line[i]);
      *index = i;
    }
  }

  for (size_t i = 0; i < count; i++) {
    if (buffline[i] != NULL) {
      buffer[i] = buffline[i];
    }
  }

  return buffer;
}

// rewrite the ghostty config file
void rewrite(FILE *confile, char **options, int *index, char **colors, int colorlines) {
  if (confile != NULL) {
    for (int i = 0; i <= *index; i++) {
      if (strlen(options[i]) > 0) {
        fprintf(confile, "%s\n", options[i]);
      }
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

int main(void) {
  const char *home = getenv("HOME");
  char *dircolor = colorfolder(home);
  char *dirconf = confolder(home);
  
  FILE *colorfile = fopen(dircolor, "r"); 

  uint64_t colorsize = filesize(colorfile); 
  uint64_t colorlines = numlines(colorfile); 

  char **colors = readcolors(colorsize, colorfile); 

  FILE *rconfile = fopen(dirconf, "r");

  // initialize the pointer
  int value; 
  int *index = &value; 

  uint64_t confsize = filesize(rconfile);
  uint64_t conflines = numlines(rconfile);

  // stores all previous options of the config file
  char **options = readoptions(rconfile, confsize, conflines, index);

  fclose(rconfile);

  FILE *confile = fopen(dirconf, "w");
  rewrite(confile, options, index, colors, colorlines);

  fclose(colorfile);
  fclose(confile);
}
