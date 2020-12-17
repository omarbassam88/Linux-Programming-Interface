#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif // !BUF_SIZE

int main(int argc, char const *argv[]) {

  int inFd, outFd;
  char *buf[BUF_SIZE];
  ssize_t count;

  if (argc != 3 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s old-file new-file\n", argv[0]);
  }

  inFd = open(argv[1], O_RDONLY);
  if (inFd == -1) {
    errExit("Error Opening File %s.\n", argv[1]);
  }
  printf("%d\n", inFd);

  outFd =
      open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
  if (outFd == -1) {
    errExit("Error Opening File %s.\n", argv[2]);
  }

  while ((count = read(inFd, buf, BUF_SIZE)) > 0) {
    if (write(outFd, buf, count) != count) {
      fatal("could not write whole buffer.\n");
    }

    printf("Copying...\n");
  }

  if (count == -1) {
    fatal("Couldn't Read from file.\n");
  }

  if (close(inFd) == -1) {
    errExit("Error Closing Input File.\n");
  }

  if (close(outFd) == -1) {
    errExit("Error Closing Output File.\n");
  }

  exit(EXIT_SUCCESS);
}
