# FakeCharDev

## How to run example.c to read and write to the fake_char_dev

1. First run the "make" command to compile fake_char_dev.c
   
2. Then run the following to load the module. Now you should see fake_char_dev0 appear in the /dev folder
   and dev_class appear in /sys/class
   ```
   sudo insmod fake_char_dev.ko
   ```
   
3. Now, compile the example usage file as follows:
   ```
   gcc -o example example.c
   ```
   
4. Then run the example
   ```
   ./example
   ```
   
5. Once done testing the character driver, unload the module as follows:
   ```
   sudo rmmod fake_char_dev
   ```
   
## Note

In case you see the "Failed to open device file!" message when running example.c, there may be an issue with the 
permissions of the device file. Follow the steps below to check:

1. Verify the permissions of the device file:
   ```
   ls -l /dev/fake_char_dev0
   ```
   The output should show permissions similar to
   ```
   crw-rw-rw- 1 root root 243, 0 May 28 12:34 /dev/fake_char_dev0
   ```

2. If the permissions are not the same as shown above, run the following to modify the permissions:
   ```
   sudo chmod 666 /dev/fake_char_dev0
   ```

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
