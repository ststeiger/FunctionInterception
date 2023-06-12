# Function interception and detours that work on Windows, Linux, and MacOS. 

Hook into method calls and change what they do, re-call the original function, all at runtime. <br />
No modifyig of the binary, overwrites the code-segment directly in RAM. <br />
This way, the MD5-checksum of the file stays the same. 


There are the preprocessor-functions HOTPATCH, UNPATCH and NATURALIZE. 


Hotpatch is just a convenience function for TemplateFuncInterceptFunction which calls InterceptFunction. 
Hotpatch requires you have a backup function pointer with name original_##FUNCTION_NAME defined and a function modified_##FUNCTION_NAME, which is the function you are overwriting ##FUNCTION_NAME with. 

UNPATCH is just a convenience preprocessor-function-definition for unpatchfunc. 

NATURALIZE is a preprocessor defined function which assigns a function in the code segment to a function pointer based on the function's address. 

The target function RE_RenderScene, and the backup and naturalization pointers:
```  
extern "C" 
{

	

	//void __cdecl RE_RenderScene()
	void RE_RenderScene()
	{
		printf("This is the original RE_RenderScene\n");
	}


	// void(__cdecl  *original_RE_RenderScene)(); // = &RE_RenderScene;
	void(*original_RE_RenderScene)(); // = &RE_RenderScene;


	// void(__cdecl  *Naturalized_RE_RenderScene)(); // = &RE_RenderScene;
	void(*Naturalized_RE_RenderScene)(); // = &RE_RenderScene;
	

	//void __cdecl modified_RE_RenderScene()
	void __cdecl modified_RE_RenderScene()
	{
		printf("Entering the modified RenderScene\n");

		printf("Calling the original RenderScene in the modified RenderScene\n");
		printf("Address of RE_RenderScene: %p\n", &RE_RenderScene);
		printf("Address of original_RE_RenderScene: %p\n", *original_RE_RenderScene);
		printf("Address of original_RE_RenderScene: %p\n", original_RE_RenderScene);
		// (*original_RE_RenderScene)();
		
		printf("Finished calling the original RenderScene in the modified RenderScene\n");

		printf("Exiting modified RenderScene\n");
	}

	
}
```


Then you can call it in code: 
```
//#define RE_RenderScene_COPY 9 // Visual Studio 2015
#define RE_RenderScene_COPY 9 // Visual Studio 2013
// #define RE_RenderScene_COPY 6 // g++

// int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	printf("Sizeof address: %d bit\n", sizeof(void*) * 8);
	printf("Address of %s: 0x%08p\n", "RE_RenderScene", &RE_RenderScene);
	printf("Address of %s: 0x%08x\n", "RE_RenderScene", &RE_RenderScene);
	printf("===================================================\n");
	

	unsigned char* memoryDumpPointer = (unsigned char*)&RE_RenderScene;
	int i;
	for (i = 0; i < 100; ++i)
	{
		printf("0x%02X\n", memoryDumpPointer[i]);
	}


	if (true)
	{
		RE_RenderScene(); // Calling original version
		
		printf("\n\n====================== Naturalizing =============================\n");
		NATURALIZE(RE_RenderScene);
		// Expands to 
		// Naturalized_RE_RenderScene = FuncConvertAddress(Naturalized_RE_RenderScene, reinterpret_cast<unsigned long>  (&RE_RenderScene));
		printf("====================== Naturalized ==============================\n\n");
		
		(*Naturalized_RE_RenderScene)();


		printf("\n\n====================== Hotpatching =============================\n");
		// HOTPATCH(RE_RenderScene);
		/////// Expands to
		/////// InterceptFunction(&RE_RenderScene, RE_RenderScene_COPY, &modified_RE_RenderScene);
		/////// original_RE_RenderScene = FuncInterceptFunction(RE_RenderScene, modified_RE_RenderScene);

		// Overwriting the RE_RenderScene function (JMP REL32 = 0xE9)
		original_RE_RenderScene = TemplateFuncInterceptFunction(
			original_RE_RenderScene,
			reinterpret_cast<unsigned long> (&RE_RenderScene),
			reinterpret_cast<unsigned long> (&modified_RE_RenderScene),
			static_cast<unsigned long> (RE_RenderScene_COPY)
		);

		printf("====================== Hotpatched ==============================\n\n");
		
		RE_RenderScene(); // Calling the modified version
		// Second time works as well
		RE_RenderScene();


		printf("\n\n====================== Unpatching =============================\n");
		UNPATCH(RE_RenderScene); // Undoing modification
		printf("====================== Unpatched ==============================\n");
		// BUG shows here
		RE_RenderScene();
	}


	printf("\n\n\n--- Press any key to continue --- \n");
	WaitChar();
	return EXIT_SUCCESS;
}
```
