// Uroš Pörš All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class WaveWarEditorTarget : TargetRules
{
	public WaveWarEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "WaveWar" } );
	}
}
