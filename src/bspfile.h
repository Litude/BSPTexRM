#ifndef BSPFILE_H
#define BSPFILE_H

/*
This code is mostly based on information available at The hlbsp Project:
http://hlbsp.sourceforge.net/index.php?content=bspdef
*/
#include <stdio.h>
#include <stdint.h>

#define LUMP_ENTITIES      0
#define LUMP_PLANES        1
#define LUMP_TEXTURES      2
#define LUMP_VERTICES      3
#define LUMP_VISIBILITY    4
#define LUMP_NODES         5
#define LUMP_TEXINFO       6
#define LUMP_FACES         7
#define LUMP_LIGHTING      8
#define LUMP_CLIPNODES     9
#define LUMP_LEAVES       10
#define LUMP_MARKSURFACES 11
#define LUMP_EDGES        12
#define LUMP_SURFEDGES    13
#define LUMP_MODELS       14
#define HEADER_LUMPS      15

typedef struct {
	int32_t offset; // File offset to data
	int32_t length; // Length of data
	char* data; // Data
} bspLump;

typedef struct {
	int32_t version;           // Must be 30 for a valid HL BSP file
	bspLump lump[HEADER_LUMPS]; // Stores the directory of lumps
} bspFile;

bspFile* read_bsp_file(FILE* filestream);
int write_bsp_file(FILE* filestream, bspFile* bsp_file);
void free_bsp_file(bspFile* bsp_file);

#endif
