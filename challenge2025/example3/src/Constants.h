#pragma once

const int ScreenWidth = 1600;
const int ScreenHeight = 1200;

const float VampireHeight = 32.0f;
const float VampireWidth = 32.0f;
const float VampireSpeed = 100.0f;
const float PlayerHeight = 55.0f;
const float PlayerWidth = 55.0f;

const float WeaponHeight = 10.0f;
const float WeaponActiveTime = 0.25f;

enum class ePauseState
{
	PAUSED,
	UNPAUSED
};

enum class UpgradeType
{
	HEALTH,
	SPEED,
	ATTACK_SPEED,
	WHIP_LENGTH
};
