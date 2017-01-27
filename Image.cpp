///must include the following in linker settings to use jpeg module, obviously you must also have the turbojpeg library installed
/**

-lturbojpeg

**/

#include "Image.h"

#include <SOIL/SOIL.h>

void Image::load(string file)
{
	data = (ClrRGB*)SOIL_load_image(file.c_str(), &w, &h, 0, SOIL_LOAD_RGB);
}

/*
void Image::loadJpg(string file)
{
	unsigned char * rawData;
	 
	std::fstream file;
	
	file
	
	const long unsigned int dataSize=0; //not sure what this is
	
	makeEmpty();
	
	tjhandle decompressor=tjInitDecompress();
	int subsamp;
	
	tjDecompressHeader2(decompressor, (unsigned char *)data, dataSize, &w, &h, &subsamp);
	
	//data=new ClrRGB[w*h];
	
	tjDecompress2(decompressor, rawData, dataSize, (unsigned char *)data, w, 0, h, TJPF_RGB, TJFLAG_BOTTOMUP);
	
	tjDestroy(decompressor);
}
*/

/*
void Image::saveJpg(string file)
{
	tjhandle compressor=tjInitCompress();
	
	if (!data)
	{
		cout << "Image::saveJpg called on empty image" << endl;
	}
	else
	{
		tjCompress2(compressor, (unsigned char *)img->getData(), dim.x, 0, dim.y, TJPF_BGR, &data, &dataSize, TJSAMP_444, quality, TJFLAG_BOTTOMUP);
	}
	
	tjDestroy(compressor);
}
*/

void Image::makeEmpty()
{
	if (data)
		SOIL_free_image_data((unsigned char *)data);
	
	data=nullptr;
	
	w=0;
	h=0;
}
