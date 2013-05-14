#include "RakPeerInterface.h"
#include "RakSleep.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "Kbhit.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "NatPunchthroughClient.h"
#include "NatTypeDetectionClient.h"
#include "Getche.h"
#include "GetTime.h"
#include "Router2.h"
#include "UDPProxyClient.h"
#include "Gets.h"
#include "Itoa.h"

#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 500

// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

// tdogl classes
#include "Game.hpp"

bool OpenUPNP(RakNet::RakPeerInterface *rakPeer)
{	
	struct UPNPDev * devlist = 0;
	devlist = upnpDiscover(2000, 0, 0, 0, 0, 0);
	if (devlist)
	{
		char lanaddr[64];	/* my ip address on the LAN */
		struct UPNPUrls urls;
		struct IGDdatas data;
		if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==1)
		{
			DataStructures::List<RakNet::RakNetSocket2* > sockets;
			rakPeer->GetSockets(sockets);
			char iport[32];
			Itoa(sockets[0]->GetBoundAddress().GetPort(),iport,10);
			char eport[32];
			strcpy(eport, iport);


			// Version 1.5
// 				int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
// 					 eport, iport, lanaddr, 0, "UDP", 0);

			// Version miniupnpc-1.6.20120410
			int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
					 					eport, iport, lanaddr, 0, "UDP", 0, "0");

			if(r!=UPNPCOMMAND_SUCCESS)
				printf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
				eport, iport, lanaddr, r, strupnperror(r));

			char intPort[6];char intClient[16];

			// Version 1.5
// 				r = UPNP_GetSpecificPortMappingEntry(urls.controlURL,
// 					data.first.servicetype,
// 					eport, "UDP",
// 					intClient, intPort);

			// Version miniupnpc-1.6.20120410
			char desc[128];
			char enabled[128];
			char leaseDuration[128];
			r = UPNP_GetSpecificPortMappingEntry(urls.controlURL,
				data.first.servicetype,
				eport, "UDP",
				intClient, intPort,
				desc, enabled, leaseDuration);

			if(r!=UPNPCOMMAND_SUCCESS)
			{
				printf("GetSpecificPortMappingEntry() failed with code %d (%s)\n",
				r, strupnperror(r));
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


int main(int argc, char *argv[]) {

	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	printf("(C) or (S)erver?\n");
	gets(str);
	if ((str[0]=='c')||(str[0]=='C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1,&sd, 1);
		isServer = false;
	} else {
		RakNet::SocketDescriptor sd(SERVER_PORT,0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	} else {
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0]==0){
			strcpy(str, "natpunch.jenkinssoftware.com");
		}
		printf("Starting the client.\n");
		peer->Connect(str, SERVER_PORT, 0,0);
	}

	bool success = OpenUPNP(peer);

	while (1)
	{
		for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
		{
			switch (packet->data[0])
				{
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n");
					break;
				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n");
					break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n");
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					printf("Our connection request has been accepted.\n");
					break;					
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					if (isServer){
						printf("A client has disconnected.\n");
					} else {
						printf("We have been disconnected.\n");
					}
					break;
				case ID_CONNECTION_LOST:
					if (isServer){
						printf("A client lost the connection.\n");
					} else {
						printf("Connection lost.\n");
					}
					break;
				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
				}
		}
	}

	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);


    try {
        Game app;
		app.Initialize();
		app.Run();
		app.Destroy();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
