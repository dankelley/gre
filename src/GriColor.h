// @(#) GriColor.hh 1.20 (c) 97/06/19 Dan E. Kelley
#ifndef _gricolor_h_
#define  _gricolor_h_
#include <stdio.h>
#include "types.h"
class GriColor
{
 public:
	enum type {rgb, hsv};
	GriColor() { t = rgb; a = b = c = 0.0;};
	GriColor(const GriColor& c);
	~GriColor() {};
	bool operator<  (const GriColor& ignored) const {return false;}
	bool operator== (const GriColor& ignored) const {return false;}
	GriColor& operator=(const GriColor& c);
	void setHSV(double H, double S, double V);
	void setRGB(double R, double G, double B);
	bool isRGB() const {return t == rgb;}
	void set_type(type tt) {t = tt;}
	type get_type() const {return t;}
	void getRGB(double *R, double *G, double *B) const;
	double getR() const {return a;}
	double getG() const {return b;}
	double getB() const {return c;}
	double getH() const {return a;}
	double getS() const {return b;}
	double getV() const {return c;}
 protected:
	type t;
	double a;			// red, or hue
	double b;			// green, or saturation
	double c;			// blue, or brightness
};
class GriNamedColor : public GriColor
{
 public:
	GriNamedColor();
	~GriNamedColor();
	GriNamedColor(const char *n, double R, double G, double B);
	GriNamedColor(const GriNamedColor& C); 
	bool operator<  (const GriNamedColor& ignored) const {return false;}
	bool operator== (const GriNamedColor& ignored) const {return false;}
	GriNamedColor& operator=(const GriNamedColor& C);
	void setNameRGB(const char *n, double R, double G, double B);
	char *getName() const {return name;}
 private:
	char *name;
};
#endif
