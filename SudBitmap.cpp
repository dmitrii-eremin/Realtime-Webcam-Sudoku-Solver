#include "stdafx.h"
#include "SudBitmap.h"
#include "OCRDigit.h"


Accumulator::Accumulator(const int imgWidth, const int imgHeight)
{	rhos      = (int)((imgWidth + imgHeight) * SQRT2);
	rhoOffset = rhos/2;
	rhosLow   = rhos/7;
	rhosHigh  = rhos - rhos/7;
	thetas    = 180;

	// prepare accumulator array - initialize it with zeroes
	bins = new int[rhos*thetas];
	ZeroMemory(bins, sizeof(int)*rhos*thetas);
}
// lookup table of sin and cos values so we don't need to calculate sines and cosines all the time 
const float Accumulator::sins[180] = {(float)0.0,(float)0.017452,(float)0.034899,(float)0.052336,(float)0.069756,(float)0.087156,(float)0.104528,(float)0.121869,(float)0.139173,(float)0.156434,(float)0.173648,(float)0.190809,(float)0.207912,(float)0.224951,(float)0.241922,(float)0.258819,(float)0.275637,(float)0.292372,(float)0.309017,(float)0.325568,(float)0.342020,(float)0.358368,(float)0.374607,(float)0.390731,(float)0.406737,(float)0.422618,(float)0.438371,(float)0.453990,(float)0.469472,(float)0.484810,(float)0.500000,(float)0.515038,(float)0.529919,(float)0.544639,(float)0.559193,(float)0.573576,(float)0.587785,(float)0.601815,(float)0.615662,(float)0.629320,(float)0.642788,(float)0.656059,(float)0.669131,(float)0.681998,(float)0.694658,(float)0.707107,(float)0.719340,(float)0.731354,(float)0.743145,(float)0.754710,(float)0.766044,(float)0.777146,(float)0.788011,(float)0.798636,(float)0.809017,(float)0.819152,(float)0.829038,(float)0.838671,(float)0.848048,(float)0.857167,(float)0.866025,(float)0.874620,(float)0.882948,(float)0.891007,(float)0.898794,(float)0.906308,(float)0.913545,(float)0.920505,(float)0.927184,(float)0.933580,(float)0.939693,(float)0.945519,(float)0.951057,(float)0.956305,(float)0.961262,(float)0.965926,(float)0.970296,(float)0.974370,(float)0.978148,(float)0.981627,(float)0.984808,(float)0.987688,(float)0.990268,(float)0.992546,(float)0.994522,(float)0.996195,(float)0.997564,(float)0.998630,(float)0.999391,(float)0.999848,(float)1.0,(float)0.999848,(float)0.999391,(float)0.998630,(float)0.997564,(float)0.996195,(float)0.994522,(float)0.992546,(float)0.990268,(float)0.987688,(float)0.984808,(float)0.981627,(float)0.978148,(float)0.974370,(float)0.970296,(float)0.965926,(float)0.961262,(float)0.956305,(float)0.951057,(float)0.945519,(float)0.939693,(float)0.933580,(float)0.927184,(float)0.920505,(float)0.913545,(float)0.906308,(float)0.898794,(float)0.891007,(float)0.882948,(float)0.874620,(float)0.866025,(float)0.857167,(float)0.848048,(float)0.838671,(float)0.829038,(float)0.819152,(float)0.809017,(float)0.798635,(float)0.788011,(float)0.777146,(float)0.766044,(float)0.754710,(float)0.743145,(float)0.731354,(float)0.719340,(float)0.707107,(float)0.694658,(float)0.681998,(float)0.669131,(float)0.656059,(float)0.642788,(float)0.629321,(float)0.615661,(float)0.601815,(float)0.587785,(float)0.573576,(float)0.559193,(float)0.544639,(float)0.529919,(float)0.515038,(float)0.500000,(float)0.484810,(float)0.469472,(float)0.453991,(float)0.438371,(float)0.422618,(float)0.406737,(float)0.390731,(float)0.374607,(float)0.358368,(float)0.342020,(float)0.325568,(float)0.309017,(float)0.292372,(float)0.275637,(float)0.258819,(float)0.241922,(float)0.224951,(float)0.207912,(float)0.190809,(float)0.173648,(float)0.156434,(float)0.139173,(float)0.121869,(float)0.104528,(float)0.087156,(float)0.069756,(float)0.052336,(float)0.034899,(float)0.017452};
const float Accumulator::coss[180] = {(float)1.0,(float)0.999848,(float)0.999391,(float)0.998630,(float)0.997564,(float)0.996195,(float)0.994522,(float)0.992546,(float)0.990268,(float)0.987688,(float)0.984808,(float)0.981627,(float)0.978148,(float)0.974370,(float)0.970296,(float)0.965926,(float)0.961262,(float)0.956305,(float)0.951057,(float)0.945519,(float)0.939693,(float)0.933580,(float)0.927184,(float)0.920505,(float)0.913545,(float)0.906308,(float)0.898794,(float)0.891007,(float)0.882948,(float)0.874620,(float)0.866025,(float)0.857167,(float)0.848048,(float)0.838671,(float)0.829038,(float)0.819152,(float)0.809017,(float)0.798636,(float)0.788011,(float)0.777146,(float)0.766044,(float)0.754710,(float)0.743145,(float)0.731354,(float)0.719340,(float)0.707107,(float)0.694658,(float)0.681998,(float)0.669131,(float)0.656059,(float)0.642788,(float)0.629320,(float)0.615662,(float)0.601815,(float)0.587785,(float)0.573576,(float)0.559193,(float)0.544639,(float)0.529919,(float)0.515038,(float)0.500000,(float)0.484810,(float)0.469472,(float)0.453991,(float)0.438371,(float)0.422618,(float)0.406737,(float)0.390731,(float)0.374607,(float)0.358368,(float)0.342020,(float)0.325568,(float)0.309017,(float)0.292372,(float)0.275637,(float)0.258819,(float)0.241922,(float)0.224951,(float)0.207912,(float)0.190809,(float)0.173648,(float)0.156434,(float)0.139173,(float)0.121869,(float)0.104528,(float)0.087156,(float)0.069757,(float)0.052336,(float)0.034899,(float)0.017452,(float)0.0,(float)-0.017452,(float)-0.034899,(float)-0.052336,(float)-0.069756,(float)-0.087156,(float)-0.104529,(float)-0.121869,(float)-0.139173,(float)-0.156434,(float)-0.173648,(float)-0.190809,(float)-0.207912,(float)-0.224951,(float)-0.241922,(float)-0.258819,(float)-0.275637,(float)-0.292372,(float)-0.309017,(float)-0.325568,(float)-0.342020,(float)-0.358368,(float)-0.374607,(float)-0.390731,(float)-0.406737,(float)-0.422618,(float)-0.438371,(float)-0.453990,(float)-0.469472,(float)-0.484810,(float)-0.500000,(float)-0.515038,(float)-0.529919,(float)-0.544639,(float)-0.559193,(float)-0.573576,(float)-0.587785,(float)-0.601815,(float)-0.615661,(float)-0.629320,(float)-0.642788,(float)-0.656059,(float)-0.669131,(float)-0.681998,(float)-0.694658,(float)-0.707107,(float)-0.719340,(float)-0.731354,(float)-0.743145,(float)-0.754710,(float)-0.766044,(float)-0.777146,(float)-0.788011,(float)-0.798635,(float)-0.809017,(float)-0.819152,(float)-0.829037,(float)-0.838671,(float)-0.848048,(float)-0.857167,(float)-0.866025,(float)-0.874620,(float)-0.882948,(float)-0.891006,(float)-0.898794,(float)-0.906308,(float)-0.913545,(float)-0.920505,(float)-0.927184,(float)-0.933580,(float)-0.939693,(float)-0.945519,(float)-0.951056,(float)-0.956305,(float)-0.961262,(float)-0.965926,(float)-0.970296,(float)-0.974370,(float)-0.978148,(float)-0.981627,(float)-0.984808,(float)-0.987688,(float)-0.990268,(float)-0.992546,(float)-0.994522,(float)-0.996195,(float)-0.997564,(float)-0.998630,(float)-0.999391,(float)-0.999848};

// bitmap with numbers to display
const BYTE SudBitmap::m_font[9][19][16] = {
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255, 0 , 0 ,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 ,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 ,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,
255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,
255,255, 0 , 0 , 0 ,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255, 0 , 0 ,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255, 0 , 0 ,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255, 0 , 0 ,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 ,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255, 0 , 0 , 0 ,255, 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,255,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,255,
255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255, 0 , 0 , 0 , 0 ,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,
255,255, 0 , 0 , 0 ,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255, 0 , 0 , 0 ,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255, 0 , 0 , 0 , 0 ,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255, 0 , 0 , 0 , 0 , 0 ,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255, 0 , 0 , 0 ,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255, 0 , 0 , 0 ,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255, 0 , 0 , 0 , 0 ,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255, 0 ,255,255,255, 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255,255,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255,255, 0 , 0 , 0 , 0 ,255, 0 , 0 , 0 ,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 ,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 ,255,255,255,255, 0 , 0 , 0 ,255,255,255,
255,255,255, 0 , 0 , 0 , 0 ,255,255, 0 , 0 , 0 ,255,255,255,255,
255,255,255,255, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,255,255,255,255,
255,255,255,255,255,255, 0 , 0 , 0 , 0 , 0 ,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};

SudResultVoter SudBitmap::m_resultVoter;	// definition of static variable
Solution SudBitmap::m_solution;				// definition of static variable

// constructor
SudBitmap::SudBitmap(DWORD bitsResolution, LPVOID buf, CSize sz, const DisplaySud* display)
{
	m_BitsResolution = bitsResolution;
	m_buf = buf;
	height = sz.cy;
	width  = sz.cx;
	m_rhos = (int)((width + height) * SQRT2)/2;

	m_pMono = new BYTE[height * width];
	FillMemory(m_pMono, height * width, 255);

	this->display = display;

	ZeroMemory(m_grid, sizeof(POINT)*10*10);
}

SudBitmap::~SudBitmap(void)
{
	delete [] m_pMono;
}

//////////////////////////////////////////////////////////////////////////
// Function:	Monochrome()
// Purpose:		Converts the color image to bitonal image.
//				Uses "Mean Adaptive Thresholding" method. http://homepages.inf.ed.ac.uk/rbf/HIPR2/adpthrsh.htm
//				Optimized algorithm with the use of integral image. http://people.scs.carleton.ca/~roth/iit-publications-iti/docs/gerh-50002.pdf.
// Parameters:
/////////////////////////////////////////////////////////////////////////
void SudBitmap::Monochrome()
{
	int x, y, yOffset;
	unsigned int sum;
	unsigned int* pIntegral = new unsigned int[height * width]; 	// integral image (sums of the original image)
	BYTE mean, byte;
	static const BYTE C=2;		// it is better 1 in some cases
	static const int RECT=5;	// TODO dynamically adapt value from 3 to 8 based on image size
	static const unsigned int count = (RECT*2+1)*(RECT*2+1);

	switch(m_BitsResolution)
	{	case 8:
			break;
		case 16:
			break;
		case 24:
			{	PIX24 pix;
				LPPIX24 buf = (LPPIX24)m_buf;

				// fill integral image. First step is to copy orig.image -> integral image
				for (x=width*height-1; x>=0; x--)
				{	pix = buf[x];
					pIntegral[x] = (((pix.red * 77) + (pix.green * 150) + (pix.blue * 28)) >> 8) & 255;		// 100% Intesity = 30% Red + 59% Green + 11% Blue
				}
				SumIntegralImage(pIntegral);

				// "Mean Adaptive Thresholding" method
				for (y=RECT+1; y<height-RECT; y++) 
				{	yOffset = y * width;
					for (x=RECT+1; x<width-RECT; x++)
					{	// sum the surounding pixels RECTxRECT around x,y
						sum = pIntegral[(y+RECT)*width+x+RECT] - pIntegral[(y-RECT-1)*width+x+RECT] - pIntegral[(y+RECT)*width+x-RECT-1] + pIntegral[(y-RECT-1)*width+x-RECT-1];
						mean = (BYTE)(sum/count);		// calculate mean value of RECTxRECT neighbours pixels

						pix = buf[yOffset + x];
						byte = (BYTE)((((pix.red * 77) + (pix.green * 150) + (pix.blue * 28)) >> 8) & 255);		// 100% Intesity = 30% Red + 59% Green + 11% Blue

						if (byte < mean-C)		// original pixel intensity above or below the local threshold?
							m_pMono[y*width + x] = 0;					// black
						//else
						//	m_pMono[y*width + x] = 255;					// white
					}
				}

			}
			break;
		case 32:
			{	COLORREF pix;
				LPCOLORREF buf = (LPCOLORREF)m_buf;
				
				// fill integral image. First step is to copy orig.image -> integral image
				for (x=width*height-1; x>=0; x--)
				{	pix = buf[x];
					pIntegral[x] = (((GetRValue(pix) * 77) + (GetGValue(pix) * 150) + (GetBValue(pix) * 28)) >> 8) & 255;		// 100% Intesity = 30% Red + 59% Green + 11% Blue
				}
				SumIntegralImage(pIntegral);

				// "Mean Adaptive Thresholding" method
				for (y=RECT+1; y<height-RECT; y++) 
				{	yOffset = y * width;
					for (x=RECT+1; x<width-RECT; x++)
					{	// sum the surounding pixels RECTxRECT around x,y. By using integral image this goes fast.
						sum = pIntegral[(y+RECT)*width+x+RECT] - pIntegral[(y-RECT-1)*width+x+RECT] - pIntegral[(y+RECT)*width+x-RECT-1] + pIntegral[(y-RECT-1)*width+x-RECT-1];
						mean = (BYTE)(sum/count);		// calculate mean value of RECTxRECT neighbours pixels

						pix = buf[yOffset + x];
						byte = (((GetRValue(pix) * 77) + (GetGValue(pix) * 150) + (GetBValue(pix) * 28)) >> 8) & 255;		// 100% Intesity = 30% Red + 59% Green + 11% Blue
						if (byte < mean-C)		// original pixel intensity above or below local threshold?
							m_pMono[y*width + x] = 0;					// black
						//else
						//	m_pMono[y*width + x] = 255;					// white
					}
				}
			}
			break;
		default:	//unknown format
			break;
	}
	delete [] pIntegral;

	
	// show monochrome image
	if (display->black || display->white)
	{	for (y=0; y<height; y++)
		{	yOffset = y * width;
			for (x=0; x<width; x++)
			{	if (m_pMono[yOffset + x] == 0)
				{	if (display->black)
						SetPixel(x, y, RGB(0, 0, 0));
				}
				else
				{	if (display->white)
						SetPixel(x, y, RGB(255, 255, 255));
	}	}	}	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	SumIntegralImage()
// Purpose:		Builds the integral image.
// Parameters:	pIntegral - pointer to the integral image to build. Initially must be filled with the copy of original image. 
/////////////////////////////////////////////////////////////////////////
void SudBitmap::SumIntegralImage(unsigned int* pIntegral)
{
	int x, y;
	unsigned int *pIntegralA, *pIntegralB, *pIntegralC, *pIntegralD;	// use pointers for speed

	// sum the first column
	pIntegralA = pIntegral;
	pIntegralB = pIntegral + width;
	for (y=1; y<height; y++)
	{	*pIntegralB += *pIntegralA;
		pIntegralA += width;
		pIntegralB += width;
	}

	// sum the first row
	pIntegralA = pIntegral;
	pIntegralB = pIntegral + 1;
	for (x=1; x<width; x++)
	{	*pIntegralB += *pIntegralA;
		pIntegralA++;
		pIntegralB++;
	}

	// sum all other pixels
	for (y=1; y<height; y++)
	{	pIntegralA = pIntegral + y*width + 1;
		pIntegralB = pIntegral + (y-1)*width + 1;
		pIntegralC = pIntegral + y*width;
		pIntegralD = pIntegral + (y-1)*width;
		for (x=1; x<width; x++)
		{	*pIntegralA += *pIntegralB + *pIntegralC - *pIntegralD;
			pIntegralA++;
			pIntegralB++;
			pIntegralC++;
			pIntegralD++;
	}	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	HoughTransformCenter()
// Purpose:		Detects the image rotation angle by detecting the strongest line around the centre of the image.
//				http://en.wikipedia.org/wiki/Hough_transform
// Parameters:	pStrongestLine - pointer to the resulting detected strongest line.
// Returns:		TRUE if strongest line detected, otherwise FALSE. 
/////////////////////////////////////////////////////////////////////////
BOOL SudBitmap::HoughTransformCenter(Bin* pStrongestLine)
{
	int theta, rho, x, y, yOffset, strip;
	Accumulator accumulator(width, height);

	if (width > height)
		strip = height/7;
	else
		strip = width/7;

	// fill accumulator from image
	// we will sweep the small portion at the the image center (not entire image) for better performance
	for (y = 3*strip; y < 4*strip; y++)	//image center strip, which is 1/7 of the height
	{	yOffset = y * width;
		for (x = width/3; x < width - width/3; x+=3)	// 1/3 of the width; skip every 3 pixels
		{	if (display->rotation)
			{	COLORREF color = GetPixel(x, y);
				SetPixel(x, y, RGB(GetRValue(color), GetGValue(color), GetBValue(color)/3));
			}

			if (m_pMono[yOffset + x] == 0)		// if black pixel
			{
				accumulator.Add(x, y, 60, 120);	// looking for horizontal lines around 90(+-30) degrees
	}	}	}

	// search for strongest line in the accumulator
	pStrongestLine->vote = -1;
	for (rho = accumulator.rhosLow; rho < accumulator.rhosHigh; rho++)
	{	yOffset = rho * accumulator.thetas;
		for (theta = 60; theta <= 120; theta++)
		{	if (accumulator.bins[yOffset + theta] > pStrongestLine->vote)
			{	pStrongestLine->vote  = accumulator.bins[yOffset + theta];
				pStrongestLine->theta = theta;
				pStrongestLine->rho   = rho;
			}
		}
	}

	if (pStrongestLine->vote == -1)
		return FALSE;

	// Display the accumulator (of the hough transformation) to the screen
	// Nice to visualize what the accumulator is. Uncomment next block to visualize it.
	/*BYTE byte;
	float factor = height / ((width + height) * SQRT2);
	for (rho = accumulator.rhosLow; rho < accumulator.rhos; rho++)
	{	for (theta = 0; theta < accumulator.thetas; theta++)
		{	{	byte = (BYTE)(accumulator.bins[rho*accumulator.thetas + theta]) * 5;
				y = (int)(rho * factor);
				SetPixel(theta, y, RGB(byte,byte,byte));
			}
		}
	}*/

	if (display->rotation)
	{	for (int x = -m_rhos; x < m_rhos; x++)
		{	int y = (int)((x * cos((180-pStrongestLine->theta)*DEG2RAD) + pStrongestLine->rho - m_rhos) / sin((180-pStrongestLine->theta)*DEG2RAD));
			if (x>0 && x<width && y>0 && y<height)
				SetPixel(x, y, RGB(70,170,0));
	}	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Function:	HoughTransform()
// Purpose:		Detects the strongest line in the specific area.
//				http://en.wikipedia.org/wiki/Hough_transform
// Parameters:	inputLine - reference line (rho value) from which to detetect the strongest line.
//				offset - parallel distance from inputLine where to search for the strongest line. Could be negative or positive, depending on the direction from the inputLine.
//				loLimit - limit the line to search for the strongest line.
//				hiLimit - limit the line to search for the strongest line.
//				thetaOffset - limit the angle to search for the strongest line.
//				isVertical - are we looking for a vertical or horizontal line?
//				pStrongestLine - resulting strongest line.
// Returns:		TRUE if strongest line detected, otherwise FALSE.
/////////////////////////////////////////////////////////////////////////
BOOL SudBitmap::HoughTransform(Line inputLine, int offset, int loLimit, int hiLimit, int thetaOffset, BOOL isVertical, Line* pStrongestLine)
{
	int rho, x, y, yOffset, start, end, vote;
	BYTE byte;
	Accumulator accumulator(width, height);

	if (offset > 0)
	{	start = inputLine.rho;
		end = start + offset;
	}
	else
	{	end = inputLine.rho;
		start = end + offset;
	}
	float sina = sin((inputLine.theta)*DEG2RAD);
	float cosa = cos((inputLine.theta)*DEG2RAD);
	inputLine.theta = 180-inputLine.theta;		// correct for bin orientation

	if (isVertical)
	{	for (rho=start; rho<end; rho+=1)			// try 5 for speed
		{	for (y = loLimit; y < hiLimit; y+=2)	// try 4 for speed
			{	x = (int)((y * cosa + rho - m_rhos) / sina);
				if (x>0 && x<width && y>0 && y<height)
				{
					if (display->lineDetect)
						SetPixel(x, y, RGB(200, 0, 255));

					byte = m_pMono[y*width + x];
					if (byte == 0)		// if black pixel
					{
						accumulator.Add(y, x, inputLine.theta-thetaOffset, inputLine.theta+thetaOffset);	// sweep +- degrees
	}	}	}	}	}
	else
	{	for (rho=start; rho<end; rho+=1)			// try 5
		{	for (x = loLimit; x < hiLimit; x+=2)	// try 4
			{	y = (int)((x * cosa + rho - m_rhos) / sina);
				if (x>0 && x<width && y>0 && y<height)
				{
					if (display->lineDetect)
						SetPixel(x, y, RGB(200, 0, 255));

					byte = m_pMono[y*width + x];
					if (byte == 0)		// if black pixel
					{
						accumulator.Add(x, y, inputLine.theta-thetaOffset, inputLine.theta+thetaOffset);	// sweep +- degrees
	}	}	}	}	}
	

	// search for strongest line in the accumulator (biggest vote)
	vote = -1;
	for (rho = accumulator.rhosLow; rho < accumulator.rhosHigh; rho++)
	{	yOffset = rho * accumulator.thetas;
		for (x = inputLine.theta-thetaOffset; x <= inputLine.theta+thetaOffset; x++)
		{	if (accumulator.bins[yOffset + x] > vote)
			{	vote = accumulator.bins[yOffset + x];
				pStrongestLine->theta = x;
				pStrongestLine->rho   = rho;
			}
		}
	}
	if (vote == -1)
		return FALSE;

	// correct the angles for the outside world
	pStrongestLine->theta = 180 - pStrongestLine->theta;
	
	// Display the accumulator (of the hough transformation) to the screen.
	// Nice to visualize what the accumulator is. Uncomment next block to visualize it.
	/*//BYTE byte;
	float factor = height / ((width + height) * SQRT2);
	for (rho = accumulator.rhosLow; rho < accumulator.rhos; rho++)
	{	for (int theta = 0; theta < accumulator.thetas; theta++)
		{	{	byte = (BYTE)(accumulator.bins[rho*accumulator.thetas + theta]) * 5;
				y = (int)(rho * factor);
				SetPixel(theta, y, RGB(byte,byte,byte));
			}
		}
	}*/
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Function:	DetectRect()
// Purpose:		Step 1. Detects the rectangle around the sudoku grid
//				Step 2. Detects the internal grid lines
//				Step 3. Detects the intersection points of grid lines. Fills them in m_grid, which is the goal of this method.
// Parameters:	theta - rotation angle of the image. 
// Returns:		TRUE if a valid rectangle is detected, otherwise FALSE. 
/////////////////////////////////////////////////////////////////////////
BOOL SudBitmap::DetectRect(int theta)
{
	int x, y, xx, yy, rho, maxValue, start, end;
	unsigned int sum;
	float sina, cosa;
	Line left, top, right, bottom;			// lines forming the white rectangle around sudoku grid - this is what we're looking for the step 1
	int* whiteV = new int[width];			// vertical uninterrupted white pixel counter
	ZeroMemory(whiteV, sizeof(int)*width);
	int* whiteH = new int[height];			// horizontal uninterrupted white pixel counter
	ZeroMemory(whiteH, sizeof(int)*height);

	static const int RECT = 1;
	static const unsigned int count = (RECT*2+1)*(RECT*2+1);

	top.theta = bottom.theta = 180-theta;
	left.theta = right.theta = theta;

	////////////////////////////////////
	// HORIZONTAL white strips detection
	sina = sin(top.theta*DEG2RAD);
	cosa = cos(top.theta*DEG2RAD);
	start = m_rhos;
	end = m_rhos + (int)(height * sina);
	for (rho=start; rho<end; rho+=3)						// todo: try with 5 for performance
	{	// sweep down->up with parallel lines and count white pixels. Our goal is to find longest uninterrupted white strip line. Strip consists of rectangles 3x3 pixels.
		int whiteLen = 0;
		int longestwhiteLen = 0;
		for (x = RECT; x < width-RECT; x+=4)
		{	y = (int)((x * cosa + rho - m_rhos) / sina);
			if (y>=RECT && y<height-RECT)
			{
				sum = 0;
				for (yy=y-RECT; yy<=y+RECT; yy++) 
				{	int yyOffset = yy * width;
					for (xx=x-RECT; xx<=x+RECT; xx++)
					{	{	if (m_pMono[yyOffset + xx] == 0)
								sum++;
							//SetPixel(xx, y, RGB(100,100,50));		// display the progress on screen
						}
					}
				}
				if (count/3 > sum)	// white
					whiteLen++;
				else				// black
				{	if (longestwhiteLen < whiteLen)
						longestwhiteLen = whiteLen;
					whiteLen = 0;
				}
			}
			if (longestwhiteLen < whiteLen)
				longestwhiteLen = whiteLen;
		}
		whiteH[rho-start] = longestwhiteLen;
	}
	// Display whiteH array to screen
	if (display->whiteCandidates)
		for (y = 0; y < height; y++)
		{	for (x=0; x<whiteH[y]; x++)
			{	SetPixel(x, y, RGB(100,100,150));
			}
		}

	////////////////////////////////////////
	// detect 2 peaks of white horizontal strips
	maxValue = 0;
	for (y = 1+height/2; y > 0; y--)
		if (maxValue < whiteH[y])
		{	maxValue = whiteH[y];
			bottom.rho = y+start;
		}
	maxValue = 0;
	for (y = height/2; y < height-1; y++)
		if (maxValue < whiteH[y])
		{	maxValue = whiteH[y];
			top.rho = y+start;
		}


	//////////////////////////////////
	// VERTICAL white strips detection
	sina = sin(left.theta*DEG2RAD);
	cosa = cos(left.theta*DEG2RAD);
	start  = m_rhos;
	end = (int)(width*sina) + m_rhos;

	for (rho=start; rho<end; rho+=3)			// todo: try 5 for performance
	{	int whiteLen = 0;
		int longestwhiteLen = 0;

		int Ydown = (int)(((rho-start) * cos(bottom.theta*DEG2RAD) + bottom.rho - m_rhos) / sin(bottom.theta*DEG2RAD));
		int Yup = Ydown + top.rho - bottom.rho;

		for (y = Ydown; y < Yup; y+=4)
		{	x = (int)((y * cosa + rho - m_rhos) / sina);
			if (x>=RECT && x<width-RECT && y>=RECT && y<height-RECT)
			{
				sum = 0;
				for (yy=y-RECT; yy<=y+RECT; yy++)
				{	int yyOffset = yy * width;
					for (xx=x-RECT; xx<=x+RECT; xx++)
					{	{	if (m_pMono[yyOffset + xx] == 0)
								sum++;

							if (display->whiteCandidates)
								SetPixel(x, yy, RGB(100,100,50));		// display tracing to screen
						}
					}
				}
				if (count/3 > sum)	// white
					whiteLen++;
				else				// black
				{	if (longestwhiteLen < whiteLen)
						longestwhiteLen = whiteLen;
					whiteLen = 0;
				}
			}
			if (longestwhiteLen < whiteLen)
				longestwhiteLen = whiteLen;
		}
		whiteV[rho-start] = longestwhiteLen;
	}
	// Display whiteV array to screen
	if (display->whiteCandidates)
		for (x = 0; x < width; x++)
		{	for (y=0; y<whiteV[x]; y++)
			{	SetPixel(x, y, RGB(100,100,150));
			}
		}

	// detect 2 peaks of white vertical strips
	maxValue = 0;
	for (x = 1+width/2; x > 0; x--)
		if (maxValue < whiteV[x])
		{	maxValue = whiteV[x];
			left.rho = x+start;
		}
	maxValue = 0;
	for (x = width/2; x < width-1; x++)
		if (maxValue < whiteV[x])
		{	maxValue = whiteV[x];
			right.rho = x+start;
		}

	delete [] whiteV;
	delete [] whiteH;


	///////////////////////////////
	// calculate line intersections points of white rectangle
	POINT lt, rt, lb, rb;	// intersection points to calculate
	float c1, c2, t1, t2;
	c1 = (left.rho - m_rhos)/sin(left.theta*DEG2RAD);
	c2 = (top.rho - m_rhos)/sin(top.theta*DEG2RAD);
	t1 = cos(left.theta*DEG2RAD)/sin(left.theta*DEG2RAD);
	t2 = cos(top.theta*DEG2RAD)/sin(top.theta*DEG2RAD);
	lt.x = (int)((c2*t1 + c1) / (1 - t1*t2));
	lt.y = (int)(lt.x*t2 + c2);
	
	//c1 = (left.rho - m_rhos)/sin(left.theta*DEG2RAD);
	c2 = (bottom.rho - m_rhos)/sin(bottom.theta*DEG2RAD);
	//t1 = cos(left.theta*DEG2RAD)/sin(left.theta*DEG2RAD);
	t2 = cos(bottom.theta*DEG2RAD)/sin(bottom.theta*DEG2RAD);
	lb.x = (int)((c2*t1 + c1) / (1 - t1*t2));
	lb.y = (int)(lb.x*t2 + c2);

	c1 = (right.rho - m_rhos)/sin(right.theta*DEG2RAD);
	//c2 = (bottom.rho - m_rhos)/sin(bottom.theta*DEG2RAD);
	t1 = cos(right.theta*DEG2RAD)/sin(right.theta*DEG2RAD);
	//t2 = cos(bottom.theta*DEG2RAD)/sin(bottom.theta*DEG2RAD);
	rb.x = (int)((c2*t1 + c1) / (1 - t1*t2));
	rb.y = (int)(rb.x*t2 + c2);
	
	//c1 = (right.rho - m_rhos)/sin(right.theta*DEG2RAD);
	c2 = (top.rho - m_rhos)/sin(top.theta*DEG2RAD);
	//t1 = cos(right.theta*DEG2RAD)/sin(right.theta*DEG2RAD);
	t2 = cos(top.theta*DEG2RAD)/sin(top.theta*DEG2RAD);
	rt.x = (int)((c2*t1 + c1) / (1 - t1*t2));
	rt.y = (int)(rt.x*t2 + c2);

	if (display->whiteArea)
		DrawRect(left, top, right, bottom, lt, rt, lb, rb, RGB(170,0,230));

	if ( abs((lt.y-lb.y) - (rt.x-lt.x)) > max(lt.y-lb.y, rt.x-lt.x)/3 )	// check if result is not a square, even an ugly one.
	{	m_resultVoter.Reset();
		return FALSE;
	}


	///////////////////////////////////////////////////
	// detect the black lines closest to the white line. First the left and right lines. Then based on the distance between them, detect the third and the sixth line, which sometimes are bolded compared to other lines. To be image skew proof.
	// The goal is to find m_grid[][] points.
	Line hor[10], ver[10];
	Line temp;
	int offb = lb.y + (lt.y-lb.y)/9;
	int offt = lt.y - (lt.y-lb.y)/9;

	// leftmost line
	HoughTransform(left, (right.rho-left.rho)/9, offb, offt, 4, TRUE, &ver[0]);
	// rightmost line
	offb = rb.y + (rt.y-rb.y)/9;
	offt = rt.y - (rt.y-rb.y)/9;
	HoughTransform(right, (left.rho-right.rho)/9, offb, offt, 4, TRUE, &ver[9]);
	// third line
	temp.theta = ver[0].theta + (int)((ver[9].theta-ver[0].theta)*3/9.0);
	temp.rho   = ver[0].rho + (int)((ver[9].rho-ver[0].rho)*3/9.0 - (ver[9].rho-ver[0].rho)/26.0);
	offb = lb.y + (rb.y-lb.y)/3 + (lt.y-lb.y)/7;
	offt = lt.y + (rt.y-lt.y)/3 - (lt.y-lb.y)/7;
	HoughTransform(temp, (int)((ver[9].rho-ver[0].rho)/13.0), offb, offt, 2, TRUE, &ver[3]);
	// sixth line
	temp.theta = ver[0].theta + (int)((ver[9].theta-ver[0].theta)*6/9.0);
	temp.rho   = ver[0].rho + (int)((ver[9].rho-ver[0].rho)*6/9.0 - (ver[9].rho-ver[0].rho)/26.0);
	offb = lb.y + 2*(rb.y-lb.y)/3 + (lt.y-lb.y)/7;
	offt = lt.y + 2*(rt.y-lt.y)/3 - (lt.y-lb.y)/7;
	HoughTransform(temp, (int)((ver[9].rho-ver[0].rho)/13.0), offb, offt, 2, TRUE, &ver[6]);

	// topmost line
	int offl = lt.x + (rt.x-lt.x)/9;
	int offr = rt.x - (rt.x-lt.x)/9;
	HoughTransform(top, (bottom.rho-top.rho)/11, offl, offr, 4, FALSE, &hor[9]);
	// bottom line
	offl = lb.x + (rb.x-lb.x)/9;
	offr = rb.x - (rb.x-lb.x)/9;
	HoughTransform(bottom, (top.rho-bottom.rho)/9, offl, offr, 4, FALSE, &hor[0]);
	// third line
	temp.theta = hor[0].theta + (int)((hor[9].theta-hor[0].theta)*3/9.0);
	temp.rho   = hor[0].rho + (int)((hor[9].rho-hor[0].rho)*3/9.0 - (hor[9].rho-hor[0].rho)/25.0);
	offl = lb.x + (lt.x-lb.x)/3 + (rb.x-lb.x)/7;
	offr = rb.x + (rt.x-rb.x)/3 - (rb.x-lb.x)/7;
	HoughTransform(temp, (int)((hor[9].rho-hor[0].rho)/12.0), offl, offr, 2, FALSE, &hor[3]);
	// sixth line
	temp.theta = hor[0].theta + (int)((hor[9].theta-hor[0].theta)*6/9.0);
	temp.rho   = hor[0].rho + (int)((hor[9].rho-hor[0].rho)*6/9.0 - (hor[9].rho-hor[0].rho)/25.0);
	offl = lb.x + 2*(lt.x-lb.x)/3 + (rb.x-lb.x)/7;
	offr = rb.x + 3*(rt.x-rb.x)/3 - (rb.x-lb.x)/7;
	HoughTransform(temp, (int)((hor[9].rho-hor[0].rho)/12.0), offl, offr, 2, FALSE, &hor[6]);
	

	//////////////////////////////////////////////////////////////////
	// calculate ver and hor line intersections of lines 0, 3, 6 and 9
	for (y=0; y<10; y+=3)
	{	c1 = (ver[y].rho - m_rhos)/sin(ver[y].theta*DEG2RAD);
		t1 = cos(ver[y].theta*DEG2RAD)/sin(ver[y].theta*DEG2RAD);
		for (x=0; x<10; x+=3)
		{	c2 = (hor[x].rho - m_rhos)/sin(hor[x].theta*DEG2RAD);
			t2 = cos(hor[x].theta*DEG2RAD)/sin(hor[x].theta*DEG2RAD);
			m_grid[x][y].x = (int)((c2*t1 + c1) / (1 - t1*t2));
			m_grid[x][y].y = (int)(m_grid[x][y].x * t2 + c2);
	}	}
	// interpolate remaining intersection points
	for (y=0; y<10; y+=3)
	{	m_grid[y][1].x = m_grid[y][0].x + (m_grid[y][3].x - m_grid[y][0].x)/3;
		m_grid[y][2].x = m_grid[y][3].x - (m_grid[y][3].x - m_grid[y][0].x)/3;
		m_grid[y][4].x = m_grid[y][3].x + (m_grid[y][6].x - m_grid[y][3].x)/3;
		m_grid[y][5].x = m_grid[y][6].x - (m_grid[y][6].x - m_grid[y][3].x)/3;
		m_grid[y][7].x = m_grid[y][6].x + (m_grid[y][9].x - m_grid[y][6].x)/3;
		m_grid[y][8].x = m_grid[y][9].x - (m_grid[y][9].x - m_grid[y][6].x)/3;
		m_grid[y][1].y = m_grid[y][0].y + (m_grid[y][3].y - m_grid[y][0].y)/3;
		m_grid[y][2].y = m_grid[y][3].y - (m_grid[y][3].y - m_grid[y][0].y)/3;
		m_grid[y][4].y = m_grid[y][3].y + (m_grid[y][6].y - m_grid[y][3].y)/3;
		m_grid[y][5].y = m_grid[y][6].y - (m_grid[y][6].y - m_grid[y][3].y)/3;
		m_grid[y][7].y = m_grid[y][6].y + (m_grid[y][9].y - m_grid[y][6].y)/3;
		m_grid[y][8].y = m_grid[y][9].y - (m_grid[y][9].y - m_grid[y][6].y)/3;
	}
	for (x=0; x<10; x+=3)
	{	m_grid[1][x].x = m_grid[0][x].x + (m_grid[3][x].x - m_grid[0][x].x)/3;
		m_grid[2][x].x = m_grid[3][x].x - (m_grid[3][x].x - m_grid[0][x].x)/3;
		m_grid[4][x].x = m_grid[3][x].x + (m_grid[6][x].x - m_grid[3][x].x)/3;
		m_grid[5][x].x = m_grid[6][x].x - (m_grid[6][x].x - m_grid[3][x].x)/3;
		m_grid[7][x].x = m_grid[6][x].x + (m_grid[9][x].x - m_grid[6][x].x)/3;
		m_grid[8][x].x = m_grid[9][x].x - (m_grid[9][x].x - m_grid[6][x].x)/3;
		m_grid[1][x].y = m_grid[0][x].y + (m_grid[3][x].y - m_grid[0][x].y)/3;
		m_grid[2][x].y = m_grid[3][x].y - (m_grid[3][x].y - m_grid[0][x].y)/3;
		m_grid[4][x].y = m_grid[3][x].y + (m_grid[6][x].y - m_grid[3][x].y)/3;
		m_grid[5][x].y = m_grid[6][x].y - (m_grid[6][x].y - m_grid[3][x].y)/3;
		m_grid[7][x].y = m_grid[6][x].y + (m_grid[9][x].y - m_grid[6][x].y)/3;
		m_grid[8][x].y = m_grid[9][x].y - (m_grid[9][x].y - m_grid[6][x].y)/3;
	}
	for (int a=1; a<9; a++)
	{	if (a!=3 && a!=6)  
		{	for (int b=1; b<9; b++)
			{	if      (b==1)
				{	m_grid[a][b].x = m_grid[a][0].x + (m_grid[a][3].x - m_grid[a][0].x)/3;
					m_grid[b][a].y = m_grid[0][a].y + (m_grid[3][a].y - m_grid[0][a].y)/3;
				} else if (b==2)	
				{	m_grid[a][b].x = m_grid[a][3].x - (m_grid[a][3].x - m_grid[a][0].x)/3;
					m_grid[b][a].y = m_grid[3][a].y - (m_grid[3][a].y - m_grid[0][a].y)/3;
				} else if (b==4)	
				{	m_grid[a][b].x = m_grid[a][3].x + (m_grid[a][6].x - m_grid[a][3].x)/3;
					m_grid[b][a].y = m_grid[3][a].y + (m_grid[6][a].y - m_grid[3][a].y)/3;
				} else if (b==5)	
				{	m_grid[a][b].x = m_grid[a][6].x - (m_grid[a][6].x - m_grid[a][3].x)/3;
					m_grid[b][a].y = m_grid[6][a].y - (m_grid[6][a].y - m_grid[3][a].y)/3;
				} else if (b==7)	
				{	m_grid[a][b].x = m_grid[a][6].x + (m_grid[a][9].x - m_grid[a][6].x)/3;
					m_grid[b][a].y = m_grid[6][a].y + (m_grid[9][a].y - m_grid[6][a].y)/3;
				} else if (b==8)	
				{	m_grid[a][b].x = m_grid[a][9].x - (m_grid[a][9].x - m_grid[a][6].x)/3;
					m_grid[b][a].y = m_grid[9][a].y - (m_grid[9][a].y - m_grid[6][a].y)/3;
	}	}	}	}

	// draw grid lines
	if (display->grid)
	{	for (int i=0; i<10; i++)
		{	for (x = m_grid[i][0].x; x < m_grid[i][9].x; x++)
			{	y = m_grid[i][0].y + ((x - m_grid[i][0].x) * (m_grid[i][9].y - m_grid[i][0].y)) / (m_grid[i][9].x - m_grid[i][0].x);
				if (x>0 && x<width && y>0 && y<height)
					SetPixel(x, y, RGB(255, 255, 0));
		}	}
		for (int i=0; i<10; i++)
		{	for (y = m_grid[0][i].y; y < m_grid[9][i].y; y++)
			{	x = m_grid[0][i].x + ((y - m_grid[0][i].y) * (m_grid[9][i].x - m_grid[0][i].x)) / (m_grid[9][i].y - m_grid[0][i].y);
				if (x>0 && x<width && y>0 && y<height)
					SetPixel(x, y, RGB(255, 255, 0));
	}	}	}

	// check boundaries
	if (m_grid[0][0].x < 0 || m_grid[0][0].y < 0 || m_grid[0][9].y < 0 || m_grid[0][9].x >= width || m_grid[9][0].x < 0 || m_grid[9][0].y >= height || m_grid[9][9].y >= height || m_grid[9][9].x >= width   || m_grid[3][3].y < 0 || m_grid[6][6].y < 0)
	{	m_resultVoter.Reset();
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Function:	DrawRect()
// Purpose:		Draws a rectangle on the output image.
// Parameters:	left, top, right, bottom - lines to draw
//				lt, rt, lb, rb - intersection points of the lines
//				color - color to draw with
// Returns:		
/////////////////////////////////////////////////////////////////////////
void SudBitmap::DrawRect(const Line& left, const Line& top, const Line& right, const Line& bottom, const POINT& lt, const POINT& rt, const POINT& lb, const POINT& rb, COLORREF color)
{
	int x, y;

	// left line
	for (y = lb.y; y < lt.y; y++)
	{	x = (int)(y*cos(left.theta*DEG2RAD)/sin(left.theta*DEG2RAD) + (left.rho - m_rhos)/sin(left.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
			SetPixel(x, y, color);
	}

	// bottom line
	for (x = lb.x; x < rb.x; x++)
	{	y = (int)(x*cos(bottom.theta*DEG2RAD)/sin(bottom.theta*DEG2RAD) + (bottom.rho - m_rhos)/sin(bottom.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
			SetPixel(x, y, color);
	}

	// right line
	for (y = rb.y; y < rt.y; y++)
	{	x = (int)(y*cos(right.theta*DEG2RAD)/sin(right.theta*DEG2RAD) + (right.rho - m_rhos)/sin(right.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
			SetPixel(x, y, color);
	}
	// top line
	for (x = lt.x; x < rt.x; x++)
	{	y = (int)(x*cos(top.theta*DEG2RAD)/sin(top.theta*DEG2RAD) + (top.rho - m_rhos)/sin(top.theta*DEG2RAD));
		if (x>0 && x<width && y>0 && y<height)
			SetPixel(x, y, color);
	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	SetPixel()
// Purpose:		Draws a single pixel on the output image.
// Parameters:	x, y - pixel coordinates
//				color - color to draw with
// Returns:		
/////////////////////////////////////////////////////////////////////////
void SudBitmap::SetPixel(int x, int y, COLORREF color)
{
	switch(m_BitsResolution)
	{	case 8:	
			break;
		case 16:
			break;
		case 24:
			{	PIX24 pix;
				LPPIX24 buf = (LPPIX24)m_buf;
				pix.blue = GetBValue(color);
				pix.green = GetGValue(color);
				pix.red = GetRValue(color);
				buf[y * width + x] = pix;
			}
			break;
		case 32:
			((LPCOLORREF)m_buf)[y*width + x] = color;
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	GetPixel()
// Purpose:		Gets a single pixel color from the input image.
// Parameters:	x, y - pixel coordinates
// Returns:		pixel color
/////////////////////////////////////////////////////////////////////////
COLORREF SudBitmap::GetPixel(int x, int y)
{
	switch(m_BitsResolution)
	{	case 8:	
			break;
		case 16:
			break;
		case 24:
			{	PIX24 pix;
				LPPIX24 buf = (LPPIX24)m_buf;
				pix = buf[y * width + x];
				return RGB(pix.red, pix.green, pix.blue);
			}
			break;
		case 32:
			return ((LPCOLORREF)m_buf)[y*width + x];
			break;
		default:		//unknown input image format
			break;
	}
	return(0);
}

//////////////////////////////////////////////////////////////////////////
// Function:	GetPixelGray()
// Purpose:		Gets a single pixel intesity from the input image. Converts the color to gray. Blue part is darkest, red is lightest.
// Parameters:	x, y - pixel coordinates
// Returns:		pixel color intensity
/////////////////////////////////////////////////////////////////////////
BYTE SudBitmap::GetPixelGray(int x, int y)
{
	switch(m_BitsResolution)
	{	case 8:	
			break;
		case 16:
			break;
		case 24:
			{	PIX24 pix;
				LPPIX24 buf = (LPPIX24)m_buf;
				pix = buf[y * width + x];
				return (BYTE)((((pix.red * 77) + (pix.green * 150) + (pix.blue * 28)) >> 8) & 255);		// 100% Intesity = 30% Red + 59% Green + 11% Blue
			}
			break;
		case 32:
			{	COLORREF pix;
				pix = ((LPCOLORREF)m_buf)[y*width + x];
				return (((GetRValue(pix) * 77) + (GetGValue(pix) * 150) + (GetBValue(pix) * 28)) >> 8) & 255;		// 100% Intesity = 30% Red + 59% Green + 11% Blue
			}
			break;
		default:	//unknown input image format
			break;
	}
	return(0);
}

//////////////////////////////////////////////////////////////////////////
// Function:	DisplayCellNumber()
// Purpose:		Shows a number on the output image. Numbers are bitmaps stored in m_font.
// Parameters:	yBottom, xLeft - coordinates of the grid cell. Not pixel coordinates.
//				color - color to draw with
//				numberToDisplay - number to display
// Returns:		
/////////////////////////////////////////////////////////////////////////
void SudBitmap::DisplayCellNumber(int yBottom, int xLeft, COLORREF color, int numberToDisplay)
{
	// get pixel coordinates of the specific grid cell
	POINT lt = m_grid[yBottom+1][xLeft];
	POINT rt = m_grid[yBottom+1][xLeft+1];
	POINT lb = m_grid[yBottom]  [xLeft];
	POINT rb = m_grid[yBottom]  [xLeft+1];

	if (display->smallFont)
	{	rt.x = lt.x + (rt.x - lt.x)/2;
		rb.x = lb.x + (rb.x - lb.x)/2;
		lt.y = lb.y + (lt.y - lb.y)/2;
		rt.y = rb.y + (rt.y - rb.y)/2;
	}

	int cellWidth  = (rt.x-1) - (lt.x+2);
	int cellHeight = (lt.y-1) - (lb.y+2);

	if (cellWidth < 5 || cellHeight < 5)
		return;

	// draw the number on the screen pixel by pixel from m_font to screen
	int yindex = 0;
	for (int Y = lb.y+2; Y < lt.y-1; Y++)
	{	int xindex = 0;
		int X = lb.x + ((Y - lb.y) * (lt.x - lb.x)) / (lt.y - lb.y);
		for (int x=X+2; x < X+(rb.x-lb.x)-1; x++)
		{
			int y = Y + ((x - X) * (rb.y - lb.y)) / (rb.x - lb.x);
			if (y >= 0 && y < height)
			{	if (m_font[numberToDisplay-1][yindex * 19/cellHeight][xindex * 16/cellWidth] == 0)
					SetPixel(x, y, color);
				else if (display->white)
					SetPixel(x, y, RGB(255,255,255));
			}
			xindex++;
		}
		yindex++;
	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	OCR()
// Purpose:		Performs OCR on all sudoku cells. If needed, fixes the OCR values based on sudoku rules.
// Parameters:
// Returns:		TRUE if OCR values are valid, otherwise FALSE
/////////////////////////////////////////////////////////////////////////
BOOL SudBitmap::OCR()
{
	int x, y;

	for (y=0; y<9; y++)
		for (x=0; x<9; x++)
			OCRCell(y, x);
	
	m_cells.Fixit();

	// display OCR result on screen
	if (display->OCRresult)
		for (y=8; y>=0; y--)
			for (x=0; x<9; x++)
				if (m_cells.cell[y][x].value != 0)
					DisplayCellNumber(y, x, RGB(10, 50, 0), m_cells.cell[y][x].value);
	
	return m_cells.AreAllCellsValid();
}

//////////////////////////////////////////////////////////////////////////
// Function:	Solve()
// Purpose:		Tries to solve the sudoku.
// Parameters:
// Returns:		TRUE if puzzle solved, otherwise FALSE
/////////////////////////////////////////////////////////////////////////
BOOL SudBitmap::Solve()
{
	Solution tempResult;
	ZeroMemory(tempResult, sizeof(tempResult));
	BOOL ret = FALSE;

	if (m_cells.SolveMe() == TRUE)
	{
		m_cells.GetSolution(&tempResult);							// get recent solution
		memcpy(m_solution, tempResult, sizeof(tempResult));			// copy it to m_solution
		m_resultVoter.GetBestSolution(&m_solution);					// get new m_solution based on several recent solutions
		ret = TRUE;
	}

	if (display->tempSolution)
		for (int y=0; y<9; y++)
			for (int x=0; x<9; x++)
				DisplayCellNumber(y, x, RGB(70, 7, 7), tempResult[y][x]);

	return ret;
}

void SudBitmap::DisplaySolution(void)
{
	if (display->finalSolution)
		for (int y=0; y<9; y++)
			for (int x=0; x<9; x++)
				DisplayCellNumber(y, x, RGB(70, 7, 7), m_solution[y][x]);
}

//////////////////////////////////////////////////////////////////////////
// Function:	OCRCell()
// Purpose:		Recognize a cell. Cell value could be empty (which is 0) or number 1-9.
// Parameters:	yBottom, xLeft - coordinates of the grid cell. Not pixel coordinates.
// Returns:
/////////////////////////////////////////////////////////////////////////
void SudBitmap::OCRCell(int yBottom, int xLeft)
{
	int sum, yindex, xindex;
	BOOL bIsWhite, bIsFirstWhite;
	POINT& lt = m_grid[yBottom+1][xLeft];
	POINT& rt = m_grid[yBottom+1][xLeft+1];
	POINT& lb = m_grid[yBottom]  [xLeft];
	POINT& rb = m_grid[yBottom]  [xLeft+1];
	int cellWidth  = (rt.x-1) - (lt.x+2);
	int cellHeight = (lt.y-1) - (lb.y+2);
	POINT reslb, resrt;		// enclosing corners of the blob - it is the goal of this function to calculate it
	BYTE blob[100][100];	// bitmap of the cell - UNROTATED.  TODO: make sure we don't get cells bigger than 100x100.
	FillMemory(&blob[0][0], 100*100, 255);

	if (cellWidth < 5 || cellHeight < 9)
		return;

	// detect if cell is empty. How: sum the black pixels from the center of the cell. Sum must be greater than the threshold.
	sum = 0;
	for (int X = lt.x+2 + cellWidth/3; X < rt.x-1 - cellWidth/3; X++)
	{	int Y = lb.y + ((X - lt.x) * (rt.y - lt.y)) / (rt.x - lt.x);
		for (int y=Y+2 + cellHeight/3; y < Y+(lt.y-lb.y)-1 - cellHeight/4; y++)
		{
			int x = X - (lt.x-lb.x) - ((y - Y) * (lb.x - lt.x)) / (lt.y - lb.y);
			if (m_pMono[y*width + x] == 0)
				sum++;
			//else
			//	SetPixel(x, y, RGB(200,200,200));	// uncomment to display the testing area
		}
	}
	if (sum < (cellWidth*cellHeight)/31)			// threshold between empty cell and cell with a digit inside. Obtained empirically.
	{
		// uncomment next to display empty cells on the screen
		/*for (int X = lt.x+2; X < rt.x-1; X++)
		{	int Y = lb.y + ((X - lt.x) * (rt.y - lt.y)) / (rt.x - lt.x);
			for (int y=Y+2; y < Y+(lt.y-lb.y)-1; y++)
			{	
				int x = X - (lt.x-lb.x) - ((y - Y) * (lb.x - lt.x)) / (lt.y - lb.y);
				if (m_pMono[y*width + x] != 0)
					SetPixel(x, y, RGB(100,100,100));
			}
		}*/
		return;		// skip OCR if the cell is empty
	}
	
	// fill blob array. UNROTATE the image, so the blob is UNROTATED.
	yindex = 0;
	for (int Y = lb.y+2; Y < lt.y-1; Y++)
	{	xindex = 0;
		int X = lb.x + ((Y - lb.y) * (lt.x - lb.x)) / (lt.y - lb.y);
		for (int x=X+2; x < X+(rb.x-lb.x)-1; x++)
		{	int y = Y + ((x - X) * (rb.y - lb.y)) / (rb.x - lb.x);
			if (m_pMono[y*width + x] == 0)
				blob[yindex][xindex] = 0;
			xindex++;
		}
		yindex++;
	}

	// detect the blob boundaries

	// hor line scan
	// sweep bottom part (first 1/3 of the height). Start from the 1/3 of the height and go down to the bottom
	bIsFirstWhite = FALSE;
	reslb.y = cellHeight/3;
	for (int y = cellHeight/3; y >= 0; y--)
	{	bIsWhite = TRUE;
		for (int x=cellWidth/3; x < cellWidth-cellWidth/3; x++)	// left->right
		{	if (blob[y][x] == 0)	// if at least one black pixel found in that line, go to next line
			{	reslb.y = y;
				bIsFirstWhite = FALSE;
				bIsWhite = FALSE;
				break;
			}
		}

		if (bIsWhite == TRUE)
		{	if (bIsFirstWhite == FALSE)
				bIsFirstWhite = TRUE;
			else
				break;	// stop search after 2 consecutive white lines
		}
	}

	// sweep top part (3/3 of the height). Direction from center->top
	bIsFirstWhite = FALSE;
	resrt.y = (2*cellHeight)/3;
	for (int y = (2*cellHeight)/3; y < cellHeight; y++)
	{	bIsWhite = TRUE;
		for (int x=cellWidth/3; x < cellWidth-cellWidth/3; x++)	// left->right
		{	if (blob[y][x] == 0)	// if at least one black pixel found in that line, go to next line
			{	resrt.y = y;
				bIsFirstWhite = FALSE;
				bIsWhite = FALSE;
				break;
			}
		}

		if (bIsWhite == TRUE)
		{
			if (bIsFirstWhite == FALSE)
				bIsFirstWhite = TRUE;
			else
				break;	// stop search after 2 white lines
		}
	}


	// vertical line scan
	// sweep right part. From the center to the right
	resrt.x = cellWidth/2 + 1;
	for (int x = cellWidth/2 + 1; x < cellWidth; x++)
	{	bIsWhite = TRUE;
		for (int y=reslb.y+2; y < resrt.y-1; y++)	// down->up
		{	if (blob[y][x] == 0)	// if at least one black pixel found in that line, go to next line
			{	resrt.x = x;
				bIsWhite = FALSE;
				break;
			}
		}

		if (bIsWhite == TRUE)
		{	break;	// stop search after first white line
		}
	}

	// sweep left part. From the center to the left
	reslb.x = cellWidth/2;
	for (int x = cellWidth/2; x >= 0; x--)
	{	bIsWhite = TRUE;
		for (int y=reslb.y+2; y < resrt.y-1; y++)	// down->up
		{	if (blob[y][x] == 0)	// if at least one black pixel found in that line, go to next line
			{	reslb.x = x;
				bIsWhite = FALSE;
				break;
			}
		}

		if (bIsWhite == TRUE)
		{	break;	// stop search after first white line
		}
	}


	// OCR the blob !
	OCRDigit ocr;
	ocr.Classify(&blob[0][0], reslb, resrt, &(m_cells.cell[yBottom][xLeft]));


	/*// uncomment next block to save wrongly recognized blobs on disk (current directory) in BMP format
	// type your initial sudoku values in gold[][] array
	int gold[9][9] = {7,0,3,0,0,8,9,0,0,
					  0,6,2,0,0,0,4,0,0,
					  0,0,0,2,0,0,0,8,0,
					  0,0,0,0,0,0,2,9,0,
					  0,0,0,0,1,4,0,0,7,
					  9,7,0,3,0,0,6,0,0,
					  6,0,0,1,8,0,0,0,0,
					  0,0,0,2,0,0,0,0,5,
					  0,0,9,0,0,6,0,4,0};
	if (gold[8-yBottom][xLeft] != 0 && m_cells.cell[yBottom][xLeft].value != gold[8-yBottom][xLeft])
		SaveBitmapForTraining(&blob[0][0], reslb, resrt, yBottom, xLeft, gold[8-yBottom][xLeft]);
	*/


	/*// uncomment next block to display UNROTATED blobs with boundaries on screen
	for (yindex=0; yindex < cellHeight; yindex++)
		for (xindex=0; xindex < cellWidth; xindex++)
			SetPixel(xindex + xLeft*(cellWidth+5), yindex+(cellHeight+5)*yBottom, RGB(blob[yindex][xindex],blob[yindex][xindex],blob[yindex][xindex]));
	for (yindex=0; yindex < cellHeight; yindex++)
	{	//if (GetPixel(resrt.x + xLeft*(cellWidth+5), yindex+(cellHeight+5)*yBottom) == RGB(255,255,255))
			SetPixel(resrt.x + xLeft*(cellWidth+5), yindex+(cellHeight+5)*yBottom, RGB(100,100,100));
		//if (GetPixel(reslb.x + xLeft*(cellWidth+5), yindex+(cellHeight+5)*yBottom) == RGB(255,255,255))
			SetPixel(reslb.x + xLeft*(cellWidth+5), yindex+(cellHeight+5)*yBottom, RGB(100,100,100));
	}
	for (xindex=0; xindex < cellWidth; xindex++)
	{	//if (GetPixel(xindex + xLeft*(cellWidth+5), reslb.y+(cellHeight+5)*yBottom) == RGB(255,255,255))
			SetPixel(xindex + xLeft*(cellWidth+5), reslb.y+(cellHeight+5)*yBottom, RGB(100,100,100));
	//	if (GetPixel(xindex + xLeft*(cellWidth+5), resrt.y+(cellHeight+5)*yBottom) == RGB(255,255,255))
			SetPixel(xindex + xLeft*(cellWidth+5), resrt.y+(cellHeight+5)*yBottom, RGB(100,100,100));
	}
	*/
}

//////////////////////////////////////////////////////////////////////////
// Function:	SaveBitmapForTraining()
// Purpose:		Saves a part of the input image (blob) in a BMP file.
// Parameters:	blob - bitmap to save
//				lb, rt - coordinates in pixels of the blob. lb is leftbottom, rt is righttop, defining a rectangle around the blob.
//				yBottom, xLeft - coordinates of the grid cell. Not pixel coordinates.
//				value - digit value suposed to be represented by blob
// Returns:
/////////////////////////////////////////////////////////////////////////
void SudBitmap::SaveBitmapForTraining(const BYTE* blob, const POINT& lb, const POINT& rt, int yBottom, int xLeft, int value)
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	RGBQUAD pal;
	pal.rgbReserved = 0;
	int x, y;
	char filename[255];
	sprintf_s(filename, 255, "Train%d_%d_%d.bmp", value, yBottom, xLeft);		// build file name

	infoheader.biSize = sizeof(BITMAPINFOHEADER);
	infoheader.biWidth = rt.x - lb.x + 1;
	infoheader.biHeight = rt.y - lb.y + 1;
	infoheader.biPlanes = 1;
	infoheader.biBitCount = 8;
	infoheader.biCompression = BI_RGB;
	infoheader.biSizeImage = ((((infoheader.biWidth * infoheader.biBitCount) + 31) & ~31) >> 3) * infoheader.biHeight;
	infoheader.biXPelsPerMeter = 100;
	infoheader.biYPelsPerMeter = 100;
	infoheader.biClrUsed = 256;
	infoheader.biClrImportant = 0;
	int nBitsOffset = sizeof(BITMAPFILEHEADER) + infoheader.biSize + infoheader.biClrUsed*sizeof(RGBQUAD); 
	LONG lImageSize = infoheader.biSizeImage;
	LONG lFileSize = nBitsOffset + lImageSize;
	fileheader.bfType = 'B'+('M'<<8);
	fileheader.bfOffBits = nBitsOffset;
	fileheader.bfSize = lFileSize;
	fileheader.bfReserved1 = fileheader.bfReserved2 = 0;

	FILE* pFile;
	fopen_s(&pFile, filename, "wb");
	fwrite((char*) &fileheader, 1, sizeof(BITMAPFILEHEADER), pFile);
	fwrite((char*) &infoheader, 1, sizeof(BITMAPINFOHEADER), pFile);
	for (x=0; x<256; x++)		// write grayscale pallete
	{	pal.rgbBlue = pal.rgbGreen = pal.rgbRed = x;
		fwrite((char*) &pal, 1, sizeof(RGBQUAD), pFile);
	}
	for (y = lb.y; y <= rt.y; y++)
		fwrite((char*) (blob + y*100 + lb.x), 1, infoheader.biSizeImage/infoheader.biHeight, pFile);	// write entire scanline at once
	fclose(pFile);
}

