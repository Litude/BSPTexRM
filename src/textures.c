#include <string.h>
#include <stdlib.h>
#include "textures.h"

static void update_offsets(textureLump* tex_lump) {
	for (uint32_t i = 0; i < tex_lump->header.num_textures; ++i) {
		tex_lump->header.texture_offsets[i] = sizeof(uint32_t) + sizeof(uint32_t) * tex_lump->header.num_textures + sizeof(texture) * i;
	}
}

static void lump_to_buffer(textureLump* tex_lump, bspLump* tex_buffer) {
	memcpy(tex_buffer->data, &tex_lump->header.num_textures, sizeof(uint32_t));
	for (uint32_t i = 0; i < tex_lump->header.num_textures; ++i) {
		memcpy(&tex_buffer->data[sizeof(int32_t) * i] + sizeof(uint32_t), &tex_lump->header.texture_offsets[i], sizeof(int32_t));
	}

	uint32_t header_size = tex_lump->header.num_textures * sizeof(int32_t) + sizeof(uint32_t);
	for (uint32_t i = 0; i < tex_lump->header.num_textures; ++i) {
		memcpy(&tex_buffer->data[header_size+i*sizeof(texture)], &tex_lump->textures[i], sizeof(texture));
	}
}

void trim_texture_lump(bspLump* tex_buffer) {
	textureLump tex_lump;
	memcpy(&(tex_lump.header.num_textures), tex_buffer->data, sizeof(int32_t));
	tex_lump.header.texture_offsets = malloc(tex_lump.header.num_textures * sizeof(int32_t));

	memcpy(tex_lump.header.texture_offsets, tex_buffer->data+sizeof(int32_t), tex_lump.header.num_textures * sizeof(int32_t));

	tex_lump.textures = malloc(tex_lump.header.num_textures * sizeof(texture));

	for (uint32_t i = 0; i < tex_lump.header.num_textures; ++i) {
		memcpy(&(tex_lump.textures[i].name), &(tex_buffer->data[tex_lump.header.texture_offsets[i]]), 16);
		memcpy(&(tex_lump.textures[i].width), &(tex_buffer->data[tex_lump.header.texture_offsets[i]]) + 16, sizeof(uint32_t));
		memcpy(&(tex_lump.textures[i].height), &(tex_buffer->data[tex_lump.header.texture_offsets[i]]) + 16 + sizeof(uint32_t), sizeof(uint32_t));
		//set all miptex offsets to zero since they will be removed anyway
		memset(tex_lump.textures[i].miptex_offsets, 0, sizeof(uint32_t) * MIPLEVELS);
	}

	update_offsets(&tex_lump);

	bspLump* new_buffer;
	new_buffer = malloc(sizeof(bspLump));
	new_buffer->length = tex_lump.header.num_textures * (sizeof(texture) + sizeof(int32_t)) + sizeof(uint32_t);
	if (tex_buffer->length == new_buffer->length) {
		printf("WARNING: Size of old and new texture lump matches; there are probably no textures included in the original BSP file.\n");
	} else {
		printf("Old texture lump length: %d\n", tex_buffer->length);
		printf("New texture lump length: %d\n", new_buffer->length);
	}

	memset(&new_buffer->offset, 0, sizeof(int32_t));

	new_buffer->data = malloc(new_buffer->length);
	lump_to_buffer(&tex_lump, new_buffer);

	free(tex_lump.header.texture_offsets);
	free(tex_lump.textures);

	memcpy(tex_buffer->data, new_buffer->data, new_buffer->length);
	tex_buffer->length = new_buffer->length;

	free(new_buffer->data);
	free(new_buffer);
}