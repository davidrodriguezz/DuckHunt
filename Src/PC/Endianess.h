#pragma once
#include <SDL_endian.h>


#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define PLATFORM_IS_LITTLE_ENDIAN  
#else
#define PLATFORM_IS_BIG_ENDIAN  
#endif


#define SWAP_ENDIANESS_32(n) n = ((n>>24) & 0xff) | ((n << 8) & 0xff0000) |((n >> 8) & 0xff00) | ((n << 24) & 0xff000000);