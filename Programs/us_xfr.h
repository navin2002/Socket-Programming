#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define SV_SOCK_PATH "/tmp/us_xfr"

#define BUF_SIZE 100
