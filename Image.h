#pragma once

#include "utils.h"

struct ClrRGB
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Image
{
public:
	
	void load(string file);
	//void saveJpg(string file);
	ClrRGB* getData() {return data;}
	~Image() {makeEmpty();}
	void makeEmpty();
	
private:
	int w=0, h=0;
	ClrRGB* data=nullptr;
};

