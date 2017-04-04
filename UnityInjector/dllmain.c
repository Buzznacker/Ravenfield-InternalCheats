#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>

#include "mono.h"

PVOID GetMonoFunction(char* funcName)
{
	return GetProcAddress(GetModuleHandle(L"mono.dll"), funcName);
}

VOID AttachMonoAndInject(LPWSTR cheatDllPath)
{
	char buffer[500];
	wcstombs(buffer, cheatDllPath, 500);

	//Attaching Procedure
	mono_thread_attach_t mono_thread_attach = GetMonoFunction("mono_thread_attach");

	//Class informations.
	mono_class_from_name_t mono_class_from_name = GetMonoFunction("mono_class_from_name");
	mono_class_get_method_from_name_t mono_class_get_method_from_name = GetMonoFunction("mono_class_get_method_from_name");

	//Code Execution.
	mono_runtime_invoke_t mono_runtime_invoke = GetMonoFunction("mono_runtime_invoke");

	//Assembly informations.
	mono_assembly_open_t mono_assembly_open_ = GetMonoFunction("mono_domain_assembly_open");
	mono_assembly_get_image_t mono_assembly_get_image_ = GetMonoFunction("mono_assembly_get_image");

	//Domain informations.
	mono_root_domain_get_t mono_root_domain_get = GetMonoFunction("mono_get_root_domain");
	mono_domain_get_t mono_domain_getnormal = GetMonoFunction("mono_domain_get");

	//Prepare the domain to be ready for code-execution.
	mono_thread_attach(mono_root_domain_get());

	//Get the current domain we are in.
	MonoDomain* domain = mono_domain_getnormal();

	//Open our C# Dll (future will just be passed as an arg in the manual map injection)
	MonoAssembly* domainAssembly = mono_assembly_open_(domain, buffer);

	//Getting our file-module as our assembly_image
	MonoImage* image = mono_assembly_get_image_(domainAssembly);

	//Declaring our class inside the assembly (Image, NameSpace, ClassName)
	MonoClass* pClass = mono_class_from_name(image, "GameHack", "Hack");

	//Getting our method which attaches our assembly to the game, essentially our entry point. (Class, MethodName, Params)
	MonoMethod* monoLoadMethod = mono_class_get_method_from_name(pClass, "Entry", 0);

	//Invoke our method.
	mono_runtime_invoke(monoLoadMethod, NULL, NULL, NULL);
}

INT __stdcall DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (DLL_PROCESS_ATTACH == ul_reason_for_call)
	{
		AttachMonoAndInject(lpReserved);
		FreeLibrary(hInstance);
	}
	return 0;
}