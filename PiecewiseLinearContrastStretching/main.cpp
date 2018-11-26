#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#define HEIGHT 512
#define WIDTH 512

using namespace std;

typedef struct RGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

typedef struct Point {
	int x;
	int y;
};

template<typename T> T** MemAlloc2D(int nHeight, int nWidth, int nInitVal)
{
	T** rtn = new T*[nHeight];
	for (int h = 0; h < nHeight; h++)
	{
		rtn[h] = new T[nWidth];
		memset(rtn[h], nInitVal, sizeof(T) * nWidth);
	}
	return rtn;
}

template<typename T> void MemFree2D(T** arr2D, int nHeight)
{
	for (int h = 0; h < nHeight; h++)
	{
		delete[] arr2D[h];
	}
	delete[] arr2D;
}

void FileRead(const char* strFilename, RGB** arr2D, int nHeight, int nWidth)
{
	FILE* fp_in = fopen(strFilename, "rb");
	for (int h = 0; h < nHeight; h++)
	{
		fread(arr2D[h], sizeof(RGB), nWidth, fp_in);
	}

	fclose(fp_in);
}

void FileWrite(const char* strFilename, RGB** arr2D, int nHeight, int nWidth)
{
	FILE* fp_out = fopen(strFilename, "wb");
	for (int h = 0; h < nHeight; h++)
	{
		fwrite(arr2D[h], sizeof(RGB), nWidth, fp_out);
	}

	fclose(fp_out);
}

template<typename T> void PiecewiseLinear(T** in, Point p1, Point p2)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			unsigned char* pData = (unsigned char*)addressof(in[y][x]);

			for (int i = 0; i < 3; i++)
			{
				if (*(pData + i) < p1.x)
				{
					*(pData + i) = *(pData + i) * (p1.y / p1.x);
				}
				else if (*(pData + i) < p2.x)
				{
					*(pData + i) = p1.y + ((*(pData + i) - p1.x) *(p2.y - p1.y) / (p2.x - p1.x));
				}
				else
				{
					*(pData + i) = 255 + ((*(pData + i) - 255) *(255 - p2.y) / (255 - p2.x));
				}
			}
		}
	}
}

void main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// File Open & Memory Allocation

	RGB** arr2D = MemAlloc2D<RGB>(HEIGHT, WIDTH, 0);


	FileRead("lena512.rgb", arr2D, HEIGHT, WIDTH);

	PiecewiseLinear(arr2D, { 25, 50 }, { 200, 120 });

	FileWrite("lena512_out_ok33.raw", arr2D, HEIGHT, WIDTH);

}