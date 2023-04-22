# LdtkImport

LdtkImport is an MIT-licensed C++ library for importing a subset of [.ldtk file](https://ldtk.io/json/) data. It specifically imports [Auto-layers](https://ldtk.io/docs/general/auto-layers/) and its [Rules](https://ldtk.io/docs/general/auto-layers/auto-layer-rules/), then performs the rule matching process to allow dynamically creating new levels during runtime.

* Engine-agnostic: This is a bare-bones implementation that will only load the data. The only dependency it has is [yyjson](https://github.com/ibireme/yyjson) for parsing the ldtk file's json text.
* A separate git repo uses this library for [LÖVE](https://github.com/love2d/love) as an example.


# Motivation

Virtually all [LDtk importing libraries](https://ldtk.io/api/) I have seen say that the Rules data should be ignored. They all copy this part of the [official documentation](https://ldtk.io/json/#ldtk-AutoRuleDef):

>This complex section isn't meant to be used by game devs at all, as these rules are completely resolved internally by the editor before any saving. You should just ignore this part.

This is a missed opportunity, as the Rules data is ideal if you want to create new Levels at runtime.

To clarify, neither LDtk's Rules nor the LdtkImport library does any procedural map generation. What Rules do, is automate the tile placement process, including random variations using a well-defined set of rules.

The LDtk editor itself also does a good job of being beginner-friendly, making it easy to pick up for level designers (at least, easier to understand than [Tiled](https://www.mapeditor.org/)'s equivalent [Automapping](https://doc.mapeditor.org/en/stable/manual/automapping/) feature, in my experience!).

Preferably, you should pair LdtkImport with a procedural map generation library: take the output of the procedural map generator, which would just be a 2d grid of pure data, and pass that to LdtkImport. Along with a properly made .ldtk file, LdtkImport will run the rule matching process, and give you the data that tells you which tiles in a tilesheet should be drawn where, how many times, what their Z-order is, etc.

This also opens up the potential for a runtime map editor for user-generated content, though that is beyond the scope of this importing library.


# Building

Right now, only Visual Studio project files exist, but there is nothing Windows-specific in the code. I plan to eventually add CMake files for ease of porting, once I understand CMake.

LdtkImport is designed to work on at least C++14 standard.


## Dependencies

The project has a vcpkg.json for these dependencies.

* [yyjson](https://github.com/ibireme/yyjson)
* [catch2](https://github.com/catchorg/Catch2) (only for the unit tests)


## Tests

Unit tests are included (in the tests subfolder) using the [catch2](https://github.com/catchorg/Catch2) library.


# Reference Manual

There is no reference manual yet, but all the public classes/structs and methods have Doxygen comments.

If you are not familiar with the structure of an .ldtk file, it's recommended to read the [official documentation](https://ldtk.io/docs/game-dev/json-overview/) first.


# Contributing

As mentioned, this library only supports a subset of the [LDtk file schema](https://ldtk.io/files/JSON_SCHEMA.json). Just to name a few of the limitations: it doesn't import regular tile layers, entities (and that includes enum properties), or world data at all. It doesn't import the existing Level data (i.e. layer instances), preferring Levels to be generated at runtime. It also uses only one IntGrid per Level.

This wasn't exactly a deliberate design goal. It was more of a "hmm well my game doesn't need this LDtk feature, so I'm going to ignore it" series of spontaneous decisions (I would be using procedural map generation a lot, so it made no sense to be placing entities manually, ergo I ignored entities completely).

So, if you want to improve this library by adding the missing features, then go ahead and fork it and/or submit a pull request.
