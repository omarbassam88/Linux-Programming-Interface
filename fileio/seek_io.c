#include "tlpi_hdr.h"
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]) {

  size_t len;
  off_t offset;
  int fd, ap, j;
  char *buf;
  ssize_t numRead, numWritten;

  fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  if (fd == -1) {
    errExit("opening");
  }

  // Loop through the arguments
  for (ap = 2; ap < argc; ap++) {
    // switch on the forst letter or the argument
    switch (argv[ap][0]) {
      // Case of Reading from file
    case 'r':
    case 'R':
      len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
      buf = malloc(len);
      if (buf == NULL)
        errExit("malloc");
      numRead = read(fd, buf, len);
      if (numRead == -1)
        errExit("read");

      if (numRead == 0) {
        printf("%s: end of file\n", argv[ap]);
      } else {
        printf("%s: ", argv[ap]);
        for (j = 0; j < numRead; j++) {
          if (argv[ap][0] == 'r') {
            printf("%c", isprint((unsigned char)buf[j]) ? buf[j] : '?');
          } else {
            printf("%02x ", (unsigned int)buf[j]);
          }
        }
        printf("\n");
      }

      free(buf);
      break;

      // Case of writeing to file
    case 'w':
      numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
      if (numWritten == -1) {
        errExit("write");
      }
      printf("%s wrote %ld bytes.\n", argv[ap], (long)numWritten);
      break;
    // Chang file offset
    case 's':
      offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
      if (lseek(fd, offset, SEEK_SET) == -1) {
        errExit("lseek");
      }
      printf("%s: seek succeeded\n", argv[ap]);
      break;

    default:
      cmdLineErr("Argument mus Start with [rRws] %s.\n", argv[ap]);
      break;
    }
  }

  exit(EXIT_SUCCESS);
}
