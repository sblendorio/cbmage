#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define MAXWIDTH 480

void print(FILE *out, unsigned char *data, int width, int height);
int  getPixel(unsigned char *data, int width, int height, int x, int y, int bpp);
void usage(int argc, char *argv[]);
void die(char *message);
unsigned char *resize_width(unsigned char *input, int *in_w, int *in_h, int depth, int out_w);

int main(int argc, char *argv[]) {
	int width, height, bpp;
	unsigned char *data, *resized;

	usage(argc, argv);
	data = stbi_load(argv[1], &width, &height, &bpp, 3);
	if (data == NULL) die("I/O error: file not found");

	if (width > MAXWIDTH) {
		resized = resize_width(data, &width, &height, 3, MAXWIDTH);
		fprintf(stderr,
			"Image exceeds %d dots per row: resizing to %dx%d before printing.\n", 
			MAXWIDTH, width, height);
		data = resized;
	}

	print(stdout, data, width, height);
	stbi_image_free(data);
	return 0;
}

void print(FILE *out, unsigned char *data, int width, int height) {
	static int pow[] = {1, 2, 4, 8, 16, 32, 64};
	int row, x, dy, rows, code, pixel;
	
	rows = height / 7 + (height % 7 == 0 ? 0 : 1);
	fprintf(out, "%c", 8);
	for (row=0; row<rows; ++row) {
		for (x=0; x<width; ++x) {
			code = 128;
			for (dy=0; dy<7; ++dy) {
				pixel = getPixel(data, 3, width, height, x, (row*7) + dy);
				code += pow[dy]*pixel;
			}
			fprintf(out, "%c", code);
		}
		fprintf(out, "%c", 13);
	}
	fprintf(out, "%c%c", 15, 13);
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
	fprintf(stderr, "cbmage 0.2\n\n");
	fprintf(stderr, "Usage: %s <image file name>\n\n", argc == 0 ? "cbmage" : argv[0]);
	fprintf(stderr, "Outputs raw bytes for printing on a CBM MPS 803 compatible printer.\n");
	fprintf(stderr, "It should be used with opencbm package by Spiro Trikaliotis to redirect the\n");
	fprintf(stderr, "output to the printer, connected through a XUM1541 cable or Zoom Floppy.\n");
	fprintf(stderr, "Max picture width: 480 pixels. It will be automatically resized if exceeds.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Example:\n\n");
	fprintf(stderr, "cbmctrl reset\ncbmctrl lock\ncbmctrl listen 4 0\n");
	fprintf(stderr, "%s picture.png | cbmctrl write\n", argc == 0 ? "cbmage" : argv[0]);
	fprintf(stderr, "cbmctrl unlisten\ncbmctrl unlock\n");
	fprintf(stderr, "\n");
	exit(1);
}

void die(char *message){
	fprintf(stderr, "%s\n", message);
	exit(2);
}

unsigned char *resize_width(unsigned char *input, int *in_w, int *in_h, int depth, int out_w) {
	int out_h;
	unsigned char *output;
	out_h = (*in_h * out_w / *in_w) + ((*in_h * out_w % *in_w) == 0 ? 0 : 1);
	
	output = (unsigned char*) malloc(out_w * out_h * depth);
	stbir_resize_uint8(input, *in_w, *in_h, 0, output, out_w, out_h, 0, depth);
	stbi_image_free(input);
	*in_w = out_w;
	*in_h = out_h;
	return output;
}
