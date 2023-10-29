#include "fft.h"
void _fft(CPLX buf[], CPLX out[], int n, int step)
{
   int i;
   CPLX t;
   double a, b, c, d, theta;

   if (step < n) 
   {
   	_fft(out, buf, n, step * 2);
   	_fft(out + step, buf + step, n, step * 2);

   	for (i = 0; i < n; i += 2 * step) 
   	{
   		theta = -PI * i / n;
   		a = cos(theta);
   		b = sin(theta);
   		c = out[i + step].real;
   		d = out[i + step].image;
   		t.real = a * c - b * d;
   		t.image = b * c + a * d;
   		buf[i / 2].real = out[i].real + t.real;
   		buf[i / 2].image = out[i].image + t.image;
   		buf[(i + n) / 2].real = out[i].real - t.real;
   		buf[(i + n) / 2].image = out[i].image - t.image;
   	}
   }
}

int fft(CPLX buf[], int n)
{
   int i;
   CPLX *out = (CPLX *)malloc(n * sizeof(CPLX));
   if (out == NULL)
   {
   	return 0;
   }

   for (i = 0; i < n; i++)
   {
   	out[i].real = buf[i].real;
   	out[i].image = buf[i].image;
   }

   _fft(buf, out, n, 1);

   free(out);
   out = NULL;
   return 1;
}
