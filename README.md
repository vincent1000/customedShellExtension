# customedShellExtension
a customed shell extension for both arm and x64
The solution includes three projects:
1. wap: package project
2. WindowsExplorerCMD:build shell extension dll
3. WpfApp1: an empty wpf application

First, build dll with a specific platform like arm64,x64 or anycpu. The dll will be built and output under wap project. Then build package with wap publish. The dll will be wrapped in package. 
Currently, it works well in x64 machine with anycpu and x64 package. But when it can run on arm machine, dllhost.exe(x86) will load it but there is no menu displayed when right click on image or audio/video files. It can only work well when dll is built with arm64 and dllhost.exe(arm64) load it.
