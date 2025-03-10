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

struct ClientRecord {
	std::string name;
};

class Server {
public:
	void init(uint16 port); // Initialize and start the server
	void tick(); // one iteration of the server loop
	void pause(); // Pause server execution but keep it online
	void unpause(); // Unpause server execution if paused
	void close(); // Terminate the server
	void broadcast(const char* str); // Send a message to all connected clients
	Server();
	~Server();
private:
	bool running = false;

	std::map<HSteamNetConnection, ClientRecord> mapClients;
	HSteamListenSocket listenSocket;
	HSteamNetPollGroup pollGroup;
	ISteamNetworkingSockets* networkInterface;

	static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo); // The static method that is actually called on callback
	static void applyServerInstance(Server* server); // Sets the server instance
	static void debugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char *msg);

	void sendMessage(HSteamNetConnection conn, const char* str); // Send a message to a client
	void pollMessages(); // Poll incoming messages and process them
	void pollConnectionStateChanges(); // Check for changes in connections
	void pollServerStateChanges(); // poll for server state changes (such as a command to shutdown)
	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo); // A callback for when a client's connection status changes
};
