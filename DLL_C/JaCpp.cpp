#include<Windows.h>
#include<iostream>
#include<math.h>

using namespace std;

void zmiana_na_sepie_c(unsigned char *& RGB, int poczatek , int zakres)
{
	int r, g, b, I = 51, Y, Q = 1;
	for (int i = poczatek; i <poczatek + zakres; i+=3) {

		r = RGB[i + 2];
		g = RGB[i + 1];
		b = RGB[i];

		Y = 0.299 * r + 0.587 * g + 0.114 * b;

		r = abs(1.0 * Y + 0.956 * I + 0.621 * Q);
		g = abs(1.0 * Y - 0.272 * I - 0.647 * Q);
		b = abs(1.0 * Y - 1.105 * I + 1.702 * Q);

				//B
		if (RGB[i] > 255)
			RGB[i] = 255;
		else
			RGB[i] = b;

				//G
		if (g > 255)
			RGB[i + 1] = 255;
		else
			RGB[i + 1] = g;

				//R
		if (r > 255)
			RGB[i + 2] = 255;
		else
			RGB[i + 2] = r;
	}
}