#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <rpc/rpc.h>

#include "server.h"
#include "queue.h"
#include "operandos.h"

#define MAX_BUFFER 1024

void * svc_thread_handler(void * args);