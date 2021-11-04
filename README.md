# Build breakpad

## Prepare environment

https://chromium.googlesource.com/breakpad/breakpad/+/HEAD/README.md

- Install `depot_tools`

## Fetch code

#### Windows

```bat
rem Command Prompt
> git clone https://chromium.googlesource.com/external/gyp
> git clone https://chromium.googlesource.com/breakpad/breakpad
```

### macOS/Linux

```sh
mkdir $BREAKPAD && cd $BREAKPAD
fetch breakpad
```

## Setting up and build

### Windows

```bat
rem Command Prompt
> %GYP%\gyp.bat --no-circular-check breakpad\src\client\windows\breakpad_client.gyp -Dwin_release_RuntimeLibrary=2 -Dwin_debug_RuntimeLibrary=3
```

```bat
rem Developer Command Prompt
> cd %BREAKPAD%\src\client\windows
> DevEnv breakpad_client.sln /Upgrade
> DevEnv breakpad_client.sln /Build "Debug|x64" /Project exception_handler
exception_handler.vcxproj -> C:\Users\sunbr\google\breakpad\src\client\windows\Debug\lib\exception_handler.lib
> DevEnv breakpad_client.sln /Build "Debug|x64" /Project common
common.vcxproj -> C:\Users\sunbr\google\breakpad\src\client\windows\Debug\lib\common.lib
> DevEnv breakpad_client.sln /Build "Debug|x64" /Project crash_generation_client
crash_generation_client.vcxproj -> C:\Users\sunbr\google\breakpad\src\client\windows\Debug\lib\crash_generation_client.lib
```

```bat
> cd %BREAKPAD%\src\tools\windows\dump_syms
> DevEnv dump_syms.sln /Upgrade
> DevEnv dump_syms.sln /Build "Release|x64" /Project dump_syms
```

### macOS

```sh
cd $BREAKPAD/src
./configure && make
cd $BREAKPAD/src/client/mac && xcodebuild -target Breakpad
cd $BREAKPAD/src/tool/mac/dump_syms && xcodebuild -target dump_syms
```

### Linux

```sh
cd $BREAKPAD/src
./configure && make
```

## Install library && tools

### Windows

```bat
rem Command Prompt
> mkdir -p breakpad\windows\%PROCESSOR_ARCHITECTURE%\Debug
> xcopy %BREAKPAD%\src\client\windows\Debug\lib\* breakpad\windows\%PROCESSOR_ARCHITECTURET%\Debug\
> xcopy %BREAKPAD%\src\tools\windows\dump_syms\Release\dump_syms.exe breakpad\windows\%PROCESSOR_ARCHITECTURE%\
rem TODO minidump_stackwalk
```

### macOS

```sh
mkdir -p ./breakpad/mac/$(arch)
cp -r $BREAKPAD/src/src/client/mac/build/Release/Breakpad.framework ./breakpad/mac/
cp $BREAKPAD/src/src/tools/mac/dump_syms/build/Release/dump_syms ./breakpad/mac/
cp $BREAKPAD/src/src/processor/minidump_stackwalk ./breakpad/mac/$(arch)
mkdir Frameworks && cd Frameworks && ln -s ../breakpad/mac/Breakpad.framework .
```

### Linux

```sh
mkdir -p ./breakpad/linux/$(arch)
cp $BREAKPAD/src/src/client/linux/libbreakpad_client.a ./breakpad/linux/$(arch)
cp $BREAKPAD/src/src/tools/linux/dump_syms/dump_syms ./breakpad/linux/$(arch)
cp $BREAKPAD/src/src/processor/minidump_stackwalk ./breakpad/linux/$(arch)
```

# Use breakpad for cli-breakpad

## Build & Run cli-breakpad

### Windows

```bat
rem Developer Command Prompt
> cmake -S . -Bbuild
> cmake --build build --config Debug
> build\Debug\cli-breakpad
dump_path: .
minidump_id: 887a36fe-d37e-4b79-97c4-133e36a1c24e
```

### macOS

```sh
$ cmake -S . -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo
$ cmake --build build
$ ./build/cli-breakpad
dump_dir: .
minidump_id: 64C31053-BBE4-400C-9588-B29F63E08E71
```

### Linux

```sh
$ cmake -S . -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo
$ cmake --build build
$ ./build/cli-breakpad
Dump path: ./7df95962-acd7-488b-ac443ebd-933ebb6a.dmp
Segmentation fault (core dumped)
```

## Dump info for cli-breakpad

### Windows

> If `CoCreateInstance CLSID_DiaSource {E6756135-1E65-4D17-8576-610761398C3C} failed (msdia*.dll unregistered?)` when running `dump_syms`
>
> ```bat
> rem Administrator: Command Prompt
> > regsvr32 "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\Remote Debugger\x64\msdia140.dll"
> ```

```bat
rem Command Prompt
> breakpad\windows\%PROCESSOR_ARCHITECTURE%\dump_syms build\Debug\cli-breakpad.pdb > cli-breakpad.sym
```

> Use Linux's `minidump_stackwalk` because it is unable to build Windows's `minidump_stackwalk` right now

```sh
# Linux
$ uuid=`awk 'FNR==1{print \$4}' cli-breakpad.sym`
$ mkdir -p symbols/cli-breakpad.pdb/$uuid/
$ mv ./cli-breakpad.sym symbols/cli-breakpad.pdb/$uuid/
$ ./breakpad/linux/$(arch)/minidump_stackwalk 887a36fe-d37e-4b79-97c4-133e36a1c24e.dmp symbols > cli-breakpad.log
```

### macOS

```sh
$ dsymutil ./build/cli-breakpad -o cli-breakpad.dSYM
$ ./breakpad/mac/dump_syms cli-breakpad.dSYM > cli-breakpad.sym
$ uuid=`awk 'FNR==1{print \$4}' cli-breakpad.sym`
$ mkdir -p symbols/cli-breakpad/$uuid/
$ mv ./cli-breakpad.sym symbols/cli-breakpad/$uuid/
$ ./breakpad/mac/$(arch)/minidump_stackwalk 64C31053-BBE4-400C-9588-B29F63E08E71.dmp symbols > cli-breakpad.log
```

### Linux

```sh
$ ./breakpad/linux/$(arch)/dump_syms build/cli-breakpad > cli-breakpad.sym
$ uuid=`awk 'FNR==1{print \$4}' cli-breakpad.sym`
$ mkdir -p symbols/cli-breakpad/$uuid/
$ mv ./cli-breakpad.sym symbols/cli-breakpad/$uuid/
$ ./breakpad/linux/$(arch)/minidump_stackwalk 7df95962-acd7-488b-ac443ebd-933ebb6a.dmp symbols > cli-breakpad.log
```