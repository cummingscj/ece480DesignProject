#include "stdafx.h"
#include "Communication.h"
#include "DesignProjectDoc.h"

using namespace std;

/**
 * Constructor.
 */
CCommunication::CCommunication()
{
	mPort = UDPPORTNUMBER;		// Default UDP port number
	InitializeSockData();		// Init UDP sock data
	mCommandsSent = 0;			// Init commands sent
}


/**
 * Destructor.
 */
CCommunication::~CCommunication()
{
	closesocket(mUdpSendSock);
}


/**
 * Initialize UDP sock data.
 * This is done on startup and also by the connect menu option.
 * Sets the UDP connection to broadcast commands.
 */
void CCommunication::InitializeSockData()
{
	mConnected = false;

	mUdpSendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int udp_len = sizeof(mUdp_remote);

	mUdp_remote.sin_family = AF_INET;
	mUdp_remote.sin_port = mPort;
	mUdp_remote.sin_addr.s_addr = INADDR_BROADCAST;

	int broadcast = 1;
	if ((setsockopt(mUdpSendSock, SOL_SOCKET, SO_BROADCAST,
		(char*)&broadcast, sizeof(broadcast))) == -1) {
		AfxMessageBox(L"Error setting sock opt udp.");
		exit(6);
	}

	mConnected = true;
}


/**
 * Send a command over the UDP connection by broadcast.
 */
bool CCommunication::SendCommand(string command, bool exit)
{
	if (mConnected)
	{
		Command_MSG* msg = new Command_MSG();
		char temp[sizeof(Command_MSG)];
		strcpy_s(msg->command, command.c_str());
		msg->exit = exit;
		memcpy(temp, msg, sizeof(msg));

		sendto(mUdpSendSock, temp, sizeof(temp), 0,
			(sockaddr*)&mUdp_remote, (socklen_t)sizeof(mUdp_remote));

		delete msg;
		mCommandsSent++;
		return true;
	}
	return false;
}


/**
 * Disconnect.
 * Exits the server.
 */
void CCommunication::Disconnect()
{
	SendCommand("exit", true);
	mConnected = false;
}


/**
 * Clear.
 */
void CCommunication::Clear()
{
	mCommandsSent = 0;
}