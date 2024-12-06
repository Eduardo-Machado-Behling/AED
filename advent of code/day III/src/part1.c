#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  FILE *in = fopen("input.txt", "r");
  if (!in) {
    fprintf(stderr, "input file wasn't open for the following reason: \"%s\"\n",
            strerror(errno));
    return 1;
  }

  int ch = 0;
  int i = 0;
  int digitCount = 0;
  int currNumber = 0;
  const char fmt[] = "mul(,)";
  char digitStr[2][4] = {0};
  long res = 0;

  while ((ch = fgetc(in)) != EOF) {
    if (fmt[i] == '\0') {
      res += strtol(digitStr[0], NULL, 10) * strtol(digitStr[1], NULL, 10);
      i = 0;
      digitCount = 0;
      currNumber = 0;
    }

    if (i == 4 || i == 5) {
      if (ch == ',' || ch == ')') {
        i++;
        digitStr[currNumber++][digitCount] = '\0';
        digitCount = 0;
        continue;
      }
      if (!isdigit(ch) || digitCount > 3) {
        i = 0;
        digitCount = 0;
        currNumber = 0;
        continue;
      }
      digitStr[currNumber][digitCount++] = (char)ch;
    } else if (fmt[i] == ch) {
      i++;
    } else if (i) {
      i = 0;
    }
  }

  printf("res: %ld\n", res);

  fclose(in);
}
