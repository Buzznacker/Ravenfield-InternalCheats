#pragma once

#include <mono\jit\jit.h>

//Attach Definition.
typedef MonoThread* (__cdecl* mono_thread_attach_t)(MonoDomain* mDomain);

//Class Definition.
typedef MonoClass* (__cdecl* mono_class_from_name_t)(MonoImage* image, const char* name_space, const char* name);
typedef MonoMethod* (__cdecl* mono_class_get_method_from_name_t)(MonoClass* mclass, const char* name, int param_count);

//Reflective Execution.
typedef MonoObject* (__cdecl* mono_runtime_invoke_t)(MonoMethod* method, void* obj, void** params, MonoObject** exc);

//Assembly Definition.
typedef MonoAssembly* (__cdecl* mono_assembly_open_t)(MonoDomain* mDomain, const char* filepath);
typedef MonoImage* (__cdecl* mono_assembly_get_image_t)(MonoAssembly *assembly);

//Domains Definition.
typedef MonoDomain* (__cdecl* mono_root_domain_get_t)();
typedef MonoDomain* (__cdecl* mono_domain_get_t)();


