# _This document is still WIP, please check back for updates_ #

# Introduction #

Tremulous uses Id Tech 3 (Quake 3) map format and shaders. Dretchstorm in the other hand is based on Xreal which is Id Tech 4 (Doom3) compatible with extensions. In order to use Tremulous maps in Dretchstorm those maps must be converted.

## Differences between Xreal and Quake 3 maps ##
  1. Xreal uses a different map format: Doom 3-compatible
  1. In Xreal (D3), shaders are known as material files .mtr
  1. Tremulous (Q3) maps are compiled with "q3map2", Xreal with "xmap2"
  1. Tremulous (Q3) maps use a global lightmap system for rendering lights on maps. In Xreal (D3), lights have individual color, volume and intensity.

# Steps Summary #

The updates for maps are as follow:

  1. Convert tremulous/quake3 map to Xreal format
  1. Update all 256px textures to 512px or 1024px, mostly for walls and floors
  1. Make diffuse, normal, specular maps of all textures, as needed
  1. Rename shader file (.shader) to materials file (.mtr) and make Xreal/Doom3 changes to the shader definitions
  1. Fix all light classes for color and volume (converting from quake3 to doom3 light system)
  1. Compile with xmap2
  1. Archive in pk3 with all unique assets, lightmaps images and materials