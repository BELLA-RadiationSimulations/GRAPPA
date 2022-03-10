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
- Visualization support: In order to produce visual outputs, you must install some of the G4 provided visualization drivers (_e.g._ OpenGL, RayTracer, etc...)
- `HDF5` libraries to produce `.h5` binary files as outputs. If `HDF5` is not available, a `.csv` file will be produced (stronlgy discouraged). G4 must be built with `-DGEANT4_USE_HDF5:BOOL=ON` for `HDF5` support.
- For nicer plots, `Freetype` libraries should be available. G4 must be built with `-DGEANT4_USE_FREETYPE:BOOL=ON` for `Freetype` support

### Build instructions

GRAPPA is best build using cmake. From the source folder type

```
mkdir -p build; cd build
cmake .. -DGeant4_DIR={G4 directory on the system}
cmake --build . --config Release --target install
```

You should provide `cmake` with the G4 installation directory containing the `Geant4Config.cmake` file. Typically, it can be found in `${G4_base_install_dir}/lib/Geant4-version`.

## Run

GRAPPA can either run using an interactive user interface (`UI`) or using a provided macro script.
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
Examples of macros can be found in the `example` folder.
