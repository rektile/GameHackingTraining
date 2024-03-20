#pragma once
#include <Windows.h>
#include <vector>

namespace offsets
{
	DWORD moneyBase = 0x01150DB0;
	std::vector<unsigned int> moneyOffsets = { 0x04, 0x04 };
	std::vector<unsigned int> incomeOffsets = { 0x04 , 0x68 };


	unsigned char* mapHackLocation = (unsigned char*)0x006CD519;
	std::vector<unsigned char> mapHackBytes = { 0x90, 0x90, 0x90, 0x83, 0x0E, 0xFF, 0x90, 0x90 }; // doesnt work

	unsigned char* noFogHackLocation = (unsigned char*)0x006D6C04;
	std::vector<unsigned char> noFogHackBytes = { 0xEB }; // je -> jmp
}
