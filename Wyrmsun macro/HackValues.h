#pragma once
#include <Windows.h>
#include <vector>

namespace HackValues
{
	DWORD moneyBase = 0x0092E560;
	std::vector<unsigned int> moneyOffsets = { 0x84, 0x04, 0x14 };

	DWORD foodBase = 0x0092E560;
	std::vector<unsigned int> foodOffsets = { 0x194 };
	std::vector<unsigned int> maxFoodOffsets = { 0x190 };
	
	DWORD copperBase = 0x0092E560;
	std::vector<unsigned int> copperOffsets = { 0x84, 0x0, 0x14 };

	DWORD stoneBase = 0x0092E560;
	std::vector<unsigned int> stoneOffsets = { 0x84, 0x8, 0x14 };

	DWORD createUnitHookLocationOffset = 0x30979B;
	DWORD createUnitFunctionLocationOffset = 0x2077;

	DWORD gameLoopHookLocationOffset = 0x4E84DB;

}