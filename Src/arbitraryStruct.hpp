#ifndef ARB_STRUCT_H
#define ARB_STRUCT_H

struct arbitrary
{
	arbitrary(int a) { anInt = a;};
	arbitrary(double a) { aDouble = a;};

	int anInt;
	double aDouble;
};

#endif
