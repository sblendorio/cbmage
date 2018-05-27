#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void print(unsigned char *data, int width, int height);
int  getPixel(unsigned char *data, int width, int height, int x, int y, int bpp);
void usage(int argc, char *argv[]);
void die(char *message);

int main(int argc, char *argv[]) {
	int width, height, bpp;
	unsigned char *data;

	usage(argc, argv);
	data = stbi_load(argv[1], &width, &height, &bpp, 3);
	if (data == NULL) die("I/O error: file not found");

	print(data, width, height);
	stbi_image_free(data);
	return 0;
}

void print(unsigned char *data, int width, int height) {
	static int pow[] = {1, 2, 4, 8, 16, 32, 64};
	int row, x, dy, rows, code, pixel;
	
	rows = height / 7 + (height % 7 == 0 ? 0 : 1);
	printf("%c", 8);
	for (row=0; row<rows; ++row) {
		for (x=0; x<width; ++x) {
			code = 128;
			for (dy=0; dy<7; ++dy) {
				pixel = getPixel(data, 3, width, height, x, (row*7) + dy);
				code += pow[dy]*pixel;
			}
			printf("%c", code);
		}
		printf("%c", 13);
	}
	printf("%c%c", 15, 13);
}

int getPixel(unsigned char *data, int bpp, int width, int height, int x, int y) {
	if (x >= width || y >= height || x < 0 || y < 0) return 0;

	unsigned long addr = (x + width * y) * bpp;
	unsigned char r = data[addr];
	unsigned char g = data[addr+1];
	unsigned char b = data[addr+2];
	
	return (r==255 && g==255 && b==255) ? 0 : 1;
}

void usage(int argc, char *argv[]) {
	if (argc > 1) return;
	printf("cbmage 0.1\n\n");
	printf("Usage: %s <image file name>\n\n", argc == 0 ? "cbmage" : argv[0]);
	printf("Outputs raw bytes for printing on a CBM MPS 803 compatible printer.\n");
	printf("It should be used with opencbm package by Spiro Trikaliotis to redirect the\n");
	printf("output to the printer, connected through a XUM1541 cable or Zoom Floppy.\n");
	printf("Max width of the picture: 480 pixels. It\'s not automatically resized.\n");
	printf("\n");
	printf("Example:\n\n");
	printf("cbmctrl reset\ncbmctrl lock\ncbmctrl listen 4 0\n");
	printf("%s picture.png | cbmwrite\n", argc == 0 ? "cbmage" : argv[0]);
	printf("cbmctrl unlisten\ncbmctrl unlock\n");
	printf("\n");
	exit(1);
}

void die(char *message){
	fprintf(stderr, "%s\n", message);
	exit(2);
}
