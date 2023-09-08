

using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class AuraProjectTarget : TargetRules
{
	public AuraProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		CppStandard = CppStandardVersion.Latest;
		CppStandardEngine = CppStandardVersion.Latest;
		CStandard = CStandardVersion.Latest;
		GeneratedCodeVersion = EGeneratedCodeVersion.VLatest;

		ExtraModuleNames.AddRange( new string[] { "AuraProject" } );
	}
}
