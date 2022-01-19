#include"GreenScreenRemove.h"
#include <utility>
inline bool isGreenMax(RGBA source) {
	return (source.green > source.blue && source.green > source.red);
}

void removeGreenScreen(const BitmapImageWithAlpha source, BitmapImageWithAlpha &dest, uint8_t minDif){
	for (int y = 0; y < source.height; y++) {
		for (int x = 0; x < source.width; x++) {
			if (isGreenMax(source.img[y][x])){
				int dif = ((int)source.img[y][x].green -
					std::max((int)source.img[y][x].red,(int)source.img[y][x].blue));
				if (minDif <= dif) {
					dest.img[y][x].alpha = 0;
				}
				else {
					dest.img[y][x].alpha = 255;
				}
			}
			else {
				dest.img[y][x].alpha = 255;
			}
			

			dest.img[y][x].red = source.img[y][x].red;
			dest.img[y][x].green = source.img[y][x].green;
			dest.img[y][x].blue = source.img[y][x].blue;

		}
	}
}


void GreenScreen::removeGreenScreenRow_delegate(Object^ obj)
{
	removeGreenScreenArgsStruct^ args = (removeGreenScreenArgsStruct^)obj;
	//removeGreenScreenRow(args->source, args->dest, args->len,args->minDif);
	RGBA* source = args->source;
	RGBA* dest = args->dest;
	uint32_t len = args->len;
	uint32_t minDif = args->minDif;
	bool masm = args->masm;

	//removeGreenScreenAssembly(source, dest, len ,0);
	if(masm)rmGreenScreen(source, dest, len, minDif);
	else
	{
		removeGreenScreenRow(source, dest, len, minDif);
	}
	System::Threading::Interlocked::Decrement(GreenScreen::currentThreads);
}



void GreenScreen::removeGreenScreenRow(RGBA* source, RGBA* dest, uint32_t len, uint8_t minDif) {
	for (int x = 0; x < len; x++) {
		if (isGreenMax(source[x])) {
			int dif = ((int)source[x].green - std::max((int)source[x].red, (int)source[x].blue));
			if (minDif <= dif) {
				dest[x].alpha = 0;
			}
			else {
				dest[x].alpha = 255;
			}
		}
		else {
			dest[x].alpha = 255;
		}

	}
}