#pragma once

class CmpOper {
public:
	static bool LeqOper(double v1, double v2)
		{return v1 <= v2;}

	static bool LesOper(double v1, double v2)
		{return v1 < v2;}

	static bool GeqOper(double v1, double v2)
		{return v1 >= v2;}

	static bool GrtOper(double v1, double v2)
		{return v1 > v2;}

	static bool EqvOper(double v1, double v2)
		{return v1 == v2;}
};