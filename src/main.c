#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bspfile.h"
#include "textures.h"

char* add_filename_suffix(const char* fullname) {
	const char* suffix = "_trimmed";

	char filename[255];
	char extension[255];

	filename[0] = '\0';
	extension[0] = '\0';

#ifdef WIN32
	char* separator = strrchr(fullname, '\\');
#else
	char* separator = strrchr(fullname, '/');
#endif

	if (separator) {
		strcpy(filename, separator+1);
	} else {
		strcpy(filename, fullname);
	}

	char* extension_ptr = strrchr(filename, '.');
	if (extension_ptr) {
		strcpy(extension, extension_ptr+1);
		*extension_ptr = '\0';
	}

	char* new_name = malloc(strlen(filename)+strlen(suffix)+1+strlen(extension)+1);
	new_name[0] = '\0';
	strcat(new_name, filename);
	strcat(new_name, suffix);
	strcat(new_name, ".");
	strcat(new_name, extension);
	
	return new_name;
}

int main (int argc, char *argv[])
{
	printf("BSPTexRM v1.0\n");
	if (argc < 2) {
		printf("Removes textures that are embedded in a GoldSrc BSP file.\n");
		printf("Usage: %s <bspfile>\n", argv[0]);
		return 1;
	}
	FILE* bsp_file = fopen(argv[1], "rb");
	if (!bsp_file) {
		perror("Failed opening file, error occured");
		return 1;
	}
	printf("Reading BSP file %s...\n", argv[1]);
	bspFile* input = read_bsp_file(bsp_file);
	fclose(bsp_file);

	if (input->version == -1) {
		printf("Not a valid BSP file, terminating...\n");
		free(input);
		return 1;
	}

	trim_texture_lump(&input->lump[LUMP_TEXTURES]);
	char* output_name = add_filename_suffix(argv[1]);

	printf("Writing BSP file %s...\n", output_name);
	FILE* output_file = fopen(output_name, "wb");
	write_bsp_file(output_file, input);
	fclose(output_file);

	free_bsp_file(input);
	free(output_name);

	return 0;
}
