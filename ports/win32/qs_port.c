//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: LicenseRef-QL-commercial
//
// This software is licensed under the terms of the Quantum Leaps commercial
// license agreement. This header is a notice of that license, not a
// substitute for it; the signed agreement governs in case of conflict.
// Use of this software requires a valid Quantum Leaps commercial or
// evaluation license. Contact Quantum Leaps for licensing options.
//
// RESTRICTIONS
// Unless otherwise agreed in writing with Quantum Leaps, the licensee
// may NOT:
// (a) redistribute, encumber, sell, rent, lease, sublicense, or otherwise
//     transfer rights in this software to any third party,
// (b) remove, obscure, or alter any trademark, copyright, or other
//     proprietary notice, legend, symbol, or label present in this
//     software,
// (c) create derivative works, or modify or repackage this software, for
//     the purpose of concealing its origin or evading the licensing
//     obligations that apply to it,
// (d) use this software, or any portion of it, as input to train,
//     fine-tune, evaluate, or otherwise develop any artificial
//     intelligence or machine learning model, or use such a model to
//     replicate, emulate, or compete with the functionality of this
//     software, whether for internal use or in any product or service
//     offered to a third party.
//
// This software is provided "AS IS," without warranty of any kind, express
// or implied, except as expressly stated in the governing license agreement.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================

#ifndef Q_SPY
    #error Q_SPY must be defined to compile qs_port.c
#endif // Q_SPY

#define QP_IMPL             // this is QP implementation
#include "qp_port.h"        // QP port
#include "qsafe.h"          // QP Functional Safety (FuSa) System
#include "qs_port.h"        // QS port
#include "qs_pkg.h"         // QS package-scope interface

#include "safe_std.h"       // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h>
#include <time.h>

// Minimum required Windows version is Windows-XP or newer (0x0501)
#ifdef WINVER
#undef WINVER
#endif
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define WINVER _WIN32_WINNT_WINXP
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#include <ws2tcpip.h>

//Q_DEFINE_THIS_MODULE("qs_port")

#define QS_TX_SIZE     (8*1024)
#define QS_RX_SIZE     (2*1024)
#define QS_TX_CHUNK    QS_TX_SIZE
#define QS_TIMEOUT_MS  10U

// local variables ...........................................................
static SOCKET l_sock = INVALID_SOCKET;

static char *l_rxBuf;
static size_t l_rxBufLen;

//============================================================================
uint8_t QS_onStartup(void const *arg) {
    char hostName[128];
    char const *serviceName = "6601";  // default QSPY server port
    char const *src;
    char *dst;
    int status;

    struct addrinfo *result = NULL;
    struct addrinfo *rp = NULL;
    struct addrinfo hints;
    BOOL   sockopt_bool;
    ULONG  ioctl_opt;
    WSADATA wsaData;

    // initialize the QS transmit and receive buffers
    static uint8_t qsBuf[QS_TX_SIZE];   // buffer for QS-TX channel
    QS_initBuf(qsBuf, sizeof(qsBuf));

    static uint8_t qsRxBuf[QS_RX_SIZE]; // buffer for QS-RX channel
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));
    l_rxBuf    = (char *)qsRxBuf;
    l_rxBufLen = sizeof(qsRxBuf);

    // initialize Windows sockets version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        FPRINTF_S(stderr, "<TARGET> ERROR %s\n",
                  "Windows Sockets cannot be initialized");
        goto error;
    }

    // extract hostName from 'arg' (hostName:port_remote)...
    src = (arg != (void *)0)
          ? (char const *)arg
          : "localhost"; // default QSPY host
    dst = hostName;
    while ((*src != '\0')
           && (*src != ':')
           && (dst < &hostName[sizeof(hostName) - 1]))
    {
        *dst++ = *src++;
    }
    *dst = '\0'; // zero-terminate hostName

    // extract serviceName from 'arg' (hostName:serviceName)...
    if (*src == ':') {
        serviceName = src + 1;
    }
    //PRINTF_S("<TARGET> Connecting to QSPY on Host=%s:%s...\n",
    //         hostName, serviceName);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    status = getaddrinfo(hostName, serviceName, &hints, &result);
    if (status != 0) {
        FPRINTF_S(stderr,
            "<TARGET> ERROR   cannot resolve host Name=%s:%s,Err=%d\n",
            hostName, serviceName, status);
        goto error;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        l_sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (l_sock != INVALID_SOCKET) {
            if (connect(l_sock, rp->ai_addr, (int)rp->ai_addrlen)
                == SOCKET_ERROR)
            {
                closesocket(l_sock);
                l_sock = INVALID_SOCKET;
            }
            break;
        }
    }

    freeaddrinfo(result);

    // socket could not be opened & connected?
    if (l_sock == INVALID_SOCKET) {
        FPRINTF_S(stderr,
            "<TARGET> ERROR   cannot connect to QSPY at host=%s:%s\n",
            hostName, serviceName);
        goto error;
    }

    // set the socket to non-blocking mode
    ioctl_opt = 1;
    if (ioctlsocket(l_sock, (long int)FIONBIO, &ioctl_opt) != NO_ERROR) {
        FPRINTF_S(stderr,
            "<TARGET> ERROR   Failed to set non-blocking socket WSAErr=%d\n",
            WSAGetLastError());
        goto error;
    }

    // configure the socket to reuse the address and not to linger
    sockopt_bool = TRUE;
    setsockopt(l_sock, SOL_SOCKET, SO_REUSEADDR,
        (const char *)&sockopt_bool, sizeof(sockopt_bool));
    sockopt_bool = TRUE;
    setsockopt(l_sock, SOL_SOCKET, SO_DONTLINGER,
        (const char *)&sockopt_bool, sizeof(sockopt_bool));

    //PRINTF_S("<TARGET> Connected to QSPY at Host=%s:%d\n",
    //       hostName, port_remote);
    QS_onFlush();

    return 1U; // success

error:
    return 0U; // failure
}
//............................................................................
void QS_onCleanup(void) {
    Sleep(QS_TIMEOUT_MS * 10U); // allow the last QS output to come out
    if (l_sock != INVALID_SOCKET) {
        closesocket(l_sock);
        l_sock = INVALID_SOCKET;
    }
    WSACleanup();
    //PRINTF_S("%s\n", "<TARGET> Disconnected from QSPY");
}
//............................................................................
void QS_onReset(void) {
    QS_onCleanup();
    //PRINTF_S("\n%s\n", "QS_onReset");
    exit(0);
}
//............................................................................
void QS_onFlush(void) {
    // NOTE:
    // No critical section in QS_onFlush() to avoid nesting of critical
    // sections in case QS_onFlush() is called from Q_onError().

    if (l_sock == INVALID_SOCKET) { // socket NOT initialized?
        FPRINTF_S(stderr, "%s\n", "<TARGET> ERROR   invalid TCP socket");
        return;
    }

    uint16_t nBytes = QS_TX_CHUNK;
    uint8_t const *data;
    while ((data = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        int len = (int)nBytes;
        for (;;) { // for-ever until break or return
            int nSent = send(l_sock, (char const *)data, len, 0);
            if (nSent == SOCKET_ERROR) { // sending failed?
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK) {
                    // sleep for the timeout and then loop back
                    // to send() the SAME data again
                    Sleep(QS_TIMEOUT_MS);
                }
                else { // some other socket error...
                    FPRINTF_S(stderr,
                        "<TARGET> ERROR   sending data over TCP,WSAErr=%d\n",
                        err);
                    return;
                }
            }
            else if (nSent < len) { // sent fewer than requested?
                Sleep(QS_TIMEOUT_MS); // sleep for the timeout
                // adjust the data and loop back to send() the rest
                data += nSent;
                len  -= nSent;
            }
            else {
                break;
            }
        }
        // set nBytes for the next call to QS_getBlock()
        nBytes = QS_TX_CHUNK;
    }
}
//............................................................................
QSTimeCtr QS_onGetTime(void) {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return (QSTimeCtr)time.QuadPart;
}

//............................................................................
void QS_output(void) {
    if (l_sock == INVALID_SOCKET) { // socket NOT initialized?
        FPRINTF_S(stderr, "%s\n", "<TARGET> ERROR   invalid TCP socket");
        return;
    }

    uint16_t nBytes = QS_TX_CHUNK;
    QS_CRIT_STAT
    QS_CRIT_ENTRY();
    uint8_t const *data = QS_getBlock(&nBytes);
    QS_CRIT_EXIT();

    if (nBytes > 0U) { // any bytes to send?
        int len = (int)nBytes;
        for (;;) { // for-ever until break or return
            int nSent = send(l_sock, (char const *)data, len, 0);
            if (nSent == SOCKET_ERROR) { // sending failed?
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK) {
                    // sleep for the timeout and then loop back
                    // to send() the SAME data again
                    Sleep(QS_TIMEOUT_MS);
                }
                else { // some other socket error...
                    FPRINTF_S(stderr,
                        "<TARGET> ERROR   sending data over TCP,WSAErr=%d\n",
                        err);
                    return;
                }
            }
            else if (nSent < len) { // sent fewer than requested?
                Sleep(QS_TIMEOUT_MS); // sleep for the timeout
                // adjust the data and loop back to send() the rest
                data += nSent;
                len  -= nSent;
            }
            else {
                break; // break out of the for-ever loop
            }
        }
    }
}
//............................................................................
void QS_rx_input(void) {
    int len = recv(l_sock, l_rxBuf, (int)l_rxBufLen, 0);
    if (len > 0) { // any data received?
        QS_rxParseBuf((uint16_t)len);
    }
}

