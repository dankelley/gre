#include	<string>
#include	<vector>
//#include	<strings.h>
//#include	<unistd.h>
#include	<stdio.h>
//#include	<time.h>
#include	"gr.h"
#include	"extern.h"
#include        "private.h"
//#include	"defaults.h"
//#include	"files.h"
//#include        "gr_coll.h"
#include        "macro.h"
#include        "GriColor.h"
//#include        "superus.h"

using namespace std;
static vector<GriNamedColor> colorStack;

bool _contour_label_rotated = false;   // <-> draw.c set.c
bool _contour_label_whiteunder = true; // <-> draw.c set.c



// RETURN 1 if found color and dumped RGB into red, green, blue
bool
look_up_color(const char *name, double *red, double *green, double *blue)
{
	int             i = 0, num = colorStack.size();
	for (i = 0; i < num; i++) {
		if (!strcmp(name, colorStack[i].getName())) {
			*red   = colorStack[i].getR();
			*green = colorStack[i].getG();
			*blue  = colorStack[i].getB();
			return true;
		}
	}
	return false;
}

bool
create_color(const char *name, double r, double g, double b)
{
	GriNamedColor color(name, r, g, b);
	colorStack.push_back(color);
	return true;
}


void
create_builtin_colors()
{
	GriNamedColor       color;

	color.setNameRGB(		"white",		1.000,	1.000,	1.000);
	colorStack.push_back(color);

	color.setNameRGB(		"LightGray",		0.827,	0.827,	0.827);
	colorStack.push_back(color);

	color.setNameRGB(		"darkslategray",	0.184,	0.310,	0.310);
	colorStack.push_back(color);

	color.setNameRGB(		"black",		0.000,	0.000,	0.000);
	colorStack.push_back(color);

	color.setNameRGB(		"red",			1.000,	0.000,	0.000);
	colorStack.push_back(color);

	color.setNameRGB(		"brown",		0.647,	0.165,	0.165);
	colorStack.push_back(color);

	color.setNameRGB(		"tan",			0.824,	0.706,	0.549);
	colorStack.push_back(color);

	color.setNameRGB(		"orange",		1.000,	0.647,	0.000);
	colorStack.push_back(color);

	color.setNameRGB(		"yellow",		1.000,	1.000,	0.000);
	colorStack.push_back(color);

	color.setNameRGB(		"green",		0.000,	1.000,	0.000);
	colorStack.push_back(color);

	color.setNameRGB(		"ForestGreen",		0.133,	0.545,	0.133);
	colorStack.push_back(color);

	color.setNameRGB(		"cyan",			0.000,	1.000,	1.000);
	colorStack.push_back(color);

	color.setNameRGB(		"blue",			0.000,	0.000,	1.000);
	colorStack.push_back(color);

	color.setNameRGB(		"skyblue",		0.529,	0.808,	0.922);
	colorStack.push_back(color);

	color.setNameRGB(		"magenta",		1.000,	0.000,	1.000);
	colorStack.push_back(color);
}
bool
start_up()
{
	//printf("debug: in startup.cpp:startup()\n");
	extern unsigned char *_imageTransform; // image.cpp
	extern double        *_imageHist;      // image.cpp
	GET_STORAGE(_imageTransform, unsigned char, 3 * 256);
	GET_STORAGE(_imageHist, double, 256);
	return true;
}
