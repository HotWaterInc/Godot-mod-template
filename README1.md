# Godot-mod-template

This should be the starting point for developing godot modules. You depend only on godot-cpp and godot build tools. Here
you will have scripts and instructions for all you need to get a quickstart

## Getting started

### Clone this repository

```
git clone git@github.com:HotWaterInc/Godot-mod-template.git 
```

### Run this repository

```
cd Godot-mod-template
cmake -DBUILD_FOR_EDITOR=True . -B build
cmake --build build
```

### Prerequisites

If you dont have godot installed, consider using [this CLI](https://github.com/HotWaterInc/GodotHub) to set up godot and
the prerequisites for building modules

#### Note about godot-cpp

The godot-cpp project is not included as a submodule because further update may break the compatibility with the godot
version you are using. So you should clone the godot-cpp repository and checkout the version you are using.
There are also some precompiled binaries for linux in there that take a long time to compile.