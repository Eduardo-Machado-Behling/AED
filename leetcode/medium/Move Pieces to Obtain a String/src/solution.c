#include "solution.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool canChange(const char *start, char *target) {
  int li = 0;
  int ri = 0;
  int dist = 0;

  for (int i = 0; target[i]; i++) {
    if (target[i] == 'R') {
      for (; start[ri] != '\0' && start[ri] != 'R'; ri++) {
        ;
      }
      dist = i - ri;
	  for (size_t j = 1; ri + j <= i && j <= dist; j++) {
		  if(start[ri + j] == 'L' || target[ri + j] == 'L'){
			  return false;
		  }
	  }
	  if(start[ri] == '\0' || dist < 0){
		return false;
	  }
	  ri++;
    } else if (target[i] == 'L'){
      for (; start[li] != '\0' && start[li] != 'L'; li++) {
		  if(li >= i && (start[li] == 'R' || target[li] == 'R')){
			return false;
		  }
      }
	  dist = i - li;
	  if(start[li++] == '\0' || dist > 0){
		return false;
	  }
	}
  }

  for (; start[ri]; ri++) {
	  if(start[ri] != '_' && start[ri] != 'L'){
		return false;
	  }
  }

  for (; start[li]; li++) {
	  if(start[li] != '_' && start[li] != 'R'){
		return false;
	  }
  }

  return true;
}
