#pragma once
#include<stdint.h>

struct RGB {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct RGBA {
	uint8_t alpha;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct BitmapImage {
	uint32_t width;
	uint32_t height;
	RGB** img;
};

struct BitmapImageWithAlpha {
	uint32_t width;
	uint32_t height;
	RGBA** img;
};

ref class removeGreenScreenArgsStruct {
public:
	int column;
	System::Drawing::Bitmap^ bmp;
	RGBA* source;
	RGBA* dest;
	uint32_t len;
	uint8_t minDif;
	bool masm;
};

extern uint32_t convertRGBATouint32(const RGBA source);

void removeGreenScreen(BitmapImageWithAlpha source, BitmapImageWithAlpha &dest, uint8_t minDif);
ref class GreenScreen {
public:
	static int currentThreads = 0;
	GreenScreen(int id, int n, System::Threading::ManualResetEvent^ doneEvent)
	{
		//currentThreads = 0;
	}

	static void removeGreenScreenRow_delegate(Object^ obj);
	static void removeGreenScreenRow(RGBA* source, RGBA* dest, uint32_t len, uint8_t minDif);
	
	static void setBitmapRow() {

	}
};
extern "C" int _stdcall removeGreenScreenAssembly(RGBA * source, RGBA * dest, uint32_t len, uint8_t minDif);

extern "C" int _stdcall rmGreenScreen(RGBA * source, RGBA * dest, uint32_t len, uint8_t minDif);