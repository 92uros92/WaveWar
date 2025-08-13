// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Data/CharacterClassData.h"





FCharacterClassInfo UCharacterClassData::GetCharacterClassInfo(ECharacterClass CharacterClass)
{
	/** This function will return information of enum */
	return CharacterClassMap.FindChecked(CharacterClass);
}
