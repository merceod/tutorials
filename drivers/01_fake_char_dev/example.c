
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

// Command structure
typedef struct command {
  uint32_t Op;
  int32_t  A;
  int32_t  B;
  int32_t  Result;
} command_t;

// Some Useful Constants
#define CMD_ADD   1
#define CMD_SUB   2

// Captures errors
extern int errno;

int main()
{
  int fd;
  command_t cmd = {0, 0, 0, 0};
  ssize_t count;

  // Open character device file
  fd = open("/dev/fake_char_dev0", O_RDWR);
  if (fd < 0) {
    printf("Failed to open device file!\n");
    return 0;
  }

  // Provision command
  cmd.Op  = CMD_ADD;
  cmd.A   = 100;
  cmd.B   = 20;

  // Write Computation to Device
  count = write(fd, (const void *)&cmd, sizeof(cmd));
  if (count < 0) {
    // check errno (read https://man7.org/linux/man-pages/man2/write.2.html)
    goto finally;
  }

  // Read results
  count = read(fd, (void *)&cmd, sizeof(cmd));
  if (count < 0) {
    // check errno (read https://man7.org/linux/man-pages/man2/read.2.html)
    goto finally;
  }

  // Perform checks on results
  // ...here...
  if(cmd.Op == CMD_ADD) {
    if(cmd.Result != cmd.A + cmd.B) {
        printf("Addition failed\n");
    }
  } else if(cmd.Op == CMD_SUB) {
    if(cmd.Result != cmd.A - cmd.B) {
        printf("Subtraction failed\n");
    }
  }

  printf("The result is: %d\n", cmd.Result);

  // close device file
finally:
  close(fd);

  return 0;
}
