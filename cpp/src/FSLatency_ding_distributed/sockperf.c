#include "sockperf.h"
#include <stdio.h>

#ifndef WIN32

DWORD GetTickCount(void)
{
    static struct timeval s_stBegin;

    struct timeval tv;
    struct timezone tz;

    gettimeofday( &tv, &tz );

    if( s_stBegin.tv_sec == 0 )
    {
        s_stBegin = tv;
    }

    //printf( "tv.tv_sec: %d, tv.tv_usec: %d\n", tv.tv_sec, tv.tv_usec );

    return (tv.tv_sec - s_stBegin.tv_sec)*1000 + 
        (tv.tv_usec - s_stBegin.tv_usec)/1000;
}

#else
#endif

static int ReceiveData( SOCKET s, int nCount, PVOID pBuffer )
{
    do
    {
        int nReceived = recv( s, (PSTR)pBuffer, nCount, 0 );
        if( nReceived == -1 )
        {
            return -1;
        }

        pBuffer = (PBYTE)pBuffer + nReceived;
        nCount -= nReceived;
    }while( nCount != 0 );
    
    return 0;
}

static int SendData( SOCKET s, int nCount, PVOID pBuffer )
{
    do
    {
        int nSent = send( s, (PSTR)pBuffer, nCount, 0 );
        if( nSent == -1 )
        {
            return -1;
        }

        pBuffer = (PBYTE)pBuffer + nSent;
        nCount -= nSent;
    }while( nCount != 0 );
    
    return 0;
}

BOOL 
__cdecl
TestSocketClient(
               const struct sockaddr_in * pstAddress,
               Eu_SocketCmdType eSocketCmd, 
               int nBlockSize1,
               int nBlockSize2,
               int nLoopCount, 
               St_TestResult * pstTestResult )
{
    BOOL isDone;
    SOCKET Socket;

    //ASSERT( pstTestResult != NULL );

    //  Create a socket
    Socket = socket( AF_INET, SOCK_STREAM, 0 );
    if( Socket == INVALID_SOCKET )
    {
        return FALSE;
    }

    //  Connect the port at the server at the ip address
    if( connect( Socket, (struct sockaddr*)pstAddress,//(struct sockaddr*)&SocketAddress,
        sizeof(struct sockaddr_in) ) == 0 )
    {
        DWORD dwElapsedTime = 0;
        int nTransferRate = 0;
        int nReceived;
        
        St_TestSet stTestSet;

        stTestSet.OpCmd = eSocketCmd;

        stTestSet.nBlockSize1 = nBlockSize1;
        stTestSet.nBlockSize2 = nBlockSize2;
        stTestSet.nLoopCount = nLoopCount;

        if( SendData( Socket, sizeof(stTestSet),  &stTestSet ) == -1 )
        {
            return -1;
        }

        {
            int i;
            PBYTE pBuffer, pRecvBuffer, pSendBuffer;
            DWORD dwBeginTime;

            switch(eSocketCmd)
            {
            case SOCKET_READ:
                pBuffer = (PBYTE)malloc( nBlockSize1 );
                if( pBuffer == NULL )
                {
                    return -1;
                }

                dwBeginTime = GetTickCount();

                for( i = 0; i < nLoopCount; i ++ )
                {
                    nReceived = ReceiveData( Socket, nBlockSize1, pBuffer );
                    if( nReceived == -1 )
                    {
                        //  The socket has an error, so quit this thread
                        printf( "An error occurs at socket %d !\n", Socket );
                        break;
                    }
                }

                dwElapsedTime = GetTickCount() - dwBeginTime;
                if( dwElapsedTime != 0 )
                {
                    nTransferRate = (int)((INT64)nLoopCount*
                        nBlockSize1*1000/dwElapsedTime);
                }

                free( pBuffer );
                break;
            case SOCKET_WRITE:
                pBuffer = (PBYTE)malloc( nBlockSize1 );
                if( pBuffer == NULL )
                {
                    return -1;
                }

                dwBeginTime = GetTickCount();

                for( i = 0; i < nLoopCount; i ++ )
                {
                    int nSent = SendData( Socket, nBlockSize1, pBuffer );
                    if( nSent == -1 )
                    {
                        //  The socket has an error, so quit this thread
                        printf( "An error occurs at socket %d !\n", Socket );
                        break;
                    }
                }

                dwElapsedTime = GetTickCount() - dwBeginTime;
                if( dwElapsedTime != 0 )
                {
                    nTransferRate = (int)((INT64)nLoopCount*
                        nBlockSize1*1000/dwElapsedTime);
                }

                free( pBuffer );
                break;
            case SOCKET_R_THEN_W:
                pRecvBuffer = (PBYTE)malloc( nBlockSize1 );
                if( pRecvBuffer == NULL )
                {
                    return -1;
                }

                pSendBuffer = (PBYTE)malloc( nBlockSize2 );
                if( pSendBuffer == NULL )
                {
                    free( pRecvBuffer );
                    return -1;
                }

                dwBeginTime = GetTickCount();

                for( i = 0; i < nLoopCount; i ++ )
                {
                    int nSent;

                    nReceived = ReceiveData( Socket, nBlockSize1, pRecvBuffer );
                    if( nReceived == -1 )
                    {
                        //  The socket has an error, so quit this thread
                        printf( "An error occurs at socket %d !\n", Socket );
                        break;
                    }

                    nSent = SendData( Socket, nBlockSize2, pSendBuffer );
                    if( nSent == -1 )
                    {
                        //  The socket has an error, so quit this thread
                        printf( "An error occurs at socket %d !\n", Socket );
                        break;
                    }
                }

                dwElapsedTime = GetTickCount() - dwBeginTime;
                if( dwElapsedTime != 0 )
                {
                    nTransferRate = (int)((INT64)nLoopCount*
                        (nBlockSize1+nBlockSize2)*1000/dwElapsedTime);
                }

                free( pRecvBuffer );
                free( pSendBuffer );
                break;
            case SOCKET_W_THEN_R:
                pSendBuffer = (PBYTE)malloc( nBlockSize1 );
                if( pSendBuffer == NULL )
                {
                    return -1;
                }

                pRecvBuffer = (PBYTE)malloc( nBlockSize2 );
                if( pRecvBuffer == NULL )
                {
                    free( pSendBuffer );
                    return -1;
                }

                dwBeginTime = GetTickCount();

                for( i = 0; i < nLoopCount; i ++ )
                {
                    int nSent;

                    nSent = SendData( Socket, nBlockSize1, pSendBuffer );
                    if( nSent == -1 )
                    {
                        //  The socket has an error, so quit this thread
                        printf( "An error occurs at socket %d !\n", Socket );
                        break;
                    }

                    nReceived = ReceiveData( Socket, nBlockSize2, pRecvBuffer );
                    if( nReceived == -1 )
                    {
                        //  The socket has an error, so quit this thread
                        printf( "An error occurs at socket %d !\n", Socket );
                        break;
                    }
                }

                dwElapsedTime = GetTickCount() - dwBeginTime;
                if( dwElapsedTime != 0 )
                {
                    nTransferRate = (int)((INT64)nLoopCount*
                        (nBlockSize1+nBlockSize2)*1000/dwElapsedTime);
                }

                free( pRecvBuffer );
                free( pSendBuffer );
                break;
            }
        }

        isDone = TRUE;
        pstTestResult->nElapsedTime = dwElapsedTime;
        pstTestResult->nTransferRate = nTransferRate;
    }
    else
    {
        isDone = FALSE;
    }

    //  Close the created socket
    closesocket( Socket );

    return isDone;
}
