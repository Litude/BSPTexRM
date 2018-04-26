#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bspfile.h"

bspFile* read_bsp_file(FILE* filestream) {
	//Returns -1 as version if a reading error occured

	bspFile* file_data;
	file_data = malloc(sizeof(bspFile));
	fread(&(file_data->version), sizeof(int32_t), 1, filestream);
	if (file_data->version != 30) {
		fprintf(stderr, "Invalid BSP version or not a BSP file.\n");
		file_data->version = -1;
		return file_data;
	}

	for (uint32_t i = 0; i < HEADER_LUMPS; ++i) {
		fread(&(file_data->lump[i].offset), sizeof(int32_t), 1, filestream);
		fread(&(file_data->lump[i].length), sizeof(int32_t), 1, filestream);
	}

	for (uint32_t i = 0; i < HEADER_LUMPS; ++i) {
		bspLump* current_lump = &(file_data->lump[i]);

		fseek(filestream, current_lump->offset, SEEK_SET);
		current_lump->data = malloc(current_lump->length);
		//Check that read equals length
		if (fread(current_lump->data, sizeof(char), current_lump->length, filestream) != current_lump->length) {
			fprintf(stderr, "Unexpected end of lump in BSP file.\n");
			file_data->version = -1;
			for (uint32_t j = 0; j <= i; ++j) {
				current_lump = &(file_data->lump[j]);
				free(current_lump->data);
			}
			return file_data;
		}
		memset(&(current_lump->offset), 0, sizeof(uint32_t));
	}

	return file_data;
}

void free_bsp_file(bspFile* bsp_file) {
	for (uint32_t i = 0; i < HEADER_LUMPS; ++i) {
		free(bsp_file->lump[i].data);
	}
	free(bsp_file);
}

static int write_lump(int lump_number, FILE* filestream, bspFile* bsp_file) {
	char padding = 0;
	bspLump* current_lump = &(bsp_file->lump[lump_number]);
	int padding_amount = current_lump->length % 4;
	if (padding_amount) {
		padding_amount = 4 - padding_amount;
	}

	current_lump->offset = ftell(filestream);
	fwrite(current_lump->data, current_lump->length, 1, filestream);

	for (int i = 0; i < padding_amount; ++i) {
		fwrite(&padding, 1, 1, filestream);
	}
	return 1;
}

static int write_header(FILE* filestream, bspFile* bsp_file) {
	fseek(filestream, 0, SEEK_SET);
	fwrite(&(bsp_file->version), sizeof(int32_t), 1, filestream);
	for (uint32_t i = 0; i < HEADER_LUMPS; ++i) {
		fwrite(&(bsp_file->lump[i].offset), sizeof(int32_t), 1, filestream);
		fwrite(&(bsp_file->lump[i].length), sizeof(int32_t), 1, filestream);
	}
	return 1;
}

int write_bsp_file(FILE* filestream, bspFile* bsp_file) {
	//write header with empty offsets
	write_header(filestream, bsp_file);

	write_lump(LUMP_PLANES, filestream, bsp_file);
	write_lump(LUMP_LEAVES, filestream, bsp_file);
	write_lump(LUMP_VERTICES, filestream, bsp_file);
	write_lump(LUMP_NODES, filestream, bsp_file);
	write_lump(LUMP_TEXINFO, filestream, bsp_file);
	write_lump(LUMP_FACES, filestream, bsp_file);
	write_lump(LUMP_CLIPNODES, filestream, bsp_file);
	write_lump(LUMP_MARKSURFACES, filestream, bsp_file);
	write_lump(LUMP_SURFEDGES, filestream, bsp_file);
	write_lump(LUMP_EDGES, filestream, bsp_file);
	write_lump(LUMP_MODELS, filestream, bsp_file);
	write_lump(LUMP_LIGHTING, filestream, bsp_file);
	write_lump(LUMP_VISIBILITY, filestream, bsp_file);
	write_lump(LUMP_ENTITIES, filestream, bsp_file);
	write_lump(LUMP_TEXTURES, filestream, bsp_file);

	//rewrite header since lump writing has stored offsets
	write_header(filestream, bsp_file);
	return 1;
}