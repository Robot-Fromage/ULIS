// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
using System;
using System.IO;
using UnrealBuildTool;

public class ULIS : ModuleRules
{
    public ULIS( ReadOnlyTargetRules Target ) : base( Target )
    {
        Type = ModuleType.External;
        if (Target.Platform == UnrealTargetPlatform.Win64 ||
            Target.Platform == UnrealTargetPlatform.Win32 )
        {
            string IncludeBase                      = Path.Combine( ModuleDirectory, "redist", "include" );
            PublicSystemIncludePaths.AddRange(
                new string[] {
                      Path.Combine( IncludeBase, "ULIS3" )
                    , Path.Combine( IncludeBase, "static_math", "include" )
                    , Path.Combine( IncludeBase, "glm" )
                }
            );

            string LibBase          = Path.Combine( ModuleDirectory, "redist", "bin", "x64" );
            string LibBaseRelease   = Path.Combine( LibBase, "release" );
            string LibBaseDebug     = Path.Combine( LibBase, "debug" );
            string LibBaseFull = LibBaseRelease;

            string ULIS_LibName         = "ULIS3";

            if (Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT) {
                    ULIS_LibName        += "d";
                    LibBaseFull = LibBaseDebug;
            }

            PublicAdditionalLibraries.AddRange(
                new string[] {
                      Path.Combine( LibBaseFull, ULIS_LibName ) + ".lib"
                    //, Path.Combine( LibBaseFull, ULIS_LibName ) + ".dll"
                }
            );

            PublicDefinitions.Add("WITH_ULIS=1");
        }
        else if ( Target.Platform == UnrealTargetPlatform.Mac )
        {
            string IncludeBase                      = Path.Combine( ModuleDirectory, "redist", "include" );
            PublicSystemIncludePaths.AddRange(
                new string[] {
                      Path.Combine( IncludeBase, "ULIS3" )
                    , Path.Combine( IncludeBase, "static_math", "include" )
                    , Path.Combine( IncludeBase, "glm" )
                }
            );

            string LibBase          = Path.Combine( ModuleDirectory, "redist", "bin", "x64" );
            string LibBaseRelease   = Path.Combine( LibBase, "release" );
            string LibBaseDebug     = Path.Combine( LibBase, "debug" );
            string LibBaseFull = LibBaseRelease;

            string ULIS_LibName         = "ULIS3";

            if (Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT) {
                    ULIS_LibName        += "d";
                    LibBaseFull = LibBaseDebug;
            }

            PublicAdditionalLibraries.AddRange(
                new string[] {
                      Path.Combine( LibBaseFull, ULIS_LibName ) + ".a"
                    //, Path.Combine( LibBaseFull, ULIS_LibName ) + ".dylib"
                }
            );

            PublicDefinitions.Add("WITH_ULIS=1");
        }
        else // unsupported platform
        {
            PublicDefinitions.Add("WITH_ULIS=0");
        }
    }
}

