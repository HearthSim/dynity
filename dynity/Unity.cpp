#include "Unity.h"

#include "LoadMonoDynamically.h"

#include "Log.h"

const char *gAssemblyPath;
const char *gAssemblyMethodDesc;

// This hook enables the debugger.  mono_set_commandline_arguments is called in the release version of Unity Player
// right after the debugger is initialized in the development version of Unity Player.
void mono_set_commandline_arguments_hook(int argc, const char **argv, char *baseDir) {
	LogPrintln("mono_set_commandline_arguments hook called");
	const char *jitOpts = "--debugger-agent=transport=dt_socket,embedding=1,defer=y";
	char *jitArgv[1];
	jitArgv[0] = _strdup(jitOpts);
	mono_jit_parse_options(1, jitArgv);
	mono_debug_init(1);

	free(jitArgv[0]);

	mono_set_commandline_arguments(argc, argv, baseDir);
}

void UnityInit() {
	LoadMono();

#if UNITY_ENABLE_MONO_LOGGING
	mono_unity_set_vprintf_func((vprintf_func)vprintf);
	mono_trace_set_level_string("debug");
	mono_trace_set_mask_string("all");
#endif

#if UNITY_USE_ASSEMBLY_TXT
	FILE *asmTxt;
	fopen_s(&asmTxt, "assembly.txt", "r");
	char *assemblyPath = (char *)malloc(MAX_PATH);
	char *methodDesc = (char *)malloc(MAX_PATH);
	assemblyPath[0] = 0;
	methodDesc[0] = 0;
	fscanf_s(asmTxt, "%[^\n]", assemblyPath, MAX_PATH);
	if (fgetc(asmTxt) > 0) {
		fscanf_s(asmTxt, "%[^\n]", methodDesc, MAX_PATH);
		fclose(asmTxt);
	}
	if (!strlen(assemblyPath)) {
		LogPrintln("No assembly path supplied in assembly.txt");
		free(assemblyPath);
		assemblyPath = 0;
	}
	if (!strlen(methodDesc)) {
		LogPrintln("No method desc supplied in assembly.txt");
		free(methodDesc);
		methodDesc = 0;
	}
	gAssemblyPath = assemblyPath;
	gAssemblyMethodDesc = methodDesc;
#else
	gAssemblyPath = UNITY_ASSEMBLY_PATH;
	gAssemblyMethodDesc = UNITY_ASSEMBLY_METHOD_DESC;
#endif

#if UNITY_ENABLE_RELEASE_DEBUGGING
	// Hearthstone.exe exports the address of imported mono_* API functions.  We can use this to very easily install our
	// hook:
	HMODULE mainModule = GetModuleHandle(0);
	fp_mono_set_commandline_arguments *val =
	    (fp_mono_set_commandline_arguments *)GetProcAddress(mainModule, "mono_set_commandline_arguments");
	*val = (fp_mono_set_commandline_arguments)mono_set_commandline_arguments_hook;

	// For release debugging, we are called when a MessageBox is being shown; here we sleep for 10 seconds before
	// loading the assembly to allow for the message box to close and for Unity to load.
	Sleep(10000);
#endif
}

void UnityTest() {
	if (!gAssemblyPath || !gAssemblyMethodDesc) {
		LogPrintln("Not loading assembly: invalid args");
	}
	LogPrintf("Loading assembly: %s\n", gAssemblyPath);
#define CheckVar(x, l)                                                                                                 \
	{                                                                                                                  \
		if (!x) {                                                                                                      \
			LogPrintf("%s is null; aborting\n", #x);                                                                   \
			goto l;                                                                                                    \
		}                                                                                                              \
	}
	MonoDomain *rootDomain = mono_get_root_domain();
	CheckVar(rootDomain, fail);
	MonoThread *thread = mono_thread_attach(rootDomain);
	CheckVar(thread, fail);
	MonoAssembly *assembly = mono_domain_assembly_open(rootDomain, gAssemblyPath);
	CheckVar(assembly, thread_fail);
	MonoImage *image = mono_assembly_get_image(assembly);
	CheckVar(image, thread_fail);
	LogPrintf("Invoking method: %s\n", gAssemblyMethodDesc);
	MonoMethodDesc *desc = mono_method_desc_new(gAssemblyMethodDesc, true);
	CheckVar(desc, thread_fail);
	MonoMethod *method = mono_method_desc_search_in_image(desc, image);
	CheckVar(method, desc_fail);
	mono_runtime_invoke(method, 0, 0, 0);
	LogPrintln("Invocation successful");
desc_fail:
	mono_method_desc_free(desc);
thread_fail:
	mono_thread_detach(thread);
fail:
	return;
}
