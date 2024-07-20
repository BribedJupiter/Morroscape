// --------------------------------------------------
// 
// Morroscape
// Created 7/20/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

#include <map>
#include <string>

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

struct Client {
	std::string name;
};

class Server {
public:
	void open(); // Initialize and start the server
	void pause(); // Pause server execution but keep it online
	void unpause(); // Unpause server execution if paused
	void close(); // Terminate the server

private:
	std::map<HSteamNetConnection, Client> mapClients;
	HSteamListenSocket listenSocket;
	HSteamNetPollGroup pollGroup;
	ISteamNetworkingSockets* networkInterface;

	void send(); // Send a message to a client
	void broadcast(); // Send a message to all connected clients
	void recv(); // Poll incoming messages and process them
	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo); // A callback for when a client's connection status changes
};
