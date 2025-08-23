// Uroš Pörš All Rights Reserved


#include "Interaction/CombatInterface.h"




// Add default functionality here for any ICombatInterface functions that are not pure virtual.

int32 ICombatInterface::GetPlayerLevel()
{
	/** Return 0 because function doesn´t need to be implemented heer in Interface class. */
	return 0;
}

/** Override in ShadowCharacter */
FVector ICombatInterface::GetCameraLocation()
{
	return FVector();
}
