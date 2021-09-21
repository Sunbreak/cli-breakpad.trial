# Build breakpad

## Prepare environment

https://chromium.googlesource.com/breakpad/breakpad/+/HEAD/README.md

- Install `depot_tools`

## Fetch code

### macOS/Linux

```sh
mkdir $BREAKPAD && cd $BREAKPAD
fetch breakpad
```

## Setting up and build

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

### macOS

```sh
$ ./breakpad/mac/dump_syms build/cli-breakpad > cli-breakpad.sym
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