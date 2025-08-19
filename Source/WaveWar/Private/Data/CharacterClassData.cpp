// Uroš Pörš All Rights Reserved


#include "Data/CharacterClassData.h"





FCharacterClassInfo UCharacterClassData::GetCharacterClassInfo(ECharacterClass CharacterClass)
{
	/** This function will return information of enum */
	return CharacterClassMap.FindChecked(CharacterClass);
}
