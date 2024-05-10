# FakeCharDev

## Overview

In this project, we want to design a Linux device driver for a simple virtual character device called FakeCharDev.

## Goals

1. Create a character driver accessible through the standard open, write, read, close API
2. Add support for IOCTL

## Software Specification

FakeCharDev has the following interface from the perspective of user applications:

```c
typedef struct command {
  // Operation Type
  // Can be:
  // - 1 -> ADD(A, B)
  // - 2 -> SUB(A, B)
  uint32_t Op;

  // Operand A
  int32_t  A;

  // Operand B
  int32_t  B;

  // Result of operation
  int32_t  Result;
} command_t;
```

## Example Usage

```c

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
  fd = open("/dev/fakechardev0", O_RDWR);
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

  // close device file
finally:
  close(fd);

  return 0;
}

```
