// --------------------------------------------------
// 
// Morroscape
// Created 8/26/2024
// Jack Bauer
// Aided by: https://github.com/ValveSoftware/GameNetworkingSockets/blob/master/examples/example_chat.cpp#L690
// 
// --------------------------------------------------
#include <raylib.h>
#include <iostream>
#include <assert.h>
#include <string>

#include "Client.h"

static Client* callbackInstance;

Client::Client() {

}

Client::~Client() {
	close();
}

void Client::init(const SteamNetworkingIPAddr &serverAddr) {
	callbackInstance = this;

	SteamDatagramErrMsg errMsg;
	if (!GameNetworkingSockets_Init(nullptr, errMsg))
		std::cerr << "GameNetworkingSockets_Init failed: " << errMsg << std::endl;
	
	networkInterface = SteamNetworkingSockets();
	if (networkInterface == nullptr) {
		std::cerr << "Error: SteamNetworkingSockets returned nullptr." << std::endl;
		return;
	}

	// Find address
	char address[SteamNetworkingIPAddr::k_cchMaxString];
	serverAddr.ToString(address, sizeof(address), true);
	std::cout << "Connecting to server at: " << address << std::endl;

	// options
	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);
	
	// Connect
	connection = networkInterface->ConnectByIPAddress(serverAddr, 1, &opt);
	if (connection == k_HSteamNetConnection_Invalid) {
		std::cerr << "Failed to create connection to server" << std::endl;
	}

	std::cout << "Client connected to server" << std::endl;
	running = true;
}

void Client::tick() {
	if (running) {
		pollMessages();
		pollConnectionStateChanges();
		// also input
	}
}

void Client::pollMessages() {
	ISteamNetworkingMessage* incomingMsg = nullptr;
	int numMsgs = networkInterface->ReceiveMessagesOnConnection(connection, &incomingMsg, 1);
	if (numMsgs == 0) {
		return;
	}
	if (numMsgs < 0) {
		std::cerr << "Error checking messages" << std::endl;
	}

	if (incomingMsg != nullptr) {
		DrawText((const char*)incomingMsg->m_pData, 100, 100, 12, RED);
		incomingMsg->Release();
	}
}

void Client::sendMessage(const char* msg) {
	networkInterface->SendMessageToConnection(connection, msg, sizeof(msg), k_nSteamNetworkingSend_Reliable, nullptr);
}

void Client::close() {
	if (running) {
		callbackInstance = nullptr;
		running = false;
		GameNetworkingSockets_Kill();
	}
}

void Client::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo) {
	callbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

void Client::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) {
	assert(pInfo->m_hConn == connection || connection == k_HSteamNetConnection_Invalid);

	switch (pInfo->m_info.m_eState) {

		// Called when connections are destroyed, but handled elsewhere.
		case k_ESteamNetworkingConnectionState_None:
			break;

		case k_ESteamNetworkingConnectionState_ClosedByPeer:
		case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		{
			std::string errorMsg;
			// quit
			if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting) {
				errorMsg = "Error connecting: ", pInfo->m_info.m_szEndDebug;
			} 
			else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally) {
				errorMsg = "Lost contact: ", pInfo->m_info.m_szEndDebug;
			}
			else {
				errorMsg = "Disconnected.", pInfo->m_info.m_szEndDebug;
			}

			std::cout << errorMsg << std::endl;

			networkInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
			connection = k_HSteamNetConnection_Invalid;
			break;
		}

		// start connecting
		case k_ESteamNetworkingConnectionState_Connecting:
			break;

		case k_ESteamNetworkingConnectionState_Connected:
			std::cout << "Connected OK" << std::endl;
			break;

		default:
			break;
	}
}

void Client::pollConnectionStateChanges() {
	callbackInstance = this;
	networkInterface->RunCallbacks();
}
