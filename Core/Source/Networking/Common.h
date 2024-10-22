// --------------------------------------------------
// 
// Morroscape
// Created 10/22/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

class Networkable {
public:
	bool running;
	bool connected;

	void initialize();
	Networkable();
	~Networkable();
private:
	bool initGameNetworkingSockets(); // step 1 of init
	bool initDebug(); // step 2
	bool linkNetworkInterface(); // step 3
	void applyInstance();
};