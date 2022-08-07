#pragma once
#include <iostream>


const double tolerance_small = 0.0001;

class _P3D;     // точка
class _V3D;     //вектор
class _Plane3D; // плоскость


// Общие методы
//-------------------------------------------------------------------------
namespace BaseGeom
{
	// проверка на то, имеют ли отрезки общий конец
	bool haveCmnEnd(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4);

	// пересекаются ли 2 отрезка.
	// Потенциально эти 2 отрезка лежат в одной плоскости, нормаль которой dir
	bool isLineSegmentsIntersect(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4, const _V3D& dir);

	// если 2 отрезка лежат в разных плоскостях , метод возвращает расстояние между параллельными плоскостями 
	//а также проецирует один отрезок на плоскость, в которой находится другой отрезок
	// dir- вектор, перпендикулярный двум отрезкам

	double distBetweenSegments3D(_P3D& p1, _P3D& p2, _P3D& p3, _P3D& p4, const _V3D& dir);

	// функция находит минимальное расстояние между концами отрезков, попарно их перебирая
	double minDistanceBetweenEnds(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4);


	// основная функция, считающая дистанцию между переданными отрезками
	// Если нельзя создать отрезок по переданной точке, возвращает -1
	double calculateMinDistance(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4);
};
//-------------------------------------------------------------------------



// точка
//-------------------------------------------------------------------------
class _P3D
{
public:
	_P3D() :m_x(0), m_y(0), m_z(0) {};
	_P3D(double x, double y, double z) :m_x(x), m_y(y), m_z(z) {};
	_P3D(const _P3D& p) : m_x(p.x()), m_y(p.y()), m_z(p.z()) {};
	~_P3D() {};

	void set(double x, double y, double z) { m_x = x;  m_y = y;  m_z = z; };
	double x() const { return m_x; };
	double y() const { return m_y; };
	double z() const { return m_z; };

	_P3D operator+(const _V3D& vec) const;
	_P3D operator-(const _V3D& vec) const;
	bool operator== (const _P3D& rhs) const;


	double distanseToLineSegment(const _P3D& pA, const _P3D& pB) const; // минимальное расстояние до отрезка (pA и pB - концы отрезка)

private:
	double m_x;
	double m_y;
	double m_z;
};
//-------------------------------------------------------------------------


// вектор
//-------------------------------------------------------------------------
class _V3D
{
public:
	_V3D() :m_x(0), m_y(0), m_z(0) {};
	_V3D(double x, double y, double z) :m_x(x), m_y(y), m_z(z) {};
	_V3D(const _P3D& start, const _P3D& end);
	_V3D(const _V3D& v) : m_x(v.x()), m_y(v.y()), m_z(v.z()) {};

	~_V3D() {};

	void set(double x, double y, double z) { m_x = x;  m_y = y;  m_z = z; };
	double x() const { return m_x; };
	double y() const { return m_y; };
	double z() const { return m_z; };


	_V3D normal() const;  // возвращает нормализированный вектор
	double length() const;  // длина вектора	
	bool isNull() const;   // нулевой вектор или нет
	_V3D& operator *=(double k);  // умножение на число

	_V3D crossProduct(const _V3D& vecB) const;  // результат векторного произведения двух векторов- вектор
	double dotProduct(const _V3D& vecB) const;  // результат скалярного произведения двух векторов- число

private:
	double m_x;
	double m_y;
	double m_z;
};
//-------------------------------------------------------------------------


// плоскость
//-------------------------------------------------------------------------
class _Plane3D
{
public:

	_Plane3D() :m_A(1), m_B(0), m_C(0), m_D(0) {};
	_Plane3D(const _P3D& point, _V3D norm);

	~_Plane3D() {};

	double distanceToPoint(const _P3D& point) const;  // расстояние от плоскости до произвольной точки	
	bool isOn(const _P3D& point) const;  // лежит ли точка на данной плоскости

private:

	double m_A;
	double m_B;
	double m_C;
	double m_D;
};
//-------------------------------------------------------------------------



