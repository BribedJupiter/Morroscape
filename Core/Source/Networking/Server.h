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
	void run(uint16 port); // Initialize and start the server
	void pause(); // Pause server execution but keep it online
	void unpause(); // Unpause server execution if paused
	void close(); // Terminate the server

private:
	bool running = false;

	std::map<HSteamNetConnection, Client> mapClients;
	HSteamListenSocket listenSocket;
	HSteamNetPollGroup pollGroup;
	ISteamNetworkingSockets* networkInterface;
	static Server* serverInstance;

	Server();
	~Server();

	static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo); // The static method that is actually called on callback
	static void applyServerInstance(Server* server); // Sets the server instance
	void sendMessage(HSteamNetConnection conn, const char* str); // Send a message to a client
	void broadcast(const char* str); // Send a message to all connected clients
	void pollMessages(); // Poll incoming messages and process them
	void pollConnectionStateChanges(); // Check for changes in connections
	void pollServerStateChanges(); // poll for server state changes (such as a command to shutdown)
	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo); // A callback for when a client's connection status changes
};
