# dynity - Dynamic assembly loader for Unity

This is a quick proof of concept runtime assembly loader for Hearthstone.

It primarily functions by calling exports of mono.dll to load a new assembly
and run a static method.

It also has a simple hook that allows debugging Hearthstone in the Visual Studio
Tools for Unity.  To set this up:

- Build the `dynity` solution to produce Inject.exe and dynity.dll.
- Build the `Test` solution.  You'll need to edit the paths in populate_lib.bat
and the Post-Build Event to point to your Hearthstone directory, and then run
populate_lib to copy dlls from Hearthstone to the lib folder.
- Set the environment variable UNITY_GIVE_CHANCE_TO_ATTACH_DEBUGGER to 1.  You
will need to restart Agent.exe and Battle.net.exe for this environment change to
take effect on Hearthstone.
- Start Hearthstone.  A Message Box should appear, reading `You can attach a
debugger now if you want`.  Run Inject.exe, then once the console window for
dynity appears (it should read `mono successfully loaded`), press Ok.
- In the `Test` solution, go to Debug > Attach Unity Debugger.
- For some reason, the mono debugger instance wasn't announcing itself, so I
needed to click on `Input IP` and enter the port number mono bound to.  To find
the port number, I used Process Explorer; the port is a TCP listen in the range
56000-56999.
- Now you should be able to place a breakpoint in the code of Test.dll, for
example at the installed MonoBehaviour's Update() (`Test.FrameBehaviour:Update`)
method, and it should be hit.  From there, I was able to use QuickWatch to look
at everything in Assembly-CSharp.  Note, however, that you are not be able to
place breakpoints in Assembly-CSharp, as there is no .mdb file for it.  The
requirement of the .mdb file to place breakpoints is the reason for pdb2mdb's
inclusion as a post-build event.

## License information

dynity is MIT licensed

**pdb2mdb** (<https://gist.github.com/jbevain/ba23149da8369e4a966f>)

- pdb2mdb is licensed under the Microsoft Public License (Ms-PL).
- Mono.Cecil is licensed under the MIT/X11 license.

The source is available in the mono project repository.

**mono bindings**

- The mono dllimport wrapper is derived from Unity mono test cases at
<https://github.com/Unity-Technologies/mono/tree/unity-staging/unity/smalltestcases/asyncsocketshutdown>
(under the MIT/X11 license)
