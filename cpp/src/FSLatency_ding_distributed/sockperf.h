#ifndef SOCK_PERF_H_
#define SOCK_PERF_H_



#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netdb.h>
#include <errno.h>

#define WSAEWOULDBLOCK  EWOULDBLOCK
#define SOCKET_ERROR    -1
#define WINAPI 
#define INVALID_SOCKET  -1

#define closesocket(s)  close(s)
#define InitSocket()
#define WSACleanup()
typedef int BOOL;
#define TRUE    1
#define FALSE   0
typedef const char * PCSTR;
typedef int SOCKET;
typedef void * PVOID;
typedef char * PSTR;
typedef unsigned char * PBYTE;
typedef unsigned long DWORD;
typedef long long INT64;

#define in_addr_long    s_addr

#define __cdecl


#pragma pack( push, 1 )

typedef enum
{
    SOCKET_READ,
    SOCKET_WRITE,
    SOCKET_R_THEN_W,
    SOCKET_W_THEN_R,
}Eu_SocketCmdType;

typedef struct 
{
    int OpCmd;  //  See Eu_SocketCmdType
    int nLoopCount;
    int nBlockSize1;
    int nBlockSize2;
}St_TestSet;

#ifndef ST_TESTRESULT_STRUCT
typedef struct
{
    int nElapsedTime;
    int nTransferRate;
}St_TestResult;

#define ST_TESTRESULT_STRUCT
#endif

#pragma pack( pop )

#endif
