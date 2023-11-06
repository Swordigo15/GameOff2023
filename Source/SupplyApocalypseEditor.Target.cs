// Copyright Anrility. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SupplyApocalypseEditorTarget : TargetRules
{
	public SupplyApocalypseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SupplyApocalypse" } );
	}
}
