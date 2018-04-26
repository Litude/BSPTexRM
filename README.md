# BSPTexRM
Command line tool for removing textures that are embedded in a GoldSrc (Half-Life engine) BSP file.

## Purpose
This tool only removes the bitmap entries from the textures lump, not the complete texture entries. Removing these entries makes it trivial to replace the textures by loading a .WAD file instead which includes textures with the same name as the original textures but of course the textures can be whatever.

Note that this does not extract the textures, just removes them. For actually extracting the textures use a different tool instead such as Textract (command line) or WinTextract (GUI).

## Usage
Run the application with the name of the BSP file that you wish to remove the textures from. As an example (assuming a BSP file name of de_cityofsin_cz.bsp):

``BSPTexRM de_cityofsin_cz.bsp`` (Windows)<br />
``./BSPTexRM de_cityofsin_cz.bsp`` (Linux)

The tool will output a file with the ''_trimmed'' suffix which has the texture entries removed.

## Compiling
This utilizes CMake for compilation. On Windows use the open CMake project in Visual Studio 2017 to compile.

On Linux, clone or download the repository and navigate to the root. Then run the following commands:
```
mkdir bin
cd bin
cmake ../src
make
```
