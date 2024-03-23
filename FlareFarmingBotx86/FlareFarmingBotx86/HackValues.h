#pragma once
#include <Windows.h>
#include <vector>

namespace HackValues
{
	DWORD locationBase = 0x001AB274;
	std::vector<unsigned int> locationXOffsets = { 0x240 };
	std::vector<unsigned int> locationYOffsets = { 0x244 };

	DWORD mouseBase = 0x001AB298;
	std::vector<unsigned int> mouseXOffsets = { 0x664 };
	std::vector<unsigned int> mouseYOffsets = { 0x668 };

	DWORD enemyMovementHookLocationOffset = 0x27E0F;


}