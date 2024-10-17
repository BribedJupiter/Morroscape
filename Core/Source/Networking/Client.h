// --------------------------------------------------
// 
// Morroscape
// Created 8/26/2024
// Jack Bauer
// Aided by: https://github.com/ValveSoftware/GameNetworkingSockets/blob/master/examples/example_chat.cpp#L690
// 
// --------------------------------------------------
#pragma once
#include <map>
#include <string>

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

class Client {
public:
	void init(const SteamNetworkingIPAddr& serverAddr);
	void tick();
	void close();
	Client();
	~Client();

	bool running = false;

private:
	HSteamNetConnection connection;
	ISteamNetworkingSockets* networkInterface;

	void sendMessage(const char* msg);
	void pollMessages();
	void pollConnectionStateChanges();
	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);
	static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
};
