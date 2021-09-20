# Build breakpad

## Prepare environment

https://chromium.googlesource.com/breakpad/breakpad/+/HEAD/README.md

- Install `depot_tools`

## Fetch code

### Linux

```sh
mkdir $BREAKPAD && cd $BREAKPAD
fetch breakpad
```

## Setting up and build

### Linux

```sh
cd src
./configure && make
```

## Install library && tools

### Linux

```sh
mkdir -p ./breakpad/linux/$(arch)
cp $BREAKPAD/src/src/client/linux/libbreakpad_client.a ./breakpad/linux/$(arch)
cp $BREAKPAD/src/src/tools/linux/dump_syms/dump_syms ./breakpad/linux/$(arch)
cp $BREAKPAD/src/src/processor/minidump_stackwalk ./breakpad/linux/$(arch)
```

# Use breakpad for cli-breakpad

## Build & Run cli-breakpad

### Linux

```sh
$ cmake -S . -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo
$ cmake --build build
$ ./build/cli-breakpad
Dump path: ./7df95962-acd7-488b-ac443ebd-933ebb6a.dmp
Segmentation fault (core dumped)
```

## Dump info for cli-breakpad

### Linux

```sh
$ ./breakpad/linux/$(arch)/dump_syms build/cli-breakpad > cli-breakpad.sym
$ uuid=`awk 'FNR==1{print \$4}' cli-breakpad.sym`
$ mkdir -p symbols/cli-breakpad/$uuid/
$ mv ./cli-breakpad.sym symbols/cli-breakpad/$uuid/
$ ./breakpad/linux/$(arch)/minidump_stackwalk 7df95962-acd7-488b-ac443ebd-933ebb6a.dmp symbols > cli-breakpad.log
```