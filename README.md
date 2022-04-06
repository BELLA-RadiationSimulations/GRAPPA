# GRAPPA (Geant4 RApid Pair Production Application)

## Overview
 GRAPPA is a Geant4 application that simulates the interaction of an incoming particle beam
 (typically electron or photons) with an High-Z material foil.

## Build

GRAPPA is a Geant4 (G4) application, therefore it requires G4 on the system.
A comprehensive guide on how to install G4 can be found on the [G4 installation guide](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html).

### Requirements

GRAPPA requires:
- `Geant4` installed on the system
- `C++` compiler (standard required C++-17)

Besides, other optional requirements will enable extra features:
- Mutithreading support: G4 built with `-DGEANT4_BUILD_MULTITHREADED:BOOL=ON`
- Visualization support: In order to produce visual outputs, you must install some of the G4 provided visualization drivers (_e.g._ OpenGL, RayTracer, QT5, etc...)
- For nicer plots, `Freetype` libraries should be available. G4 must be built with `-DGEANT4_USE_FREETYPE:BOOL=ON` for `Freetype` support
- For the analysis of the `ROOT` (default) output file, `ROOT` should be installed on the system ([ROOT](https://root.cern))

### Build instructions

GRAPPA is best built using cmake. From the source folder type

```
mkdir -p build; cd build
cmake .. -DGeant4_DIR={G4 cmake directory on the system}
cmake --build . --config Release --target install
```

You should provide `cmake` with the G4 installation directory containing the `Geant4Config.cmake` file. Typically, it can be found in `${G4_base_install_dir}/lib/Geant4-version`.
By default, GRAPPA is installed in the `bin` directory of the source code folder.

**Optional**:
- `-DGRAPPA_USE_HDF5=ON` compiles GRAPPA with HDF5 support and generate `*.h5` output files. You must provide a G4 installation compiled with HDF5 support (`-DGEANT4_USE_HDF5=ON`).

## Run

GRAPPA can either run using an interactive user interface (`UI`) or using a provided macro script.
Before running, the G4 initialization script, provided by G4, must be sourced.
It is located in the bin directory of the G4 installation directory and should be sourced with

```
source ${G4_base_install_dir}/bin/geant4.sh
```

### Interactive GRAPPA

Launch GRAPPA with no extra argument using
```
./GRAPPA
```
and you will be prompted to the interactive session.
From there, you can control the simulation via the G4 `UI` commands.

### Execute macro

In order to execute a macro, it should be provided when running the program as
```
./GRAPPA /path/to/macro
```
Examples of macros can be found in the `script/run` folder.

## Data analysis

We provide some ROOT scripts to facilitate the analysis of the final GRAPPA data.
Those are automatically installed in the `bin` folder as `scriptname.C`.
You can copy them in your output folder and execute them in `ROOT` as
```
>> root scriptname.C
```
For more insights and to learn how to customize the scripts, visit the
[ROOT manual](https://root.cern/manual/).

|:warning: **Warning**: no utility is provided for the HDF5 output files.|
| --- |
