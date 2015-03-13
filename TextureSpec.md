# XreaL Engine Texturing Guidelines #

## Quick Summary ##

  * Use TGA unless you have a damn good reason not to. PNG is ok sometimes, but has poor alpha support.
  * Use JPG and die a painful death
  * Not all forms of DDS will load
  * Make your source art BIG, as large as you could ever imagine it needing to be, and keep to powers of two.
  * ATI cards cannot display over 2048x2048, and NVIDIA cards 4096x4096.

## Texture dimensions ##

Texture dimensions may be any powers of two from 32 to 4096. Textures need not be square. Sizes such
as 256x256, 1024x128, and 2048x32 are acceptable, but should not be smaller than 32x32. Non power-of-
two textures will work, but will be internally scaled to the next power of two, wasting memory, so please only use such textures for testing purposes, never for final art.

Please keep your source art large and uncompressed, and include it with your work for archiving. The
"average" size of textures right now is about **512x512**, but if you keep your art as 1024x1024, it'll be
reusable in the future, or as an option for higher-end video cards. Exact sizes will of course depend on
usage.

## Texturing Guidelines ##

This is a next-generation video game. If you're coming from texturing games like Quake3, Unreal
Tournament, etc... throw everything you know out the window. You now have to learn to think an entirely
different way about your textures.
So, what does all of this mean to you as a texture artist?

  1. DO NOT paint shadows into your textures - use a bump map
  1. DO NOT paint highlights into your textures - use a specular map and a bump map
  1. DO NOT paint glows into your textures - use an illumination map

## Other Things You Need To Know ##

Recommended size for typical source texture art (meaning the size it is created at) is 1k (1024x1024).
However, the larger the better in most cases due to future scalability concerns. Also, use your head. A
poker card for instance does not need to be 1k in size.

Don't paint things that should be geometry into textures. We want to see real lights and real panels rather
than fake textured ones. Ask a modeler if you have an idea and need help realizing it. Specular maps
should be considered for all surfaces, but obviously not if the surface is going to be matte. There's no
sense in having a completely black specular map.

Light maps are NOT bump maps. Light maps were used in the olden days to create shadows and the
illusion of light falling on to textures. Bump maps are superior to light maps in every way. Do not confuse
the two. If you are used to making light maps, the process for creating a bump map is NOT THE SAME.