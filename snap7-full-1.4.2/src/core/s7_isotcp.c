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
#include "s7_isotcp.h"
//---------------------------------------------------------------------------


void TIsoTcpSocket_init()
{
	RecvTimeout = 3000; // Some old equipments are a bit slow to answer....
	RemotePort  = isoTcpPort;
	// These fields should be $0000 and in any case RFC says that they are not considered.
	// But some equipment...need a non zero value for the source reference.
	DstRef = 0x0000;
	SrcRef = 0x0100;
	// PDU size requested
	IsoPDUSize =1024;
	IsoMaxFragments=MaxIsoFragments;
	LastIsoError=0;
}
//---------------------------------------------------------------------------
void TIsoTcpSocket_deinit()
{
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_CheckPDU(void *pPDU, u_char PduTypeExpected)
{
	PIsoHeaderInfo Info;
	int Size;
	TIsoTcpSocket_ClrIsoError();
	if (pPDU!=0)
	{
		Info = (PIsoHeaderInfo)(pPDU);
		Size = TIsoTcpSocket_PDUSize(pPDU);
		// Performs check
		if (( Size<7 ) || ( Size>IsoPayload_Size ) ||  // Checks RFC 1006 header length
			( Info->HLength<sizeof( TCOTP_DT )-1 ) ||  // Checks ISO 8073 header length
			( Info->PDUType!=PduTypeExpected))         // Checks PDU Type
			return TIsoTcpSocket_SetIsoError(errIsoInvalidPDU);
		else
			return noError;
	}
	else
		return TIsoTcpSocket_SetIsoError(errIsoNullPointer);
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_SetIsoError(int Error)
{
	LastIsoError = Error | LastTcpError;
	return LastIsoError;
}
//---------------------------------------------------------------------------
void TIsoTcpSocket_ClrIsoError()
{
		LastIsoError=0;
		LastTcpError=0;
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_BuildControlPDU()
{
	int ParLen, IsoLen;

	TIsoTcpSocket_ClrIsoError();
	FControlPDU.COTP.Params.PduSizeCode=0xC0; // code that identifies TPDU size
	FControlPDU.COTP.Params.PduSizeLen =0x01; // 1 byte this field
	switch(IsoPDUSize)
	{
		case 128:
			FControlPDU.COTP.Params.PduSizeVal =0x07;
			break;
		case 256:
			FControlPDU.COTP.Params.PduSizeVal =0x08;
			break;
		case 512:
			FControlPDU.COTP.Params.PduSizeVal =0x09;
			break;
		case 1024:
			FControlPDU.COTP.Params.PduSizeVal =0x0A;
			break;
		case 2048:
			FControlPDU.COTP.Params.PduSizeVal =0x0B;
			break;
		case 4096:
			FControlPDU.COTP.Params.PduSizeVal =0x0C;
			break;
		case 8192:
			FControlPDU.COTP.Params.PduSizeVal =0x0D;
			break;
		default:
			FControlPDU.COTP.Params.PduSizeVal =0x0B;  // Our Default
	};
	// Build TSAPs
	FControlPDU.COTP.Params.TSAP[0]=0xC1;   // code that identifies source TSAP
	FControlPDU.COTP.Params.TSAP[1]=2;      // source TSAP Len
	FControlPDU.COTP.Params.TSAP[2]=(SrcTSap>>8) & 0xFF; // HI part
	FControlPDU.COTP.Params.TSAP[3]=SrcTSap & 0xFF; // LO part

	FControlPDU.COTP.Params.TSAP[4]=0xC2; // code that identifies dest TSAP
	FControlPDU.COTP.Params.TSAP[5]=2;    // dest TSAP Len
	FControlPDU.COTP.Params.TSAP[6]=(DstTSap>>8) & 0xFF; // HI part
	FControlPDU.COTP.Params.TSAP[7]=DstTSap & 0xFF; // LO part

	// Params length
	ParLen=11;            // 2 Src TSAP (Code+field Len)      +
							// 2 Src TSAP len                   +
							// 2 Dst TSAP (Code+field Len)      +
							// 2 Src TSAP len                   +
							// 3 PDU size (Code+field Len+Val)  = 11
	// Telegram length
	IsoLen=sizeof(TTPKT)+ // TPKT Header
			7 +           // COTP Header Size without params
			ParLen;       // COTP params

	FControlPDU.TPKT.Version  =isoTcpVersion;
	FControlPDU.TPKT.Reserved =0;
	FControlPDU.TPKT.HI_Lenght=0; // Connection Telegram size cannot exced 255 bytes, so
									// this field is always 0
	FControlPDU.TPKT.LO_Lenght=IsoLen;

	FControlPDU.COTP.HLength  =ParLen + 6;  // <-- 6 = 7 - 1 (COTP Header size - 1)
	FControlPDU.COTP.PDUType  =pdu_type_CR; // Connection Request
	FControlPDU.COTP.DstRef   =DstRef;      // Destination reference
	FControlPDU.COTP.SrcRef   =SrcRef;      // Source reference
	FControlPDU.COTP.CO_R     =0x00;        // Class + Option : RFC0983 states that it must be always 0x40
											// but for some equipment (S7) must be 0 in disaccord of specifications !!!
	return noError;
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_PDUSize(void *pPDU)
{
	PIsoHeaderInfo iso_header_info =(PIsoHeaderInfo)(pPDU);
	return iso_header_info->TPKT.HI_Lenght*256+iso_header_info->TPKT.LO_Lenght;
}
//---------------------------------------------------------------------------
//void TIsoTcpSocket_IsoParsePDU(TIsoControlPDU pdu)
//{
// Currently we accept a connection with any kind of src/dst tsap
// Override to implement special filters.
//}
//---------------------------------------------------------------------------
//int TIsoTcpSocket_IsoConfirmConnection(u_char PDUType)
//{
//    PIsoControlPDU CPDU = PIsoControlPDU(&PDU);
//	u_short TempRef;

//	TIsoTcpSocket_ClrIsoError();
//	PDU.COTP.PDUType=PDUType;
//	// Exchange SrcRef<->DstRef, not strictly needed by COTP 8073 but S7PLC as client needs it.
//	TempRef=CPDU->COTP.DstRef;
//	CPDU->COTP.DstRef=CPDU->COTP.SrcRef;
//	CPDU->COTP.SrcRef=0x0100;//TempRef;

//	return TMsgSocket_SendPacket(&PDU,PDUSize(&PDU));
//}
//---------------------------------------------------------------------------
void TIsoTcpSocket_FragmentSkipped(int Size)
{
// override for log purpose
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_isoConnect()
{
	pbyte TmpControlPDU;
		PIsoControlPDU ControlPDU;
	u_int Length;
	int Result;

	// Build the default connection telegram
	TIsoTcpSocket_BuildControlPDU();
	ControlPDU =&FControlPDU;

	// Checks the format
	Result =TIsoTcpSocket_CheckPDU(ControlPDU, pdu_type_CR);
	if (Result!=0)
		return Result;

	Result =TMsgSocket_SckConnect();
	if (Result==noError)
	{
		// Calcs the length
		Length =TIsoTcpSocket_PDUSize(ControlPDU);
		// Send connection telegram
		TMsgSocket_SendPacket(ControlPDU, Length);
		if (LastTcpError==0)
		{
			TmpControlPDU = (pbyte)(ControlPDU);
			// Receives TPKT header (4 bytes)
			TMsgSocket_RecvPacket(TmpControlPDU, sizeof(TTPKT));
			if (LastTcpError==0)
			{
				// Calc the packet length
				Length =TIsoTcpSocket_PDUSize(TmpControlPDU);
				// Check if it fits in the buffer and if it's greater then TTPKT size
				if ((Length<=sizeof(TIsoControlPDU)) && (Length>sizeof(TTPKT)))
				{
					// Points to COTP
					TmpControlPDU+=sizeof(TTPKT);
					Length -= sizeof(TTPKT);
					// Receives remainin bytes 4 bytes after
					TMsgSocket_RecvPacket(TmpControlPDU, Length);
					if (LastTcpError==0)
					{
						// Finally checks the Connection Confirm telegram
						Result =TIsoTcpSocket_CheckPDU(ControlPDU, pdu_type_CC);
						if (Result!=0)
							LastIsoError=Result;
					}
					else
						Result =TIsoTcpSocket_SetIsoError(errIsoRecvPacket);
				}
				else
					Result =TIsoTcpSocket_SetIsoError(errIsoInvalidPDU);
			}
			else
				Result =TIsoTcpSocket_SetIsoError(errIsoRecvPacket);
			// Flush buffer
			if (Result!=0)
				TMsgSocket_Purge();
		}
		else
			Result =TIsoTcpSocket_SetIsoError(errIsoSendPacket);

		if (Result!=0)
			TMsgSocket_SckDisconnect();
	}
	return Result;
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_isoSendBuffer(void *Data, int Size)
{
	int Result;
	u_int IsoSize;

		TIsoTcpSocket_ClrIsoError();
	// Total Size = Size + Header Size
	IsoSize =Size+DataHeaderSize;
	// Checks the length
	if ((IsoSize>0) && (IsoSize<=IsoFrameSize))
	{
		// Builds the header
		Result =0;
		// TPKT
		PDU.TPKT.Version  = isoTcpVersion;
		PDU.TPKT.Reserved = 0;
		PDU.TPKT.HI_Lenght= (((u_short)(IsoSize))>> 8) & 0xFF;
		PDU.TPKT.LO_Lenght= ((u_short)(IsoSize)) & 0xFF;
		// COPT
		PDU.COTP.HLength   =sizeof(TCOTP_DT)-1;
		PDU.COTP.PDUType   =pdu_type_DT;
		PDU.COTP.EoT_Num   =pdu_EoT;
		// Fill payload
		if (Data!=0) // Data=null ==> use internal buffer PDU.Payload
						memcpy(&PDU.Payload, Data, Size);
				// Send over TCP/IP
				TMsgSocket_SendPacket(&PDU, IsoSize);

				if (LastTcpError!=0)
						Result =TIsoTcpSocket_SetIsoError(errIsoSendPacket);
	}
	else
		Result =TIsoTcpSocket_SetIsoError(errIsoInvalidDataSize );
	return Result;
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_isoRecvBuffer(void *Data, int *Size)
{
	int Result;

	TIsoTcpSocket_ClrIsoError();
	*Size =0;
	Result =TIsoTcpSocket_isoRecvPDU(&PDU);
	if (Result==0)
	{
		*Size =TIsoTcpSocket_PDUSize( &PDU )-DataHeaderSize;
		if (Data!=0)  // Data=NULL ==> a child will consume directly PDY.Payload
						memcpy(Data, &PDU.Payload, *Size);
	}
	return Result;
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_isoExchangeBuffer(void *Data, int *Size)
{
	int Result;

		TIsoTcpSocket_ClrIsoError();
	Result =TIsoTcpSocket_isoSendBuffer(Data, *Size);
	if (Result==0)
		Result =TIsoTcpSocket_isoRecvBuffer(Data, Size);
	return Result;
}
//---------------------------------------------------------------------------
bool TIsoTcpSocket_IsoPDUReady()
{
//    TIsoTcpSocket_ClrIsoError();
//	return PacketReady(sizeof(TCOTP_DT));
}
//---------------------------------------------------------------------------
int TIsoTcpSocket_isoDisconnect(bool OnlyTCP)
{
//	int Result;

//    TIsoTcpSocket_ClrIsoError();
//	if (Connected)
//		TMsgSocket_Purge(); // Flush pending
//	LastIsoError=0;
//	// OnlyTCP true -> Disconnect Request telegram is not required : only TCP disconnection
//	if (!OnlyTCP)
//	{
//		// if we are connected -> we have a valid connection telegram
//		if (Connected)
//			FControlPDU.COTP.PDUType =pdu_type_DR;
//		// Checks the format
//		Result =TIsoTcpSocket_CheckPDU(&FControlPDU, pdu_type_DR);
//		if (Result!=0)
//			return Result;
//		// Sends Disconnect request
//		TMsgSocket_SendPacket(&FControlPDU, TIsoTcpSocket_PDUSize(&FControlPDU));
//		if (LastTcpError!=0)
//		{
//			Result =TIsoTcpSocket_SetIsoError(errIsoSendPacket);
//			return Result;
//		}
//	}
//	// TCP disconnect
//	TMsgSocket_SckDisconnect();
//	if (LastTcpError!=0)
//		Result =TIsoTcpSocket_SetIsoError(errIsoDisconnect);
//	else
//		Result =0;

//	return Result;
}
//---------------------------------------------------------------------------
//int TIsoTcpSocket_isoSendPDU(PIsoDataPDU Data)
//{
//	int Result;

//    TIsoTcpSocket_ClrIsoError();
//	Result=TIsoTcpSocket_CheckPDU(Data,pdu_type_DT);
//	if (Result==0)
//	{
//		TMsgSocket_SendPacket(Data,TIsoTcpSocket_PDUSize(Data));
//		if (LastTcpError!=0)
//			Result=TIsoTcpSocket_SetIsoError(errIsoSendPacket);
//	}
//    return Result;
//}
//------------------------------------------------------------------------------
int TIsoTcpSocket_isoRecvFragment(void *From, int Max, int *Size, bool *EoT)
{
	int DataLength;

	*Size =0;
	*EoT =false;
		byte PDUType;
		TIsoTcpSocket_ClrIsoError();
	// header is received always from beginning
	TMsgSocket_RecvPacket(&PDU, DataHeaderSize); // TPKT + COPT_DT
	if (LastTcpError==0)
	{
				PDUType=PDU.COTP.PDUType;
				switch (PDUType)
				{
			case pdu_type_CR:
			case pdu_type_DR:
				*EoT=true;
				break;
			case pdu_type_DT:
								*EoT = (PDU.COTP.EoT_Num & 0x80) == 0x80;  // EoT flag
				break;
			default:
				return TIsoTcpSocket_SetIsoError(errIsoInvalidPDU);
				}

		DataLength = TIsoTcpSocket_PDUSize(&PDU) - DataHeaderSize;
		if (TIsoTcpSocket_CheckPDU(&PDU, PDUType)!=0)
			return LastIsoError;
		// Checks for data presence
		if (DataLength>0)  // payload present
		{
			// Check if the data fits in the buffer
			if(DataLength<=Max)
			{
				TMsgSocket_RecvPacket(From, DataLength);
				if (LastTcpError!=0)
					return TIsoTcpSocket_SetIsoError(errIsoRecvPacket);
				else
					*Size =DataLength;
			}
			else
				return TIsoTcpSocket_SetIsoError(errIsoPduOverflow);
		}
	}
	else
		return TIsoTcpSocket_SetIsoError(errIsoRecvPacket);

	return LastIsoError;
}
//---------------------------------------------------------------------------
// Fragments Recv schema
//------------------------------------------------------------------------------
//
//         packet 1                 packet 2                 packet 3
// +--------+------------+  +--------+------------+  +--------+------------+
// | HEADER | FRAGMENT 1 |  | HEADER | FRAGMENT 2 |  | HEADER | FRAGMENT 3 |
// +--------+------------+  +--------+------------+  +--------+------------+
//                |                         |                        |
//                |             +-----------+                        |
//                |             |                                    |
//                |             |           +------------------------+
//                |             |           |      (Packet 3 has EoT Flag set)
//                V             V           V
// +--------+------------+------------+------------+
// | HEADER | FRAGMENT 1 : FRAGMENT 2 : FRAGMENT 3 |
// +--------+------------+------------+------------+
//     ^
//     |
//     +-- A new header is built with updated info
//
//------------------------------------------------------------------------------
int TIsoTcpSocket_isoRecvPDU(PIsoDataPDU Data)
{
	int Result;
	int Size;
	pbyte pData;
	int max;
	int Offset;
	int Received;
	int NumParts;
	bool Complete;

	NumParts =1;
	Offset =0;
	Complete =false;
		TIsoTcpSocket_ClrIsoError();
	pData = (pbyte)(&PDU.Payload);
	do {
		pData=pData+Offset;
		max =IsoPayload_Size-Offset; // Maximum packet allowed
		if (max>0)
		{
			Result =TIsoTcpSocket_isoRecvFragment(pData, max, &Received, &Complete);
			if((Result==0) &&  !Complete)
			{
				++NumParts;
				Offset += Received;
				if (NumParts>IsoMaxFragments)
					Result =TIsoTcpSocket_SetIsoError(errIsoTooManyFragments);
			}
		}
		else
			Result =TIsoTcpSocket_SetIsoError(errIsoTooManyFragments);
	} while ((!Complete) && (Result==0));


	if (Result==0)
	{
		// Add to offset the header size
		Size =Offset+Received+DataHeaderSize;
		// Adjust header
		PDU.TPKT.HI_Lenght =((u_short)(Size)>>8) & 0xFF;
		PDU.TPKT.LO_Lenght =(u_short)(Size) & 0xFF;
		// Copies data if target is not the local PDU
		if (Data!=&PDU)
						memcpy(Data, &PDU, Size);
	}
	else
				if (LastTcpError!=WSAECONNRESET)
						TMsgSocket_Purge();
	return Result;
}
//---------------------------------------------------------------------------
//int TIsoTcpSocket_isoExchangePDU(PIsoDataPDU Data)
//{
//    int Result;
//    TIsoTcpSocket_ClrIsoError();
//	Result=isoSendPDU(Data);
//	if (Result==0)
//		Result=TIsoTcpSocket_isoRecvPDU(Data);
//	return Result;
//}
//---------------------------------------------------------------------------
//void TIsoTcpSocket_IsoPeek(void *pPDU, struct TPDUKind &PduKind)
//{
//	PIsoHeaderInfo Info;
//	u_int IsoLen;

//    Info=PIsoHeaderInfo(pPDU);
//    IsoLen=TIsoTcpSocket_PDUSize(Info);

//    // Check for empty fragment : size of PDU = size of header and nothing else
//    if (IsoLen==DataHeaderSize )
//    {
//        // We don't need to check the EoT flag since the PDU is empty....
//        PduKind=pkEmptyFragment;
//        return;
//    };
//    // Check for invalid packet : size of PDU < size of header
//    if (IsoLen<DataHeaderSize )
//    {
//        PduKind=pkInvalidPDU;
//        return;
//    };
//    // Here IsoLen>DataHeaderSize : check the PDUType
//    switch (Info->PDUType)
//    {
//        case pdu_type_CR:
//            PduKind=pkConnectionRequest;
//            break;
//        case pdu_type_DR:
//            PduKind=pkDisconnectRequest;
//            break;
//        case pdu_type_DT:
//            PduKind=pkValidData;
//            break;
//        default:
//            PduKind=pkUnrecognizedType;
//    };
//}
