// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.

int32 ICombatInterface::GetPlayerLevel()
{
	/** Return 0 because function doesn´t need to be implemented heer in Interface class. */
	return 0;
}

/** Override in CharacterBase */
FVector ICombatInterface::GetSocketLocation()
{
	return FVector();
}

/** Override in ShadowCharacter */
FVector ICombatInterface::GetCameraLocation()
{
	return FVector();
}
