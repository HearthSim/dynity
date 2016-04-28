DO_API(void, mono_unity_set_vprintf_func, (vprintf_func func))
DO_API(void, mono_trace_set_level_string, (const char *value))
DO_API(void, mono_trace_set_mask_string, (const char *value))

DO_API(void, mono_set_commandline_arguments, (int, const char* argv[], const char*))
DO_API(void, mono_jit_parse_options, (int argc, char * argv[]))
DO_API(void, mono_debug_init, (int format))

DO_API(MonoDomain *, mono_get_root_domain, ())
DO_API(MonoThread *, mono_thread_attach, (MonoDomain * domain))
DO_API(void, mono_thread_detach, (MonoThread * thread))
DO_API(MonoAssembly *, mono_domain_assembly_open, (MonoDomain * domain, const char *name))
DO_API(MonoImage *, mono_assembly_get_image, (MonoAssembly * assembly))
DO_API(MonoMethodDesc *, mono_method_desc_new, (const char *name, gboolean include_namespace))
DO_API(void, mono_method_desc_free, (MonoMethodDesc * desc))
DO_API(MonoMethod *, mono_method_desc_search_in_image, (MonoMethodDesc * desc, MonoImage *image))
DO_API(MonoObject *, mono_runtime_invoke, (MonoMethod * method, void *obj, void **params, MonoObject **exc))
#undef DO_API
