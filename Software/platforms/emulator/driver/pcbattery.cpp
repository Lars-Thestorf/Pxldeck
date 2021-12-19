#include <HLM_battery.h>
#include <SDL2/SDL.h>

bool hasBattery() {
	return false;
}
bool isBatteryCharging()
{
	SDL_PowerState state = SDL_GetPowerInfo(NULL, NULL);
	if (state == SDL_POWERSTATE_CHARGING || state == SDL_POWERSTATE_CHARGED)
		return true;
	return false;
}
uint8_t getBatteryPercentage()
{
	int per;
	SDL_PowerState state = SDL_GetPowerInfo(NULL, &per);
	if (state == SDL_POWERSTATE_NO_BATTERY || state == SDL_POWERSTATE_UNKNOWN)
		return 50;
	return per;
}
uint16_t getBatteryMillivolts()
{
	return 3700;
}

