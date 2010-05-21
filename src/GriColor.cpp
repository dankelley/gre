//#define DEBUG_GRICOLOR // Debug

#include "GriColor.h"
#include "gr.h"
#include "types.h"
#include "macro.h"
#include "errors.h"

GriColor& GriColor::operator=(const GriColor& color)
{
	t = color.get_type();
	switch (t) {
	case rgb:
		a = color.getR();
		b = color.getG();
		c = color.getB();
		break;
	case hsv:
		a = color.getH();
		b = color.getS();
		c = color.getV();
		break;
	default:
		gre_assert(0, "GriColor::operator=() encountered unknown color type");
	}
	return *this;
}

GriColor::GriColor(const GriColor& color)
{
	t = color.get_type();
	switch (t) {
	case rgb:
		a = color.getR();
		b = color.getG();
		c = color.getB();
		break;
	case hsv:
		a = color.getH();
		b = color.getS();
		c = color.getV();
		break;
	default:
		gre_assert(0, "GriColor::GriColor() encountered unknown color type");
	}
}

void
GriColor::setRGB(double R, double G, double B)
{
	a = pin0_1(R);
	b = pin0_1(G);
	c = pin0_1(B);
	t = rgb;
}
void GriColor::setHSV(double H, double S, double V)
{
	a = pin0_1(H);
	b = pin0_1(S);
	c = pin0_1(V);
	t = hsv;
}

GriNamedColor::GriNamedColor()
{
	name = new char [1];
	if (!name) OUT_OF_MEMORY;
	name[0] = '\0';
	a = b = c = 0.0;
	t = rgb;
}

GriNamedColor::GriNamedColor(const char *n, double R, double G, double B)
{
	name = new char [1 + strlen(n)];
	if (!name) OUT_OF_MEMORY;
	strcpy(name, n);
	a = R;
	b = G;
	c = B;
	t = rgb;
}

GriNamedColor::GriNamedColor(const GriNamedColor& color) 
{
	char *cp = color.getName();	// prevent fcn call
	name = new char[1 + strlen(cp)];
	if (!name) OUT_OF_MEMORY;
	strcpy(name, cp);
	t = color.get_type();
	switch (t) {
	case rgb:
		a = color.getR();
		b = color.getG();
		c = color.getB();
		break;
	case hsv:
		a = color.getH();
		b = color.getS();
		c = color.getV();
		break;
	default:
		gre_assert(0, "GriColor::GriNamedColor() encountered unknown color type");
	}
}

GriNamedColor::~GriNamedColor()
{
	delete [] name;
}

GriNamedColor& GriNamedColor::operator=(const GriNamedColor& color)
{
	char *cp = color.getName();	// prevent fcn call
	if (strlen(cp) > strlen(name)) {
		delete [] name;
		name = new char[1 + strlen(cp)];
		if (!name) OUT_OF_MEMORY;
	}
	strcpy(name, cp);
	t = color.get_type();
	switch (t) {
	case rgb:
		a = color.getR();
		b = color.getG();
		c = color.getB();
		break;
	case hsv:
		a = color.getH();
		b = color.getS();
		c = color.getV();
		break;
	default:
		gre_assert(0, "GriNamedColor::operator=() encountered unknown color type");
	}
	return *this;
}

void GriNamedColor::setNameRGB(const char *newName, double R, double G, double B)
{
	if (strlen(newName) > strlen(name)) {
		delete [] name;
		name = new char[1 + strlen(newName)];
		if (!name) OUT_OF_MEMORY;
	}
	strcpy (name, newName);
	t = rgb;
	a = pin0_1(R);
	b = pin0_1(G);
	c = pin0_1(B);
}
void
GriColor::getRGB(double *R, double *G, double *B) const
{
	if (t == GriColor::rgb) {
		*R = a;
		*G = b;
		*B = c;
	} else if (t == GriColor::hsv) {
		gr_hsv2rgb(a, b, c, R, G, B);
	} else {
		fatal_error("Internal error in GriColor::getRGB");
	}
}
