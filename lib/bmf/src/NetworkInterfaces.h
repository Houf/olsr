#ifndef _BMF_NETWORKINTERFACES_H
#define _BMF_NETWORKINTERFACES_H

/*
 * OLSR Basic Multicast Forwarding (BMF) plugin.
 * Copyright (c) 2005, 2006, Thales Communications, Huizen, The Netherlands.
 * Written by Erik Tromp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * * Neither the name of Thales, BMF nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* -------------------------------------------------------------------------
 * File       : NetworkInterfaces.h
 * Description: Functions to open and close sockets
 * Created    : 29 Jun 2006
 *
 * $Id: NetworkInterfaces.h,v 1.2 2007/02/10 17:05:56 bernd67 Exp $ 
 * ------------------------------------------------------------------------- */

/* System includes */
#include <netinet/in.h> /* struct in_addr */

/* Plugin includes */
#include "Packet.h" /* IFHWADDRLEN */

/* Size of buffer in which packets are received */
#define BMF_BUFFER_SIZE 2048

struct TBmfInterface
{
  /* File descriptor of raw packet socket, used for capturing multicast packets */
  int capturingSkfd;

  /* File descriptor of UDP (datagram) socket for encapsulated multicast packets. 
   * Only used for OLSR-enabled interfaces; set to -1 if interface is not OLSR-enabled. */
  int encapsulatingSkfd;

  unsigned char macAddr[IFHWADDRLEN];

  char ifName[IFNAMSIZ];

  /* OLSRs idea of this network interface. NULL if this interface is not
   * OLSR-enabled. */
  struct interface* olsrIntf;

  /* IP address of this network interface */
  struct sockaddr intAddr;

  /* Broadcast address of this network interface */
  struct sockaddr broadAddr;

  #define FRAGMENT_HISTORY_SIZE 10
  struct TFragmentHistory
  {
    u_int16_t ipId;
    u_int8_t ipProto;
    struct in_addr ipSrc;
    struct in_addr ipDst;
  } fragmentHistory [FRAGMENT_HISTORY_SIZE];

  int nextFragmentHistoryEntry;

  /* Next element in list */
  struct TBmfInterface* next; 
};

extern struct TBmfInterface* BmfInterfaces;

extern int HighestSkfd;
extern fd_set InputSet;

extern int EtherTunTapFd;

extern char EtherTunTapIfName[];

extern u_int32_t EtherTunTapIp;
extern u_int32_t EtherTunTapIpMask;
extern u_int32_t EtherTunTapIpBroadcast;

extern int CapturePacketsOnOlsrInterfaces;

enum TTunOrTap { TT_TUN = 0, TT_TAP };
extern enum TTunOrTap TunOrTap;

int SetBmfInterfaceName(const char* ifname);
int SetBmfInterfaceType(const char* iftype);
int SetBmfInterfaceIp(const char* ip);
int SetCapturePacketsOnOlsrInterfaces(const char* enable);
int CreateBmfNetworkInterfaces(struct interface* skipThisIntf);
void AddInterface(struct interface* newIntf);
void CloseBmfNetworkInterfaces(void);
int AddNonOlsrBmfIf(const char* ifName);
int IsNonOlsrBmfIf(const char* ifName);
void CheckAndUpdateLocalBroadcast(unsigned char* buffer, struct sockaddr* broadAddr);

#endif /* _BMF_NETWORKINTERFACES_H */