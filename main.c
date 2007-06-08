#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main(int argc, char **argv) {
  if(argc < 2 || argc > 3) {
    fputs(
      "Sel selects a section of a binary file.\n"
      "Usage: sel start [length]\n",
    stdout);
    return 1;
  }
  
  {
    unsigned long long start;
    unsigned long long length = 0; /* initialise to make GCC happy */
    int unlimited;
    
    setmode(STDIN_FILENO, O_BINARY);
    setmode(STDOUT_FILENO, O_BINARY);
    
    errno = 0;
    start = strtoull(argv[1], 0, 0);
    if(0 != errno) {
      perror("Start offset parameter");
      return 1;
    }
    
    if(argc < 3) {
      unlimited = 1;
    } else {
      unlimited = 0;
      length = strtoull(argv[2], 0, 0);
      if(0 != errno) {
        perror("Length parameter");
        return 1;
      }
    }
    
    {
      struct stat st;
      fstat(STDIN_FILENO, &st);
      if(S_ISREG(st.st_mode)) {
        _lseeki64(STDIN_FILENO, start, SEEK_SET);
        start = 0;
      }
    }
    
    if(!unlimited) {
      length += start;
    }
    
    while(unlimited || length > 0) {
      char buffer[BUFFER_SIZE];
      ssize_t count = read(STDIN_FILENO, buffer,
        unlimited || length > BUFFER_SIZE? BUFFER_SIZE: (ssize_t)length);
      if(0 == count) {
        break;
      }
      
      if(!unlimited) {
        length -= count;
      }
      
      if(start < count) {
        ssize_t offset = (ssize_t)start;
        while(offset < count) {
          offset += write(STDOUT_FILENO, buffer + offset, count - offset);
        }
        start = 0;
      } else {
        start -= count;
      }
    }
  }
  
  return 0;
}
