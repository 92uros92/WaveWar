// Uroš Pörš All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class WaveWarTarget : TargetRules
{
	public WaveWarTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "WaveWar" } );
	}
}
