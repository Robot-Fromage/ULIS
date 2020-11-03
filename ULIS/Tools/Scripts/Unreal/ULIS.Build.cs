// Copyright 2019 Clement Berthaud, All Rights Reserved.
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
            // string PlatformDir                      = Target.Platform.ToString();
            string IncludeBase                      = Path.Combine( ModuleDirectory, "Redist", "Include" );
            string ULIS_IncludePath                 = Path.Combine( IncludeBase, "ULIS" );
            string LittleCMS_IncludePath            = Path.Combine( IncludeBase, "Little-CMS" );
            string glm_IncludePath                  = Path.Combine( IncludeBase, "glm" );
            string coal_IncludePath                 = Path.Combine( IncludeBase, "coal" );
            string boot_preprocessor_IncludePath    = Path.Combine( IncludeBase, "boost_preprocessor" );
            PublicSystemIncludePaths.AddRange(
                new string[] {
                      ULIS_IncludePath
                    , LittleCMS_IncludePath
                    , glm_IncludePath
                    , coal_IncludePath
                    , boot_preprocessor_IncludePath
                }
            );

            string LibBase          = Path.Combine( ModuleDirectory, "Redist", "Lib", "Win", "MSVC", "x64" );
            string LibBaseRelease   = Path.Combine( LibBase, "Release" );
            string LibBaseDebug     = Path.Combine( LibBase, "Debug" );
            PublicLibraryPaths.Add( LibBaseRelease );
            PublicLibraryPaths.Add( LibBaseDebug );

            string ULIS_LibName         = "ULIS1.0";
            string LittleCMS_LibName    = "lcms2";

            if (Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT) {
                    ULIS_LibName        += "d";
                    LittleCMS_LibName   += "d";
            }

            PublicAdditionalLibraries.AddRange(
                new string[] {
                      ULIS_LibName + ".lib"
                    , LittleCMS_LibName + ".lib"
                }
            );

            PublicDefinitions.Add("WITH_ULIS=1");
        }
        else if ( Target.Platform == UnrealTargetPlatform.Mac )
        {
            // string PlatformDir                      = Target.Platform.ToString();
            string IncludeBase                      = Path.Combine( ModuleDirectory, "Redist", "Include" );
            string ULIS_IncludePath                 = Path.Combine( IncludeBase, "ULIS" );
            string LittleCMS_IncludePath            = Path.Combine( IncludeBase, "Little-CMS" );
            string glm_IncludePath                  = Path.Combine( IncludeBase, "glm" );
            string coal_IncludePath                 = Path.Combine( IncludeBase, "coal" );
            string boot_preprocessor_IncludePath    = Path.Combine( IncludeBase, "boost_preprocessor" );
            PublicSystemIncludePaths.AddRange(
                new string[] {
                      ULIS_IncludePath
                    , LittleCMS_IncludePath
                    , glm_IncludePath
                    , coal_IncludePath
                    , boot_preprocessor_IncludePath
                }
            );

            string LibBase          = Path.Combine( ModuleDirectory, "Redist", "Lib", "Mac", "Xcode", "x64" );
            string LibBaseRelease   = Path.Combine( LibBase, "Release" );
            string LibBaseDebug     = Path.Combine( LibBase, "Debug" );
            PublicLibraryPaths.Add( LibBaseRelease );
            PublicLibraryPaths.Add( LibBaseDebug );

            string ULIS_LibName         = "libULIS1.0";
            string LittleCMS_LibName    = "liblcms2";

            if (Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT) {
                    ULIS_LibName        += "d";
                    LittleCMS_LibName   += "d";
            }

            PublicAdditionalLibraries.AddRange(
                new string[] {
                      ULIS_LibName + ".a"
                    , LittleCMS_LibName + ".a"
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

