#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED
#pragma once

class ImageLoading
{
public:
	ImageLoading(char* ps, int w, int h);
	~ImageLoading();

	/* An array of the form (R1, G1, B1, R2, G2, B2, ...) indicating the
	 * color of each pixel in image.  Color components range from 0 to 255.
	 * The array starts the bottom-left pixel, then moves right to the end
	 * of the row, then moves up to the next column, and so on.  This is the
	 * format in which OpenGL likes images.
	 */
	char* pixels;
	int width;
	int height;
};

ImageLoading* loadBMP(const char* filename);










#endif