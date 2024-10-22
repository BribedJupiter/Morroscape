// --------------------------------------------------
// 
// Morroscape
// Created 7/20/2024
// Jack Bauer
// Aided by: https://github.com/ValveSoftware/GameNetworkingSockets/blob/master/examples/example_chat.cpp#L690
// 
// --------------------------------------------------
#include "Server.h"

#include <raylib.h>
#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <thread>
#include <chrono>

static Server* serverInstance; // must be in the implementation file
static SteamNetworkingMicroseconds logTimeZero;

Server::Server() {
	
}

void Server::debugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char* msg) {
	SteamNetworkingMicroseconds time = SteamNetworkingUtils()->GetLocalTimestamp() - logTimeZero;
	std::cout << "[Server] Debug time: " << time << std::endl;
	
	if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug) {
		std::cout << "[Server] Error" << std::endl;
	}
}

void Server::init(uint16 port) {
	applyServerInstance(this);

	SteamDatagramErrMsg errMsg;
	if (!GameNetworkingSockets_Init(nullptr, errMsg))
		std::cerr << "[Server] GameNetworkingSockets_Init failed: " << errMsg << std::endl;

	// init debug
	logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
	SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, debugOutput);

	// Use the default interface
	networkInterface = SteamNetworkingSockets();
	if (networkInterface == nullptr) {
		std::cerr << "[Server] Error: SteamNetworkingSockets returned nullptr." << std::endl;
		return;
	}

	// Set up server address and port
	SteamNetworkingIPAddr serverLocalAddr;
	serverLocalAddr.Clear();
	serverLocalAddr.m_port = port;

	// Set up config
	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)Server::SteamNetConnectionStatusChangedCallback);

	// Listen on socket
	listenSocket = networkInterface->CreateListenSocketIP(serverLocalAddr, 1, &opt);
	if (listenSocket == k_HSteamListenSocket_Invalid)
		throw std::invalid_argument("[Server] Invalid Socket");
	pollGroup = networkInterface->CreatePollGroup();
	if (pollGroup == k_HSteamNetPollGroup_Invalid)
		throw std::invalid_argument("[Server] Invalid Poll Group");
	
	running = true;
	std::cout << "[Server] Server listening on port: " << port << std::endl;
}

void Server::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo) {
	serverInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

void Server::applyServerInstance(Server* server) {
	serverInstance = server;
}

void Server::tick() {
	// Poll messages
	pollMessages();
	// Poll connection state changes
	pollConnectionStateChanges();
	// Poll local input
	pollServerStateChanges();

	broadcast("Hello There");
}

void Server::pause() {

}

void Server::unpause() {

}

void Server::close() {
	if (running) {
		// Close server
		for (auto it : mapClients) {
			// Send a goodbye message
			// Close connection
			networkInterface->CloseConnection(it.first, 0, "[Server] Sever Shutdown", true);
		}
		mapClients.clear();

		networkInterface->CloseListenSocket(listenSocket);
		listenSocket = k_HSteamListenSocket_Invalid;

		networkInterface->DestroyPollGroup(pollGroup);
		pollGroup = k_HSteamNetPollGroup_Invalid;

		serverInstance = nullptr;
		running = false;
		GameNetworkingSockets_Kill();
	}
}

void Server::sendMessage(HSteamNetConnection conn, const char *str) {
	networkInterface->SendMessageToConnection(conn, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr);
}

void Server::broadcast(const char *str) {
	for (auto& client : mapClients) {
		if (client.first != k_HSteamNetConnection_Invalid) {
			sendMessage(client.first, str);
		}
	}
}

void Server::pollMessages() {

	while (running) {
		// Get a message
		ISteamNetworkingMessage* pIncomingMsg = nullptr;
		int numMsgs = networkInterface->ReceiveMessagesOnPollGroup(pollGroup, &pIncomingMsg, 1);
		if (numMsgs == 0)
			break;
		if (numMsgs < 0) {
			throw std::runtime_error("[Server] Error on message check");
		}
		assert(numMsgs == 1 && pIncomingMsg);
		auto itClient = mapClients.find(pIncomingMsg->m_conn);
		assert(itClient != mapClients.end());

		// '\0' terminate to make it easier to parse
		std::string sCmd;
		sCmd.assign((const char*)pIncomingMsg->m_pData, pIncomingMsg->m_cbSize);
		const char* cmd = sCmd.c_str();

		std::cout << "[Server] Received message: " << sCmd << std::endl;
		pIncomingMsg->Release();

		// Actual processing logic
		DrawText(cmd, 200, 200, 32, PURPLE);
		continue;
	}
}

void Server::pollConnectionStateChanges() {
	serverInstance = this; // in case we want to have multiple server nodes running
	networkInterface->RunCallbacks();
}

void Server::pollServerStateChanges() {
	if (IsKeyPressed(KEY_APOSTROPHE))
		running = false;
}

void Server::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) {
	switch (pInfo->m_info.m_eState) {
		case k_ESteamNetworkingConnectionState_None:
			// Often called when connections are destroyed
			break;
		
		case k_ESteamNetworkingConnectionState_ClosedByPeer:
		case k_ESteamNetworkingConnectionState_ProblemDetectedLocally: {
			// Ignore if client not previously connected / disconnected before accepted by server
			if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected) {
				// Find where we keep track of the client, which is likely
				auto itClient = mapClients.find(pInfo->m_hConn);
				assert(itClient != mapClients.end());

				// Find appropriate log messages
				if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally) {
					std::cout << "[Server] Problem detected locally with " << itClient->second.name << std::endl;
				}
				else {
					std::cout << "[Server] Connection closed by peer " << itClient->second.name << std::endl;
				}

				std::cout << "[Server[ Connection "
					<< pInfo->m_info.m_szConnectionDescription
					<< "has left. "
					<< pInfo->m_info.m_eEndReason
					<< pInfo->m_info.m_szEndDebug
					<< std::endl;

				mapClients.erase(itClient);
				broadcast("A connection has left");
			}
			else {
				assert(pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
			}

			// Close connection on server side
			networkInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
			break;
		}

		case k_ESteamNetworkingConnectionState_Connecting:
		{
			// Ensure that this is a new connection
			assert(mapClients.find(pInfo->m_hConn) == mapClients.end());
			std::cout << "[Server] Connection request from " << pInfo->m_info.m_szConnectionDescription << std::endl;

			// Attempt to accept the connection
			if (networkInterface->AcceptConnection(pInfo->m_hConn) != k_EResultOK) {
				// If it fails (such as the client tried to connect but then disconnected
				// we need to close the connection
				networkInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
				std::cout << "[Server] Cannot accept connection" << std::endl;
				break;
			}

			// Assign the poll group
			if (!networkInterface->SetConnectionPollGroup(pInfo->m_hConn, pollGroup)) {
				networkInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
				std::cout << "[Server] Failed to set poll group" << std::endl;
				break;
			}

			// Processing logic
			char temp[1024];
			char nick[64];
			sprintf(nick, "NewGuy%d", 10000 + (rand() % 100000));
			std::cout << "[Server] Accepted Client: " << nick << std::endl;
			sprintf(temp, "Welcome to Morroscape %s", nick);
			sendMessage(pInfo->m_hConn, temp);

			if (mapClients.empty()) {
				sendMessage(pInfo->m_hConn, "You are alone");
			}
			else {
				for (auto& c : mapClients) {
					sendMessage(pInfo->m_hConn, c.second.name.c_str());
				}
			}

			sprintf(temp, "A new client has joined known as %s", nick);
			broadcast(temp);

			// Add to client list
			mapClients.insert({ pInfo->m_hConn, ClientRecord{nick} });
			break;
		}

		case k_ESteamNetworkingConnectionState_Connected:
			// Called when a new client finishes connecting
			break;

		default:
			break;
	}
}

Server::~Server() {
	close();
}