// Super Copyright


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.

int32 ICombatInterface::GetCombatLevel()
{
	return 0;
}

FVector ICombatInterface::GetCombatWeaponTipSocketLocation()
{
	return FVector();
}

void ICombatInterface::Die()
{
}
