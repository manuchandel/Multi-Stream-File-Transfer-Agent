#ifndef HEADERS_AND_CONSTANT_H
#define HEADERS_AND_CONSTANT_H

#include <stdio.h>
#include <cmath>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 9009
#define HOST "192.168.0.102"

#define NEW_FILE "new"
#define TEMP_DIR ".filetransfer"
#define TEMP_FILE ".filetransfer/F0.temp"

#define MAX_STREAMS 3
#define SECONDS_TO_WAIT 3
#define FILE_NAME_LENGTH  50
#define MAX_BYTES 4096
using namespace std;

struct FragmentHeader{
  int part_no;
  int file_size;
  char filename[FILE_NAME_LENGTH]; //contains extension of file name
};

#endif
