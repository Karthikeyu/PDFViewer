// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;

public class PDF_Viewer : ModuleRules
{
	public PDF_Viewer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
	


		if (UnrealTargetPlatform.Win64 == Target.Platform)
        {
            
            PrivateIncludePaths.Add("../Source/PDF_Viewer/pdfium/Windows/include");
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "pdfium", "Windows", "lib", "pdfium.dll.lib"));
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "pdfium", "Windows", "bin", "pdfium.dll"));
            PublicDelayLoadDLLs.Add("pdfium.dll");
        }
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "Slate",
				"SlateCore",
                "Projects",
				"HTTP",
                "RHI",
				"RenderCore",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
