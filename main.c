#include <stdio.h>
#include "s7_micro_client.h"
#include "snap_msgsock.h"


void reading_tags(byte *data);

int main()
{
	SocketsLayer_init();
	TSnapBase_init();
	TMsgSocket_init();
	TSnap7Peer_init();
	TIsoTcpSocket_init();
	TSnap7MicroClient_init();

	int ret = TSnap7MicroClient_ConnectTo("192.168.1.100",0,0);

	if (ret==0)
	{
		printf("Connected");
	}
	else
	{
		printf("Error: %d",ret);
		return 0;
	}

	byte data[800];
	memset(data,0,800);

	TSnap7MicroClient_ReadArea(S7AreaDB, 2, 0, sizeof(data), S7WLByte, &data);

	reading_tags(data);

	SocketsLayer_deinit();
	TSnap7MicroClient_deinit();
	TIsoTcpSocket_deinit();
	TSnap7Peer_deinit();
	TMsgSocket_deinit();

	return 0;
}

void reading_tags(byte *data)
{
	union {
		byte by[4];
		int i;
		word w;
		float f;
	} a;
	float f1;
	byte by1;
	int iRegim, iRabota, iPozhar, iAvar;
	float fP1, fP2, fP3, fP4, fP5, fP6, fP7;
	float fG1, fG2;
	float fT1, fT2, fT3, fT4, fT5, fT6, fT7;
	float fF1, fF2, fF3;
	float fValve1, fValve2;
	float fUstP;
	int iPump1, iPump2, iPump3;
	int iI1, iI2, iI3;
	int iSt1, iSt2, iSt3;
	int iStValve1, iStValve2;
	int iAvar1, iAvar2;

	
	a.by[0] = data[23]; a.by[1] = data[22]; a.by[2] = data[21]; a.by[3] = data[20];
	fP1 = a.f; printf("fP1 = %f" , fP1 );

	a.by[0] = data[39]; a.by[1] = data[38]; a.by[2] = data[37]; a.by[3] = data[36];
	fP2 = a.f; printf("fP2 = %f" , fP2 );
	a.by[0] = data[39]; a.by[1] = data[38]; a.by[2] = data[37]; a.by[3] = data[36];

	a.by[0] = data[55]; a.by[1] = data[54]; a.by[2] = data[53]; a.by[3] = data[52];
	fP3 = a.f; printf("fP3 = %f" , fP3 );

	a.by[0] = data[71]; a.by[1] = data[70]; a.by[2] = data[69]; a.by[3] = data[68];
	fP4 = a.f; printf("fP4 = %f" , fP4 );

	a.by[0] = data[87]; a.by[1] = data[86]; a.by[2] = data[85]; a.by[3] = data[84];
	fP5 = a.f; printf("fP5 = %f" , fP5 );

	a.by[0] = data[103]; a.by[1] = data[102]; a.by[2] = data[101]; a.by[3] = data[100];
	fP6 = a.f; printf("fP6 = %f" , fP6 );

	a.by[0] = data[119]; a.by[1] = data[118]; a.by[2] = data[117]; a.by[3] = data[116];
	fP7 = a.f; printf("fP7 = %f" , fP7 );

	a.by[0] = data[135]; a.by[1] = data[134]; a.by[2] = data[133]; a.by[3] = data[132];
	fG1 = a.f; printf("fG1 = %f" , fG1 );

	a.by[0] = data[151]; a.by[1] = data[150]; a.by[2] = data[149]; a.by[3] = data[148];
	fG2 = a.f; printf("fG2 = %f" , fG2 );

	a.by[0] = data[167]; a.by[1] = data[166]; a.by[2] = data[165]; a.by[3] = data[164];
	fT1 = a.f; printf("fT1 = %f" , fT1 );

	a.by[0] = data[183]; a.by[1] = data[182]; a.by[2] = data[181]; a.by[3] = data[180];
	fT2 = a.f; printf("fT2 = %f" , fT2 );

	a.by[0] = data[199]; a.by[1] = data[198]; a.by[2] = data[197]; a.by[3] = data[196];
	fT3 = a.f; printf("fT3 = %f" , fT3 );

	a.by[0] = data[215]; a.by[1] = data[214]; a.by[2] = data[213]; a.by[3] = data[212];
	fT4 = a.f; printf("fT4 = %f" , fT4 );

	a.by[0] = data[231]; a.by[1] = data[230]; a.by[2] = data[229]; a.by[3] = data[228];
	fT5 = a.f; printf("fT5 = %f" , fT5 );

	a.by[0] = data[247]; a.by[1] = data[246]; a.by[2] = data[245]; a.by[3] = data[244];
	fT6 = a.f; printf("fT6 = %f" , fT6 );

	a.by[0] = data[263]; a.by[1] = data[262]; a.by[2] = data[261]; a.by[3] = data[260];
	fT7 = a.f; printf("fT7 = %f" , fT7 );

	a.by[0] = data[329]; a.by[1] = data[328]; a.by[2] = data[327]; a.by[3] = data[326];
	fF1 = a.f; printf("fF1 = %f" , fF1 );

	a.by[0] = data[367]; a.by[1] = data[366]; a.by[2] = data[365]; a.by[3] = data[364];
	fF2 = a.f; printf("fF2 = %f" , fF2 );

	a.by[0] = data[405]; a.by[1] = data[404]; a.by[2] = data[403]; a.by[3] = data[402];
	fF3 = a.f; printf("fF3 = %f" , fF3 );

	iStValve1 = data[447]; printf("iStValve1 = %f" , iStValve1 );
	a.by[0] = data[441]; a.by[1] = data[440]; a.by[2] = data[439]; a.by[3] = data[438];
	fValve1 = a.f; printf("fValve1 = %f" , fValve1 );

	iStValve2 = data[461]; printf("iStValve2 = %f" , iStValve2 );
	a.by[0] = data[455]; a.by[1] = data[454]; a.by[2] = data[453]; a.by[3] = data[452];
	fValve2 = a.f; printf("fValve2 = %f" , fValve2 );

	iSt1 = data[322]; printf("iSt1 = %d" , iSt1 );
	iPump1 = data[337]; printf("iPump1 = %d" , iPump1 );
	a.by[0] = data[357]; a.by[1] = data[356]; a.by[2] = data[355]; a.by[3] = data[354];
	iI1 = a.i; printf("iI1 = %d" , iI1 );

	iSt2 = data[360]; printf("iSt2 = %d" , iSt2 );
	iPump2 = data[375]; printf("iPump2 = %d" , iPump2 );
	a.by[0] = data[395]; a.by[1] = data[394]; a.by[2] = data[393]; a.by[3] = data[392];
	iI2 = a.i; printf("iI2 = %d" , iI2 );

	iSt3 = data[398]; printf("iSt3 = %d" , iSt3 );
	iPump3 = data[413]; printf("iPump3 = %d" , iPump3 );
	a.by[0] = data[433]; a.by[1] = data[432]; a.by[2] = data[431]; a.by[3] = data[430];
	iI3 = a.i; printf("iI3 = %d" , iI3 );

	a.by[0] = data[509]; a.by[1] = data[508]; a.by[2] = data[507]; a.by[3] = data[506];
	fUstP = a.f; printf("fUstP = " , fUstP );

	iRegim = data[1]; printf("iRegim = %d" , iRegim );
	iPozhar = data[3]; printf("iPozhar = %d" , iPozhar );
	iRabota = data[5]; printf("iRabota = %d" , iRabota );
	iAvar = data[7]; printf("iAvar = %d" , iAvar );

	iAvar1 = data[560]; printf("iAvar1 = %d" , iAvar1 );
	iAvar2 = data[561]; printf("iAvar2 = %d" , iAvar2 );
}

