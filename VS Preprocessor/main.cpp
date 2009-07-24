#include <iostream>
#include "image.h"
using namespace std;

void prepareImage(int min, Image &image);
bool searchForPixel(int curX, int curY, int &nextX, int &nextY, Image &image);
void identifyBlock(int x, int y, int chars[], int count, Image &image);
void findSurroundingPixels(int x, int y, int &top, int &bottom, int &left, int &right, Image &image);

int main(int argc, char **argv) {
	if(argc != 6) {
		cout << "Usage: split <in filename> <out filename> blur sharpen min" << endl;
		return -1;
	}
	
	Image image(argv[1]);
	image.blur(atof(argv[3]));
	image.sharpen(atof(argv[4]));

	prepareImage(atoi(argv[5]), image);

	int chars[strlen(argv[1]) * 4];

	int x;
	int y;
	int count = 0;

	while(searchForPixel(0, 0, x, y, image)) {
		identifyBlock(x, y, chars, count, image);
		count++;
	}

	image.save(argv[2]);	
	//cout << "I found " << count << " letters." << endl;
	if(count == (strlen(argv[1])-4)) {
		for(int i = 0; i < count; i++) {
			printf("%c %i %i %i %i\n", argv[1][i], chars[i*4 + 3], chars[i*4 + 2], chars[i*4 + 1], chars[i*4 + 0]);
		}

		return 0;
	}

	for(int i = 0; i < count; i++) {
		printf("%i %i %i %i\n", chars[i*4 + 3], chars[i*4 + 2], chars[i*4 + 1], chars[i*4 + 0]);
	}
	
	return -1;
}

void prepareImage(int min, Image &image) {
	for(int x = 0; x < image.width; x++) {
		for(int y = 0; y < image.height; y++) {
			for(int i = 0; i < 3; i++) {
				if(image(x, y, i) < min) {
					image(x, y, 0) = 100;
					image(x, y, 1) = 100;
					image(x, y, 2) = 100;
					break;
				}
			}
		}
	}
}

bool searchForPixel(int curX, int curY, int &nextX, int &nextY, Image &image) {
	for(nextX = curX; nextX < image.width; nextX++) {
		for(nextY = curY; nextY < image.height; nextY++) {
			if(image(nextX, nextY) == 100)
				return true;
		}
	}
	return false;
}

void identifyBlock(int x, int y, int chars[], int count, Image &image) {
	int top = y;
	int bottom = y;
	int left = x;
	int right = x;

	findSurroundingPixels(x, y, top, bottom, left, right, image);

	int color[] = {255, 0, 0};
	//image.draw_rectangle(left, top, right, bottom, color);

	//image.get_crop(left, top, right, bottom).save();

	left--;
	right++;
	top--;
	bottom++;
	image.draw_line(left, top, left, bottom, color);
	image.draw_line(left, bottom, right, bottom, color);
	image.draw_line(right, bottom, right, top, color);
	image.draw_line(left, top, right, top, color);

	int c = 0;

	//Top right
	for(int x = (left + right)/2; x < right; x++) {
		for(int y = top; y < (top + bottom)/2; y++) {
			if(image(x, y) == 0)
				c++; //lolz
		}
	}
	chars[count*4] = c*100/((left + right)/2 * (top + bottom)/2);

	//top left
	for(int x = left; x < (left + right)/2; x++) {
		for(int y = top; y < (top + bottom)/2; y++) {
			if(image(x, y) == 0)
				c++; //lolz
		}
	}
	chars[count*4 + 1] = c*100/((left + right)/2 * (top + bottom)/2);

	//bottom left
	for(int x = left; x < (left + right)/2; x++) {
		for(int y = (top + bottom)/2; y < bottom; y++) {
			if(image(x, y) == 0)
				c++; //lolz
		}
	}
	chars[count*4 + 2] = c*100/((left + right)/2 * (top + bottom)/2);


	//bottom right
	for(int x = (left + right)/2; x < right; x++) {
		for(int y = (top + bottom)/2; y < bottom; y++) {
			if(image(x, y) == 0)
				c++; //lolz
		}
	}
	chars[count*4 + 3] = c*100/((left + right)/2 * (top + bottom)/2);
}

void findSurroundingPixels(int x, int y, int &top, int &bottom, int &left, int &right, Image &image) {
	image(x, y, 0) = 0;
	image(x, y, 1) = 0;
	image(x, y, 2) = 0;

	if(y < top)
		top = y;

	if(y > bottom)
		bottom = y;

	if(x < left)
		left = x;

	if(x > right)
		right = x;

	for(int dx = -1; dx < 2; dx++) {
		for(int dy = -1; dy < 2; dy++) {
			if((x + dx) >= image.width || (x + dx) < 0 || (y + dy) >= image.height || (y + dy) < 0)
				continue;

			if(image((x + dx), (y + dy)) == 100)
				findSurroundingPixels((x + dx), (y + dy), top, bottom, left, right, image);
		}
	}
}

