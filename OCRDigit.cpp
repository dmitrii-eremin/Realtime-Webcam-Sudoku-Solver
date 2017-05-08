#include "stdafx.h"
#include "OCRDigit.h"


// Next array represents the result of the training of the zones intensity features. Greater the number, the zone is darker.
// It is automatically generated in the function Train()
// The function Train() also creates the file zones.bmp, which shows how this array looks.
const int OCRDigit::m_zon[9][5][5] = {
358,	678,	962,	1002,	794,	
43,		314,	945,	988,	565,	
51,		277,	909,	931,	404,	
374,	604,	1024,	953,	449,	
479,	767,	973,	821,	449,	

754,	1024,	1000,	993,	815,	
274,	678,	736,	422,	218,	
25,		105,	549,	870,	379,	
406,	362,	364,	958,	798,	
568,	916,	994,	883,	440,	

650,	886,	841,	828,	494,	
326,	310,	345,	907,	913,	
14,		224,	689,	1024,	568,	
236,	314,	421,	920,	624,	
512,	884,	881,	842,	421,	

86,		90,		253,	624,	514,	
670,	763,	829,	1024,	805,	
411,	683,	586,	834,	557,	
36,		321,	743,	1019,	567,	
0,		33,		427,	814,	478,	

641,	886,	880,	788,	416,	
399,	451,	344,	783,	968,	
520,	735,	604,	876,	786,	
667,	1024,	707,	501,	278,	
349,	873,	902,	880,	617,	

505,	884,	861,	922,	537,	
952,	755,	220,	846,	973,	
1013,	981,	567,	752,	541,	
716,	1024,	533,	332,	190,	
189,	680,	938,	966,	639,	

221,	739,	697,	206,	0,	
202,	738,	963,	410,	38,	
33,		269,	840,	738,	203,	
316,	395,	663,	944,	589,	
833,	1024,	982,	1008,	910,	

529,	819,	752,	866,	609,	
876,	812,	272,	777,	1024,	
529,	940,	759,	1007,	721,	
666,	1002,	606,	870,	798,	
416,	799,	806,	855,	558,	

506,	865,	890,	628,	232,	
197,	388,	510,	946,	740,	
573,	855,	628,	978,	1024,	
865,	891,	272,	726,	972,	
422,	842,	842,	867,	450
};

// Next array represents the result of the training of the (width*100/height) ratios features.
// It is generated in the function Train()
// Note how all the values are similar (~64), except the first (46), which is number 1.
const int OCRDigit::m_ratios[9] = {46, 64, 64, 66, 64, 66, 60, 66, 66}; 


OCRDigit::OCRDigit(void)
{}

OCRDigit::~OCRDigit(void)
{}

//////////////////////////////////////////////////////////////////////////
// Function:	Classify()
// Purpose:		Classifies the passed blob. It compares the blob to m_zon[] and m_ratios[]. Less difference is, more probable that this is the value.
//				It is simple 'k-nearest neighbours' method with k=1.
// Parameters:	blob - input blob to be OCR-ed
//				lb, rt - pixel coordinates of the input blob
//				cell - cell to update. Updated are members cell->OCR[] and cell->value.
// Returns:		
/////////////////////////////////////////////////////////////////////////
void OCRDigit::Classify(const BYTE* blob, const POINT& lb, const POINT& rt, SudSolverCell* cell)
{
	int test[5][5]={0};
	int x, y, i, sum, count, roof=0, result=-1;
	int width  = rt.x - lb.x;
	int height = rt.y - lb.y;

	if (width < 2 || height < 5)
		return;

	// create grid 5x5
	int xslice[6];
	int yslice[6];
	xslice[0] = lb.x + 0;
	xslice[1] = lb.x + 1*width / 5;
	xslice[2] = lb.x + 2*width / 5;
	xslice[3] = lb.x + 3*width / 5;
	xslice[4] = lb.x + 4*width / 5;
	xslice[5] = lb.x + width-1;
	yslice[0] = lb.y + 0;
	yslice[1] = lb.y + 1*height / 5;
	yslice[2] = lb.y + 2*height / 5;
	yslice[3] = lb.y + 3*height / 5;
	yslice[4] = lb.y + 4*height / 5;
	yslice[5] = lb.y + height-1;

	for (y=0; y<5; y++)
	{	for (x=0; x<5; x++)
		{	sum = count = 0;
			for (int yy=yslice[y+1]; yy>=yslice[y]; yy--)
			{	for (int xx=xslice[x+1]; xx>=xslice[x]; xx--)
				{	count++;
					if (*(blob + yy*100 + xx) == 0)	// black
					{	sum += 1024;
			}	}	}
			test[y][x] = sum/count;
			if (roof < test[y][x])
				roof = test[y][x];
		}
	}
	if (roof == 0)
		roof = 1;	// avoid division by zero

	// normalize the 5x5 grid
	for (y=0; y<5; y++)
	{	for (x=0; x<5; x++)
		{	test[y][x] = (test[y][x] * 1024) / roof;
	}	}

	// next loop compares the test[] with all nine m_zons[] by summing up the differences between test[] and m_zons[].
	// the goal is to detect the lowest difference.
	roof = INT_MAX;
	for (i=0; i<9; i++)
	{	sum = 0;
		for (y=0; y<5; y++)
		{	for (x=0; x<5; x++)
			{	sum += abs(test[y][x] - m_zon[i][y][x]);
		}	}

		// include ratios comparing
		sum += abs(width*100/height - m_ratios[i]) * 300;

		if (roof > sum)
		{	roof = sum;
			result = i + 1;
		}
		cell->OCR[i+1] = sum;
	}

	// digit 1 needs a special handling
	if (result != 1)
	{	if ((width*100)/height < 37)	// if blob ratio (height:width) is less than 37% this is digit 1 for sure. Because is too slim to be anything else.
		{	result = 1;
			cell->OCR[1] = 0;
		}
	}
	else
	{	if ((width*100)/height > 53)	// if blob ratio (height:width) is more than 53% this can't be digit 1 for sure. It is too thick for 1.
		{	cell->OCR[1] = INT_MAX;
			// find next probable result between digits 2-9
			roof = INT_MAX;
			for (i=2; i<10; i++)
			{	if (roof > cell->OCR[i])
				{	roof = cell->OCR[i];
					result = i;
				}
			}
		}
	}

	cell->value = result;
}

//////////////////////////////////////////////////////////////////////////
// Function:	Train()
// Purpose:		Trains the numbers images and creates an "ideal" bitmaps.
//				It dumps the result on debug output window, ready for copy-paste to variables m_zon and m_ratios.
//				Also creates zone.bmp file to see how the "ideal" numbers looks like.
// Parameters:
// Returns:		
/////////////////////////////////////////////////////////////////////////
void OCRDigit::Train()
{	int zon[9][5][5]={0};
	RatioWH ratio[9]={0};
	CFileFind finder;
	BOOL finding;

	finding = finder.FindFile(CString ("res\\1\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 1, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\2\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 2, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\3\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 3, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\4\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 4, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\5\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 5, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\6\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 6, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\7\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 7, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\8\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 8, zon, ratio);
	}
    finder.Close();
	finding = finder.FindFile(CString ("res\\9\\*.bmp"));
	while (finding)
	{
		finding = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		Train(filename, 9, zon, ratio);
	}
    finder.Close();

	// Normalize zones
	for (int i=0; i<9; i++)
	{	int roof = 0;
		for (int y=0; y<5; y++)
		{	for (int x=0; x<5; x++)
			{	if (zon[i][y][x] > roof)
					roof = zon[i][y][x];
			}
		}
		double factor = 1024.0 / roof;

		for (int y=0; y<5; y++)
		{	for (int x=0; x<5; x++)
			{	zon[i][y][x] = (int)(zon[i][y][x] * factor);
			}
		}
	}

	// output to Output window ready for copy-paste.
	TRACE("\n\nratios=");
	for (int i=0; i<9; i++)
		TRACE("%d, ", ratio[i].ratio/ratio[i].count);

	TRACE("\n\n");
	for (int i=0; i<9; i++)
	{	for (int y=0; y<5; y++)
		{	for (int x=0; x<5; x++)
			{
				TRACE("%d,\t", zon[i][y][x]);
			}
			TRACE("\n");
		}
		TRACE("\n");
	}

	// Write to bmp
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	//Create the BMP file
	infoheader.biSize = sizeof(BITMAPINFOHEADER);
	infoheader.biWidth = 5;
	infoheader.biHeight = 45;
	infoheader.biPlanes = 1;
	infoheader.biBitCount = 24;
	infoheader.biCompression = BI_RGB;
	infoheader.biSizeImage = ((((infoheader.biWidth * infoheader.biBitCount) + 31) & ~31) >> 3) * infoheader.biHeight;
	infoheader.biXPelsPerMeter = 0;
	infoheader.biYPelsPerMeter = 0;
	infoheader.biClrUsed = 0;
	infoheader.biClrImportant = 0;
	int nBitsOffset = sizeof(BITMAPFILEHEADER) + infoheader.biSize; 
    LONG lImageSize = infoheader.biSizeImage;
    LONG lFileSize = nBitsOffset + lImageSize;
    fileheader.bfType = 'B'+('M'<<8);
    fileheader.bfOffBits = nBitsOffset;
    fileheader.bfSize = lFileSize;
    fileheader.bfReserved1 = fileheader.bfReserved2 = 0;

	FILE* pFile;
	fopen_s(&pFile, "zones.bmp", "wb");
	fwrite((char*) &fileheader, 1, sizeof(BITMAPFILEHEADER), pFile);
	fwrite((char*) &infoheader, 1, sizeof(BITMAPINFOHEADER), pFile);
	for (int i=0; i<9; i++)
	{	for (int y=0; y<5; y++)
		{	for (int x=0; x<5; x++)
			{	char c = zon[i][y][x] / 5;
				fwrite(&c, 1, 1, pFile );
				fwrite(&c, 1, 1, pFile );
				fwrite(&c, 1, 1, pFile );
			}
			fwrite(&zon, 1, 1, pFile );		// 4 bytes allign
		}
	}
	fclose(pFile);
}

void OCRDigit::Train(CString filename, int digit, int (*zon)[5][5], RatioWH *ratio)
{
	int sum, count;

	// MUST be grayscale image
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
	if (hBitmap != NULL)
	{	CBitmap bmp;
		bmp.Attach(hBitmap);
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);
		BYTE* data = ((BYTE*)bmpInfo.bmBits);
		
		// create grid 5x5
		int xslice[6] = {0};
		int yslice[6] = {0};
		xslice[1] = 1*bmpInfo.bmWidth / 5;
		xslice[2] = 2*bmpInfo.bmWidth / 5;
		xslice[3] = 3*bmpInfo.bmWidth / 5;
		xslice[4] = 4*bmpInfo.bmWidth / 5;
		xslice[5] = bmpInfo.bmWidth-1;
		yslice[1] = 1*bmpInfo.bmHeight / 5;
		yslice[2] = 2*bmpInfo.bmHeight / 5;
		yslice[3] = 3*bmpInfo.bmHeight / 5;
		yslice[4] = 4*bmpInfo.bmHeight / 5;
		yslice[5] = bmpInfo.bmHeight-1;

		for (int y=0; y<5; y++)
		{	for (int x=0; x<5; x++)
			{	sum = count = 0;
				for (int yy=yslice[y+1]; yy>=yslice[y]; yy--)
				{	for (int xx=xslice[x+1]; xx>=xslice[x]; xx--)
					{	count++;
						BYTE* pixel = data + yy*bmpInfo.bmWidthBytes + xx;
						//if (*pixel == 0)	// black
						{	sum += 255 - *pixel;
							//pDC->SetPixel(100+xx+x, 100+yy+y, RGB(0,0,255));
				}	}	}
				zon[digit-1][y][x] += sum/count;
				//pDC->SetPixel(100+x, 100+y, RGB(255-sum/count,255-sum/count,255-sum/count));
			}
		}

		ratio[digit-1].count += 1;
		ratio[digit-1].ratio += bmpInfo.bmWidth*100/bmpInfo.bmHeight;
	}
}
