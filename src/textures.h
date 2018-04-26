#include <stdint.h>
#include "bspfile.h"

#define MAXTEXTURENAME 16
#define MIPLEVELS 4

typedef struct {
	uint32_t num_textures; // Number of BSPMIPTEX structures
	int32_t* texture_offsets;
} textureHeader;

typedef struct
{
	char name[MAXTEXTURENAME];  // Name of texture
	uint32_t width, height;     // Extends of the texture
	uint32_t miptex_offsets[MIPLEVELS]; // Offsets to texture mipmaps BSPMIPTEX;
} texture;

typedef struct
{
	textureHeader header;
	texture* textures;
} textureLump;

void trim_texture_lump(bspLump* tex_lump);
