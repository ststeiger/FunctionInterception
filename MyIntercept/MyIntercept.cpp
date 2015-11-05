
#include <stdio.h>
#include <stdlib.h>

#include "OpCode.h"
#include "OsSpecific.h"
#include "WaitChar.h"

// http://stackoverflow.com/questions/31792663/visual-studio-2013-error-ms8020-build-tools-v140-cannot-be-found



// offset[ENGINE][FUNCTION_NAME] ;
// detourlength[ENGINE][FUNCTION_NAME]

#define HOTPATCH(FUNCTION_NAME) \
	original_##FUNCTION_NAME = TemplateFuncInterceptFunction(\
	original_##FUNCTION_NAME, \
	reinterpret_cast<unsigned long> (&FUNCTION_NAME), \
	reinterpret_cast<unsigned long> (&modified_##FUNCTION_NAME), \
	static_cast<unsigned long> (FUNCTION_NAME##_COPY) \
	)

#define UNPATCH(FUNCTION_NAME) \
	unpatchfunc(reinterpret_cast<void*>(reinterpret_cast<unsigned long>(&FUNCTION_NAME)), reinterpret_cast<unsigned char*> (reinterpret_cast<unsigned long>(original_##FUNCTION_NAME)), static_cast<unsigned long> (FUNCTION_NAME##_COPY))



#define NATURALIZE(FUNCTION_NAME) \
	Naturalized_##FUNCTION_NAME = FuncConvertAddress(Naturalized_##FUNCTION_NAME, &FUNCTION_NAME)


template <class DataType>
DataType FuncConvertAddress(const DataType dt_FunctionPointer, unsigned long uslng_FunctionAddress)
{
	return reinterpret_cast<DataType> (uslng_FunctionAddress);
}




void* FuncGetPage(const unsigned long &uslngVirtualMemoryAddress)
{
	return reinterpret_cast<void*> (uslngVirtualMemoryAddress & uslngPageMask);
}


void* InterceptFunction(void* voidptr_AddressOfDetouredFunction, unsigned long uslng_CopyLength, void* voidptr_AddressOfDetourFunction)
{
	DATATYPE_ADDRESS Relocation;
	//printf("copy length: %ld\n", uslng_CopyLength);
	//printf("MIN_REQUIRED_FOR_DETOUR : %d\n", MIN_REQUIRED_FOR_DETOUR );
	void* voidptr_BackupForOriginalFunction = malloc(uslng_CopyLength + MIN_REQUIRED_FOR_DETOUR);
	//printf("Sizeof Backuppointer %ld\n", sizeof(voidptr_BackupForOriginalFunction));
	//printf("Sizeof AddrDetouredFunction %d\n", sizeof(voidptr_AddressOfDetouredFunction));

	// printf("Here 1\n");
	memcpy(voidptr_BackupForOriginalFunction, voidptr_AddressOfDetourFunction, uslng_CopyLength);
	// printf("Here 2\n");
	

	if (OPCODE_NOT_DEFINED)
	{
		printf("Error: OP-Code not defined\n.");
		exit(EXIT_FAILURE);
	}

	// printf("JMP_OPCODE 0x%X\n", JMP_OPCODE);
	// printf("OPCODE_LENGTH %d\n", OPCODE_LENGTH);
	// printf("MIN_REQUIRED_FOR_DETOUR %d\n", MIN_REQUIRED_FOR_DETOUR);


	memset(reinterpret_cast<void*> (reinterpret_cast<unsigned long> (voidptr_BackupForOriginalFunction)+uslng_CopyLength),
		JMP_OPCODE, OPCODE_LENGTH);
	// printf("Here 3\n");


	Relocation = static_cast<DATATYPE_ADDRESS> (reinterpret_cast<unsigned long> (voidptr_AddressOfDetouredFunction)
		-(reinterpret_cast<unsigned long> (voidptr_BackupForOriginalFunction)
		+MIN_REQUIRED_FOR_DETOUR));
	// printf("Here 4\n");

	memcpy(reinterpret_cast<void*> (reinterpret_cast<unsigned long> (voidptr_BackupForOriginalFunction)
		+uslng_CopyLength + OPCODE_LENGTH), &Relocation, ADDRESS_LENGTH);
	// printf("Here 5\n");


	int retUnprotect = unprotect(FuncGetPage(reinterpret_cast <unsigned long> (voidptr_AddressOfDetouredFunction)), uslngPageSize);
	printf("Patch unprotect: %d\n", retUnprotect);


	memset(voidptr_AddressOfDetouredFunction, JMP_OPCODE, OPCODE_LENGTH);

	Relocation = static_cast<DATATYPE_ADDRESS> (reinterpret_cast<unsigned long> (voidptr_AddressOfDetourFunction)
		-(reinterpret_cast<unsigned long> (voidptr_AddressOfDetouredFunction)
		+MIN_REQUIRED_FOR_DETOUR));

	memcpy(reinterpret_cast<void*> (reinterpret_cast<unsigned long> (voidptr_AddressOfDetouredFunction)
		+OPCODE_LENGTH), &Relocation, ADDRESS_LENGTH);
	int retReprotect = unprotect(FuncGetPage(reinterpret_cast <unsigned long> (voidptr_BackupForOriginalFunction)), uslngPageSize);
	printf("Patch reprotect: %d\n", retReprotect);


	return voidptr_BackupForOriginalFunction;
}




void unpatchfunc(void* patched_function, unsigned char* original_function, unsigned long uslng_DetourLength)
{
	//DWORD dw_OldProtect;
	//VirtualProtect(patched_function, uslng_DetourLength, PAGE_EXECUTE_READWRITE, &dw_OldProtect);
	int retUnprotect = unprotect(FuncGetPage(reinterpret_cast<unsigned long>(patched_function)), uslngPageSize);
	printf("Unpatch: unprotect: %d\n", retUnprotect);

	unsigned int intIndex;
	for (intIndex = 0; intIndex < uslng_DetourLength; ++intIndex)
		*((unsigned char*)patched_function + intIndex) = *(original_function + intIndex);

	//VirtualProtect(patched_function, uslng_DetourLength, dw_OldProtect, &dw_OldProtect);
	int retReprotect = unprotect(FuncGetPage(reinterpret_cast<unsigned long>(patched_function)), uslngPageSize);
	printf("Unpatch reprotect: %d\n", retReprotect);
	
	if (original_function != NULL)
		free((void*)original_function);
}


// C++ is typesafe, they said...
// I say: Yes, but at which price ?
template <class DataType>
DataType TemplateFuncInterceptFunction(DataType dt_Original_Function, unsigned long uslng_FunctionAddress,
	unsigned long uslng_modified_FunctionName, unsigned long uslng_DetourLength)
{
	return reinterpret_cast<DataType>
		(reinterpret_cast<unsigned long>
		(InterceptFunction(reinterpret_cast<void*> (uslng_FunctionAddress),
		uslng_DetourLength,
		reinterpret_cast<void*> (uslng_modified_FunctionName)
		)
		)
		);
}




extern "C" 
{

	

	void RE_RenderScene()
	{
		printf("This is the original RE_RenderScene\n");
	}

	void (*original_RE_RenderScene)(); // = &RE_RenderScene;

	void modified_RE_RenderScene()
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

// gdb MyIntercept.exe
// info address RE_RenderScene
// disas RE_RenderScene
// disas 0x012E10CD


#define RE_RenderScene_COPY 9 // Visual Studio 2015
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
		
		printf("\n\n====================== Hotpatching =============================\n");
		// HOTPATCH(RE_RenderScene);

		// Overwriting the RE_RenderScene function (JMP REL32 = 0xE9)
		original_RE_RenderScene = TemplateFuncInterceptFunction(
			original_RE_RenderScene,
			reinterpret_cast<unsigned long> (&RE_RenderScene),
			reinterpret_cast<unsigned long> (&modified_RE_RenderScene),
			static_cast<unsigned long> (RE_RenderScene_COPY)
		);

		/////// Expands to
		/////// InterceptFunction(&RE_RenderScene, RE_RenderScene_COPY, &modified_RE_RenderScene);
		/////// original_RE_RenderScene = FuncInterceptFunction(RE_RenderScene, modified_RE_RenderScene);
		printf("====================== Hotpatched ==============================\n\n");
		
		RE_RenderScene(); // Calling the modified version
		// Second time works
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
