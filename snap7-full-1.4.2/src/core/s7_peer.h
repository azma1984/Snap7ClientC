/*=============================================================================|
|  PROJECT SNAP7                                                         1.3.0 |
|==============================================================================|
|  Copyright (C) 2013, 2015 Davide Nardella                                    |
|  All rights reserved.                                                        |
|==============================================================================|
|  SNAP7 is free software: you can redistribute it and/or modify               |
|  it under the terms of the Lesser GNU General Public License as published by |
|  the Free Software Foundation, either version 3 of the License, or           |
|  (at your option) any later version.                                         |
|                                                                              |
|  It means that you can distribute your commercial software linked with       |
|  SNAP7 without the requirement to distribute the source code of your         |
|  application and without the requirement that your application be itself     |
|  distributed under LGPL.                                                     |
|                                                                              |
|  SNAP7 is distributed in the hope that it will be useful,                    |
|  but WITHOUT ANY WARRANTY; without even the implied warranty of              |
|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               |
|  Lesser GNU General Public License for more details.                         |
|                                                                              |
|  You should have received a copy of the GNU General Public License and a     |
|  copy of Lesser GNU General Public License along with Snap7.                 |
|  If not, see  http://www.gnu.org/licenses/                                   |
|=============================================================================*/
#ifndef s7_peer_h
#define s7_peer_h
//---------------------------------------------------------------------------
#include "s7_types.h"
#include "s7_isotcp.h"
#include <stdbool.h>
//---------------------------------------------------------------------------

//const longword errPeerMask 	     = 0xFFF00000;
//const longword errPeerBase       = 0x000FFFFF;
extern const longword errNegotiatingPDU;

word cntword;

bool Destroying;
PS7ReqHeader PDUH_out;
word TSnap7Peer_GetNextWord();
int TSnap7Peer_SetError(int Error);
int TSnap7Peer_NegotiatePDULength();
void TSnap7Peer_ClrError();

int LastError;
int PDULength;
int PDURequest;
void TSnap7Peer_init();
void TSnap7Peer_deinit();
void TSnap7Peer_PeerDisconnect();
int TSnap7Peer_PeerConnect();

//---------------------------------------------------------------------------
#endif
