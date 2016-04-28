// If 1, will redirect mono's trace debug output to the dynity log.
#define UNITY_ENABLE_MONO_LOGGING 1
// The {GameName}_Data folder present in the same directory as the .exe
#define UNITY_DATA_PATH "Hearthstone_Data"
// If 1, use assembly.txt in the same directory as the .exe to define the assembly path and method desc.  The assembly
// path should be on the first line, and the method desc should be on the second line.
#define UNITY_USE_ASSEMBLY_TXT 0
// Path to the assembly dll.  This path may be relative or absolute; if relative, it will be relative within the same
// directory as the .exe.
#define UNITY_ASSEMBLY_PATH "Hearthstone_Data\\Managed\\Test.dll"
// Description of the method to be invoked.  `Namespace.Class:Method()`
#define UNITY_ASSEMBLY_METHOD_DESC "Test.ComponentTest:Run()"
// If 1, this will install a hook to enable the mono debugger.  dynity should be loaded on Unity startup.  To achieve
// this, set UNITY_GIVE_CHANCE_TO_ATTACH_DEBUGGER to 1 and a MessageBox will show on start up.  Then, inject dynity, and
// press Ok on the message box.
#define UNITY_ENABLE_RELEASE_DEBUGGING 1

void UnityInit();
void UnityTest();
