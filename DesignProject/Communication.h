#pragma once

#define TEST_COMMAND "TEST"
#define UDPPORTNUMBER 4463
#define UDPROBOTRECIEVEPORTNUMBER 4462

#include <string>

class CDesignProjectDoc; // Project doc forward ref

class CCommunication
{
public:
	CCommunication();
	~CCommunication();

	// Initialize socket data and connection
	void InitializeSockData();

	// Set port
	void SetPort(int port) { this->mPort = port; }

	// Get Port
	int GetPort() const { return this->mPort; }

	// Connected?
	bool Connected() const { return this->mConnected; }

	// Send command
	bool SendCommand(std::string command, bool exit=false);

	// Total commands sent
	int CommandsSent() const { return mCommandsSent; }

	// Disconnect/exit the server
	void Disconnect();

	// Clear
	void Clear();

private:
	/// UDP send socket
	SOCKET mUdpSendSock;

	/// UDP sockaddr in
	sockaddr_in mUdp_remote;

	/// Port number
	int mPort;

	/// Connected?
	bool mConnected;

private:
	/// Commands sent
	int mCommandsSent;
};



/* Command message type. */
typedef struct Command_MSG_type
{
	char command[1000];
	bool exit;
}Command_MSG;

