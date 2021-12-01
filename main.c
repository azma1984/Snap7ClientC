#include <stdio.h>
#include "s7_micro_client.h"
#include "snap_msgsock.h"

int main()
{
	TSnapBase_init();
	TMsgSocket_init();
	TSnap7Peer_init();
	TIsoTcpSocket_init();
	TSnap7MicroClient_init();

	int ret = TSnap7MicroClient_ConnectTo("10.85.12.153",0,0);

	if (ret==0)
	{
		printf("Connected");
	}
	else
	{
		printf("Error: %d",ret);
		return 0;
	}

	byte result[1000];
	memset(result,0,1000);

	TSnap7MicroClient_ReadArea(S7AreaDB, 2, 0, 800, S7WLByte, &result);

	TSnap7MicroClient_deinit();
	TIsoTcpSocket_deinit();
	TSnap7Peer_deinit();
	TMsgSocket_deinit();

	return 0;
}
