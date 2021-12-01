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
#ifndef s7_micro_client_h
#define s7_micro_client_h
//---------------------------------------------------------------------------
#include "s7_peer.h"
#include <stdbool.h>
#include "snap_sysutils.h"
//---------------------------------------------------------------------------

extern const longword errCliMask;
extern const longword errCliBase;

extern const longword errCliInvalidParams;
extern const longword errCliJobPending;
extern const longword errCliTooManyItems;
extern const longword errCliInvalidWordLen;
extern const longword errCliPartialDataWritten;
extern const longword errCliSizeOverPDU;
extern const longword errCliInvalidPlcAnswer;
extern const longword errCliAddressOutOfRange;
extern const longword errCliInvalidTransportSize;
extern const longword errCliWriteDataSizeMismatch;
extern const longword errCliItemNotAvailable;
extern const longword errCliInvalidValue;
extern const longword errCliCannotStartPLC;
extern const longword errCliAlreadyRun;
extern const longword errCliCannotStopPLC;
extern const longword errCliCannotCopyRamToRom;
extern const longword errCliCannotCompress;
extern const longword errCliAlreadyStop;
extern const longword errCliFunNotAvailable;
extern const longword errCliUploadSequenceFailed;
extern const longword errCliInvalidDataSizeRecvd;
extern const longword errCliInvalidBlockType;
extern const longword errCliInvalidBlockNumber;
extern const longword errCliInvalidBlockSize;
extern const longword errCliDownloadSequenceFailed;
extern const longword errCliInsertRefused;
extern const longword errCliDeleteRefused;
extern const longword errCliNeedPassword;
extern const longword errCliInvalidPassword;
extern const longword errCliNoPasswordToSetOrClear;
extern const longword errCliJobTimeout;
extern const longword errCliPartialDataRead;
extern const longword errCliBufferTooSmall;
extern const longword errCliFunctionRefused;
extern const longword errCliDestroying;
extern const longword errCliInvalidParamNumber;
extern const longword errCliCannotChangeParam;

extern const time_t DeltaSecs; // Seconds between 1970/1/1 (C time base) and 1984/1/1 (Siemens base)

#pragma pack(1)

// Read/Write Multivars
typedef struct{
   int   Area;
   int   WordLen;
   int   Result;
   int   DBNumber;
   int   Start;
   int   Amount;
   void  *pdata;
} TS7DataItem, *PS7DataItem;

//typedef int TS7ResultItems[MaxVars];
//typedef TS7ResultItems *PS7ResultItems;

typedef struct {
   int OBCount;
   int FBCount;
   int FCCount;
   int SFBCount;
   int SFCCount;
   int DBCount;
   int SDBCount;
} TS7BlocksList, *PS7BlocksList;

typedef struct {
   int BlkType;
   int BlkNumber;
   int BlkLang;
   int BlkFlags;
   int MC7Size;  // The real size in bytes
   int LoadSize;
   int LocalData;
   int SBBLength;
   int CheckSum;
   int Version;
   // Chars info
   char CodeDate[11];
   char IntfDate[11];
   char Author[9];
   char Family[9];
   char Header[9];
} TS7BlockInfo, *PS7BlockInfo ;

//typedef word TS7BlocksOfType[0x2000];
//typedef TS7BlocksOfType *PS7BlocksOfType;

//typedef struct {
//   char Code[21]; // Order Code
//   byte V1;       // Version V1.V2.V3
//   byte V2;
//   byte V3;
//} TS7OrderCode, *PS7OrderCode;

typedef struct {
   char ModuleTypeName[33];
   char SerialNumber[25];
   char ASName[25];
   char Copyright[27];
   char ModuleName[25];
} TS7CpuInfo, *PS7CpuInfo;

typedef struct {
   int MaxPduLengt;
   int MaxConnections;
   int MaxMpiRate;
   int MaxBusRate;
} TS7CpInfo, *PS7CpInfo;

// See §33.1 of "System Software for S7-300/400 System and Standard Functions"
// and see SFC51 description too
//typedef struct {
//   word LENTHDR;
//   word N_DR;
//} SZL_HEADER, *PSZL_HEADER;

//typedef struct {
//   SZL_HEADER Header;
//   byte Data[0x4000-4];
//} TS7SZL, *PS7SZL;

// SZL List of available SZL IDs : same as SZL but List items are big-endian adjusted
//typedef struct {
//   SZL_HEADER Header;
//   word List[0x2000-2];
//} TS7SZLList, *PS7SZLList;

// See §33.19 of "System Software for S7-300/400 System and Standard Functions"
//typedef struct {
//   word  sch_schal;
//   word  sch_par;
//   word  sch_rel;
//   word  bart_sch;
//   word  anl_sch;
//} TS7Protection, *PS7Protection;

#define s7opNone               0
#define s7opReadArea           1
#define s7opWriteArea          2
#define s7opReadMultiVars      3
#define s7opWriteMultiVars     4
#define s7opDBGet              5
#define s7opUpload             6
#define s7opDownload           7
#define s7opDelete             8
#define s7opListBlocks         9
#define s7opAgBlockInfo       10
#define s7opListBlocksOfType  11
#define s7opReadSzlList       12
#define s7opReadSZL           13
#define s7opGetDateTime       14
#define s7opSetDateTime       15
#define s7opGetOrderCode      16
#define s7opGetCpuInfo        17
#define s7opGetCpInfo         18
#define s7opGetPlcStatus      19
#define s7opPlcHotStart       20
#define s7opPlcColdStart      21
#define s7opCopyRamToRom      22
#define s7opCompress          23
#define s7opPlcStop           24
#define s7opGetProtection     25
#define s7opSetPassword       26
#define s7opClearPassword     27
#define s7opDBFill            28

// Param Number (to use with setparam)

// Low level : change them to experiment new connections, their defaults normally work well
//const int pc_iso_SendTimeout   = 6;
//const int pc_iso_RecvTimeout   = 7;
//const int pc_iso_ConnTimeout   = 8;
//const int pc_iso_SrcRef        = 1;
//const int pc_iso_DstRef        = 2;
//const int pc_iso_SrcTSAP       = 3;
//const int pc_iso_DstTSAP       = 4;
//const int pc_iso_IsoPduSize    = 5;

#pragma pack()

// Internal struct for operations
// Commands are not executed directly in the function such as "DBRead(...",
// but this struct is filled and then PerformOperation() is called.
// This allow us to implement async function very easily.

struct TSnap7Job
{
		int Op;        // Operation Code
		int Result;    // Operation result
		bool Pending;  // A Job is pending
		longword Time; // Job Execution time
		// Read/Write
		int Area;      // Also used for Block type and Block of type
		int Number;    // Used for DB Number, Block number
		int Start;     // Offset start
		int WordLen;   // Word length
		// SZL
		int ID;        // SZL ID
		int Index;     // SZL Index
		// ptr info
		void * pData;  // User data pointer
		int Amount;    // Items amount/Size in input
		int *pAmount;  // Items amount/Size in output
		// Generic
		int IParam;   // Used for full upload and CopyRamToRom extended timeout
};

//		void TSnap7MicroClient_FillTime(word SiemensTime, char *PTime);
//		byte TSnap7MicroClient_BCDtoByte(byte B);
//		byte TSnap7MicroClient_WordToBCD(word Value);
		int TSnap7MicroClient_opReadArea();
		int TSnap7MicroClient_opWriteArea();
		int TSnap7MicroClient_opReadMultiVars();
		int TSnap7MicroClient_opWriteMultiVars();
		int TSnap7MicroClient_opListBlocks();
		int TSnap7MicroClient_opListBlocksOfType();
		int TSnap7MicroClient_opAgBlockInfo();
		int TSnap7MicroClient_opDBGet();
		int TSnap7MicroClient_opDBFill();
		int TSnap7MicroClient_opUpload();
		int TSnap7MicroClient_opDownload();
		int TSnap7MicroClient_opDelete();
		int TSnap7MicroClient_opReadSZL();
		int TSnap7MicroClient_opReadSZLList();
		int TSnap7MicroClient_opGetDateTime();
		int TSnap7MicroClient_opSetDateTime();
		int TSnap7MicroClient_opGetOrderCode();
		int TSnap7MicroClient_opGetCpuInfo();
		int TSnap7MicroClient_opGetCpInfo();
		int TSnap7MicroClient_opGetPlcStatus();
		int TSnap7MicroClient_opPlcStop();
		int TSnap7MicroClient_opPlcHotStart();
		int TSnap7MicroClient_opPlcColdStart();
		int TSnap7MicroClient_opCopyRamToRom();
		int TSnap7MicroClient_opCompress();
		int TSnap7MicroClient_opGetProtection();
		int TSnap7MicroClient_opSetPassword();
		int TSnap7MicroClient_opClearPassword();
		int TSnap7MicroClient_CpuError(int Error);
	//	longword TSnap7MicroClient_DWordAt(void * P);
		int TSnap7MicroClient_CheckBlock(int BlockType, int BlockNum,  void *pBlock,  int Size);
		int TSnap7MicroClient_SubBlockToBlock(int SBB);

		word ConnectionType;
		longword JobStart;
		struct TSnap7Job Job;
		int TSnap7MicroClient_DataSizeByte(int WordLength);
    int opSize; // last operation size
		int TSnap7MicroClient_PerformOperation();


	//  TS7Buffer opData;
		void TSnap7MicroClient_init();
		void TSnap7MicroClient_deinit();
		int TSnap7MicroClient_Reset(bool DoReconnect);
		void TSnap7MicroClient_SetConnectionParams(const char *RemAddress, word LocalTSAP, word RemoteTsap);
	//	void TSnap7MicroClient_SetConnectionType(word ConnType);
	int TSnap7MicroClient_ConnectTo(const char *RemAddress, int Rack, int Slot);
		int TSnap7MicroClient_Connect();
	int TSnap7MicroClient_Disconnect();
	int TSnap7MicroClient_GetParam(int ParamNumber, void *pValue);
	int TSnap7MicroClient_SetParam(int ParamNumber, void *pValue);
    // Fundamental Data I/O functions
		int TSnap7MicroClient_ReadArea(int Area, int DBNumber, int Start, int Amount, int WordLen, void * pUsrData);
		int TSnap7MicroClient_WriteArea(int Area, int DBNumber, int Start, int Amount, int WordLen, void * pUsrData);
		int TSnap7MicroClient_ReadMultiVars(PS7DataItem Item, int ItemsCount);
		int TSnap7MicroClient_WriteMultiVars(PS7DataItem Item, int ItemsCount);
    // Data I/O Helper functions
		int TSnap7MicroClient_DBRead(int DBNumber, int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_DBWrite(int DBNumber, int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_MBRead(int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_MBWrite(int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_EBRead(int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_EBWrite(int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_ABRead(int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_ABWrite(int Start, int Size, void * pUsrData);
		int TSnap7MicroClient_TMRead(int Start, int Amount, void * pUsrData);
		int TSnap7MicroClient_TMWrite(int Start, int Amount, void * pUsrData);
		int TSnap7MicroClient_CTRead(int Start, int Amount, void * pUsrData);
		int TSnap7MicroClient_CTWrite(int Start, int Amount, void * pUsrData);
    // Directory functions
		int TSnap7MicroClient_ListBlocks(PS7BlocksList pUsrData);
		int TSnap7MicroClient_GetAgBlockInfo(int BlockType, int BlockNum, PS7BlockInfo pUsrData);
		int TSnap7MicroClient_GetPgBlockInfo(void * pBlock, PS7BlockInfo pUsrData, int Size);
//		int TSnap7MicroClient_ListBlocksOfType(int BlockType, TS7BlocksOfType *pUsrData, int & ItemsCount);
    // Blocks functions
	//	int TSnap7MicroClient_Upload(int BlockType, int BlockNum, void * pUsrData, int & Size);
	//	int TSnap7MicroClient_FullUpload(int BlockType, int BlockNum, void * pUsrData, int & Size);
		int TSnap7MicroClient_Download(int BlockNum, void * pUsrData, int Size);
		int TSnap7MicroClient_Delete(int BlockType, int BlockNum);
	//	int TSnap7MicroClient_DBGet(int DBNumber, void * pUsrData, int & Size);
		int TSnap7MicroClient_DBFill(int DBNumber, int FillChar);
    // Date/Time functions
//		int TSnap7MicroClient_GetPlcDateTime(tm &DateTime);
//		int TSnap7MicroClient_SetPlcDateTime(tm * DateTime);
		int TSnap7MicroClient_SetPlcSystemDateTime();
    // System Info functions
	//	int TSnap7MicroClient_GetOrderCode(PS7OrderCode pUsrData);
		int TSnap7MicroClient_GetCpuInfo(PS7CpuInfo pUsrData);
		int TSnap7MicroClient_GetCpInfo(PS7CpInfo pUsrData);
//		int TSnap7MicroClient_ReadSZL(int ID, int Index, PS7SZL pUsrData, int &Size);
//		int TSnap7MicroClient_ReadSZLList(PS7SZLList pUsrData, int &ItemsCount);
    // Control functions
		int TSnap7MicroClient_PlcHotStart();
		int TSnap7MicroClient_PlcColdStart();
		int TSnap7MicroClient_PlcStop();
		int TSnap7MicroClient_CopyRamToRom(int Timeout);
		int TSnap7MicroClient_Compress(int Timeout);
	//	int TSnap7MicroClient_GetPlcStatus(int &Status);
    // Security functions
//		int TSnap7MicroClient_GetProtection(PS7Protection pUsrData);
		int TSnap7MicroClient_SetSessionPassword(char *Password);
		int TSnap7MicroClient_ClearSessionPassword();
    // Properties
//		bool TSnap7MicroClient_Busy(){ return Job.Pending; };
	//	int TSnap7MicroClient_Time(){ return int(Job.Time);}

//---------------------------------------------------------------------------
#endif // s7_micro_client_h
