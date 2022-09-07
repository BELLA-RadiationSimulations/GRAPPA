# GRAPPA (Geant4 RApid Pair Production Application)

# Overview
 GRAPPA is a Geant4 application that simulates the interaction of an incoming particle beam
 (typically electron or photons) with an High-Z material foil.
 The basic GRAPPA geometry consists of an incoming beam and a parallelepipedon target that can be rotated around the vertical axis.
 Particle detection is performed at the boundary of the _world_, that consists in a sphere that surrounds the initial particles and the target.
 Final positions, momenta and proper time are registered by the detector, that is triggered by the passage of

 - Primary particles
 - Electrons
 - Positrons
 - Pions
 - Muons
 - Photons

# Build

GRAPPA is a Geant4 (G4) application, therefore it requires G4 on the system.
A comprehensive guide on how to install G4 can be found on the [G4 installation guide](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html).

## Requirements

GRAPPA requires:
- `Geant4` installed on the system
- `C++` compiler (standard required C++-17)

Besides, other optional requirements will enable extra features:
- Mutithreading support: G4 built with `-DGEANT4_BUILD_MULTITHREADED:BOOL=ON`
- Visualization support: In order to produce visual outputs, you must install some of the G4 provided visualization drivers (_e.g._ OpenGL, RayTracer, QT5, etc...)
- For nicer plots, `Freetype` libraries should be available. G4 must be built with `-DGEANT4_USE_FREETYPE:BOOL=ON` for `Freetype` support
- For the analysis of the `ROOT` (default) output file, `ROOT` should be installed on the system ([ROOT](https://root.cern))

## Build instructions

GRAPPA is best built using cmake. From the source folder type

```
mkdir -p build; cd build
cmake .. -DGeant4_DIR={G4 cmake directory on the system}
cmake --build . --config Release --target install
```

You should provide `cmake` with the G4 installation directory containing the `Geant4Config.cmake` file. Typically, it can be found in `${G4_base_install_dir}/lib/Geant4-version`.
By default, GRAPPA is installed in the `bin` directory of the source code folder.

---

# Run a simulation and postprocess data

GRAPPA can either run using an interactive user interface (`UI`) or using a provided macro script.
Before running, the G4 initialization script, provided by G4, must be sourced.
It is located in the bin directory of the G4 installation directory and should be sourced with

```
source ${G4_base_install_dir}/bin/geant4.sh
```

## Interactive GRAPPA

Launch GRAPPA with no extra argument using
```
./GRAPPA
```
and you will be prompted to the interactive session.
From there, you can control the simulation via the G4 `UI` commands.

## Execute macro

In order to execute a macro, it should be provided when running the program as
```
./GRAPPA /path/to/macro
```
Examples of macros can be found in the `script/run` folder.

## Structure of macro files

Macro files in GRAPPA act as input file, providing commands to initialize
simulations, change parameters, set up the in-situ data analysis, change the physics, _etc..._

GRAPPA accepts all the basic Geant4 commands, and introduces some specific ones.
Here we only discuss some simple examples, for a more complete guide please look into the 
[G4 application developers guide](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/index.html).
In a typical macro file, one wants to address some aspects of the simulation:

- Define the initial particle source
- Assemble the target geometry
- Modify the physics package
- Personalize the data analysis
- Set up the visualization
- Start the Run
- Others

Macro files are saved using the `.mac` extension and interpret
everything after a `#` as comments.

### Particle source

GRAPPA can shoot particle either from a [General Particle Source](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/GettingStarted/generalParticleSource.html?highlight=gps) (that is the default behavior),
where the beam statistics must be specified,
or from a file of pre-generated list positions and momenta.

> Choice between GPS and particles from file must be made BEFORE the run initialization

An example of GPS settings is
```
# =/=/=/=/= Particle source section =/=/=/=/=

# Set the particle source: must be done after run initialization.
# Particle source must be chosen between e- and gamma (for our use)

/gps/verbose 0 # verbose level of the GPS
/gps/particle e- #electron chosen as source

# Particle source spatial information

/gps/pos/type Beam # This is the source type, it represents a beam in the focal position
/gps/pos/centre 0 0 5 mm
/gps/pos/shape Circle
# The radius parameter defines a central flat plateau
/gps/pos/radius 0 mm
# The sigma_r parameter adds a gaussian uncertainty around the center
/gps/pos/sigma_r 10 um

# Angular distribution of the beam
/gps/ang/type beam1d  
/gps/ang/sigma_r 0.002 rad # Beam divergence

# Energy distribution of the beam
/gps/ene/mono 10 GeV # Central beam energy
/gps/ene/type Gauss
/gps/ene/sigma 10 MeV # Beam energy spread
```

The command to extract particles from a given ASCII file is

```
/particle_source/ParticlesFromFile true
/particle_source/FileName Filename.txt

...
# Run initialization
/run/initialize
...

# Define particle type (default is electrons)
/gun/particle gamma
```

### Target geometry

Commands to modify the geometry are entirely custom for GRAPPA.
They **must** be invoked before the run is initialized in order to avoid the loss of the geometry.

```
# Modify geometry (custom functions)
/geometry/world/radius 120 cm
/geometry/target/size 10 mm
/geometry/target/center 0 0 0 mm
/geometry/target/thickness 3 mm
/geometry/target/rotation 45 deg
/geometry/target/material G4_W
```

The world is a sphere and surrounds everything else.
Sensitive detectors are positioned _on_ the world boundary,
such that when a particle reaches it, it is registered before being killed.
The rotation parameter refers to the target rotation around the y-axis.

> Tip: if you don't remember the syntax for some GRAPPA commands, run a simulation in interactive mode. There you can find a full list of commands with their usage cases.



## Data analysis

We provide some ROOT scripts to facilitate the analysis of the final GRAPPA data.
Those are automatically installed in the `bin` folder as `scriptname.C`.
You can copy them in your output folder and execute them in `ROOT` as
```
>> root scriptname.C
```
For more insights and to learn how to customize the scripts, visit the
[ROOT manual](https://root.cern/manual/).
