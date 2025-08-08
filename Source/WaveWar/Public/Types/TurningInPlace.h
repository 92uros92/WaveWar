#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Right UMETA(DisplayName = "Turn Right"),
	ETIP_Left UMETA(DisplayName = "Turn Left"),
	ETIP_NotTurning UMETA(DisplayName = "NO Turn"),

	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};