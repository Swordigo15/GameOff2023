// Copyright Anrility. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SupplyApocalypseTarget : TargetRules
{
	public SupplyApocalypseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "SupplyApocalypse" } );
	}
}
