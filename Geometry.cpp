#include "Geometry.h"


// BaseGeom
//-------------------------------------------------------------------------
bool BaseGeom::haveCmnEnd(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4)
{
	return (p1 == p3 || p1 == p4 || p2 == p3 || p2 == p4);
}
//-------------------------------------------------------------------------
bool BaseGeom::isLineSegmentsIntersect(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4, const _V3D& dir)
{
	// составим уравнение плоскости X по нормали (normVec) и точке (например p1)
	// получается, первый отрезок лежит в плоскости Х
	const _Plane3D plane(p1, dir);

	// точки должны лежать на одной плоскости!
	if (!plane.isOn(p1) || !plane.isOn(p2) || !plane.isOn(p3) || !plane.isOn(p4))
		return false;


	// Проверка, вдруг отрезки имеют общий конец
	if (haveCmnEnd(p1, p2, p3, p4))
		return true;


	//Далее суть метода- если отрезки р1р2 и р3р4 пересекаются, то относительного каждого отрезка концы другого 
	// отрезка должны лежать по разную сторону

	const _V3D vec1 = (_V3D(p3, p4)).crossProduct(_V3D(p3, p1));
	const _V3D vec2 = (_V3D(p3, p4)).crossProduct(_V3D(p3, p2));
	const _V3D vec3 = (_V3D(p1, p2)).crossProduct(_V3D(p1, p3));
	const _V3D vec4 = (_V3D(p1, p2)).crossProduct(_V3D(p1, p4));


	//Если векторы сонаправлены, то угол между ними равен 0°.Так как косинус угла в 0° равен единице,
	// то скалярное произведение сонаправленных векторов является произведением их длин - > 0

	//Если векторы направлены в разные стороны, тогда угол между ними 180°.
	//Так как косинус угла в 180° равен - 1, то скалярное произведение противоположно
	//направленных векторов равно произведению их длин, взятому с обратным знаком - < 0

	const double v1 = vec1.dotProduct(dir);
	const double v2 = vec2.dotProduct(dir);
	const double v3 = vec3.dotProduct(dir);
	const double v4 = vec4.dotProduct(dir);

	//Итак, если v1*v2 < 0 и v3*v4 < 0, то отрезки пересекаются

	return  (v1 * v2 < 0) && (v3 * v4 < 0);
}
//-------------------------------------------------------------------------
double BaseGeom::distBetweenSegments3D(_P3D& p1, _P3D& p2, _P3D& p3, _P3D& p4, const _V3D& dir)
{
	// составим уравнение плоскости X по нормали (dir) и точке (например p1)
	// получается, первый отрезок лежит в плоскости Х
	const _Plane3D plane(p1, dir);

	// найдем расстояние до точки p3 или з4- это и будет искомым расстоянием между скрещивающимися прямыми
	const double dist = plane.distanceToPoint(p3);
	//const double distance3D11 = plane.distanceToPoint(p4);

	//вектор между двумя плоскостями- находим его в размер
	_V3D vecBetweenPlanes = dir.normal();
	vecBetweenPlanes *= dist;

	// сделаем проекцию второго отрезка на плоскость
	_P3D p3_proj = p3 + vecBetweenPlanes;
	if (!plane.isOn(p3_proj))
		p3_proj = p3 - vecBetweenPlanes;

	_P3D p4_proj = p4 + vecBetweenPlanes;
	if (!plane.isOn(p4_proj))
		p4_proj = p4 - vecBetweenPlanes;


	//// проверка на то, что действительно правильно спроецировали
	//if (!plane.isOn(p3_proj) || !plane.isOn(p4_proj))
	//{
	//	return 0;
	//}

	//p1 и p2 уже лежат в этой плоскости, 
	// их не нужно проецировать
	p3 = p3_proj;
	p4 = p4_proj;

	return dist;
}
//-------------------------------------------------------------------------
double BaseGeom::minDistanceBetweenEnds(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4)
{
	//минимальное расстояние это наименьшее из 4 расстояний от концов отрезков до другого отрезка
	const double dist1 = p1.distanseToLineSegment(p3, p4);
	const double dist2 = p2.distanseToLineSegment(p3, p4);
	const double dist3 = p3.distanseToLineSegment(p1, p2);
	const double dist4 = p4.distanseToLineSegment(p1, p2);

	return std::min(std::min(dist1, dist2), std::min(dist3, dist4));
}
//-------------------------------------------------------------------------
double BaseGeom::calculateMinDistance(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4)
{
	// Проверка, вдруг отрезки имеют общий конец
	if (haveCmnEnd(p1, p2, p3, p4))
		return 0;

	// находим направляющие векторы
	const _V3D v12(p1, p2);
	const _V3D v34(p3, p4);

	// проверка на то, что отрезок является точкой (точки совпадают)
	if (v12.isNull() || v34.isNull())
	{
		//std::cout << "incorrect points" << std::endl;
		return -1;
	}


	// найдем вектор, перпендикулярный данным векторам
	const _V3D cross = v12.crossProduct(v34);

	double min_dist{ 0 };

	// если вектор нулевой, то значит вектора коллинеарны, а следовательно прямые, содержащие отрезки, параллельны
	if (cross.isNull())
	{
		//минимальное расстояние это наименьшее из 4 расстояний от концов отрезков до другого отрезка
		min_dist = BaseGeom::minDistanceBetweenEnds(p1, p2, p3, p4);
	}
	else
	{
		// найдем расстояние между скрещивающимися прямыми. При этом, если оно ненулевое, спроецируем все точки на одну плоскость
		_P3D p1_proj(p1);
		_P3D p2_proj(p2);
		_P3D p3_proj(p3);
		_P3D p4_proj(p4);

		// находим расстояние между плоскостями и проецируем точки на одну плоскость
		min_dist = BaseGeom::distBetweenSegments3D(p1_proj, p2_proj, p3_proj, p4_proj, cross);

		//Проверка на пересечение спроецированных отрезков
		if (!BaseGeom::isLineSegmentsIntersect(p1_proj, p2_proj, p3_proj, p4_proj, cross))
		{
			//Если спроецированные на одну плоскость отрезки не пересекаются, 
			//минимальное расстояние это наименьшее из 4 расстояний от концов отрезков до другого отрезка
			min_dist = BaseGeom::minDistanceBetweenEnds(p1, p2, p3, p4);
		}
	}

	return min_dist;
}
//-------------------------------------------------------------------------


// class _P3D
//-------------------------------------------------------------------------
_P3D _P3D::operator+(const _V3D& vec) const
{
	return _P3D(m_x + vec.x(), m_y + vec.y(), m_z + vec.z());
}
//-------------------------------------------------------------------------
_P3D _P3D::operator-(const _V3D& vec) const
{
	return _P3D(m_x - vec.x(), m_y - vec.y(), m_z - vec.z());
}
//-------------------------------------------------------------------------
bool _P3D::operator==(const _P3D& rhs) const
{
	return (abs(m_x - rhs.x()) < tolerance_small &&
		abs(m_y - rhs.y()) < tolerance_small &&
		abs(m_z - rhs.z()) < tolerance_small);
}
//-------------------------------------------------------------------------
double _P3D::distanseToLineSegment(const _P3D& p1, const _P3D& p2) const
{
	//Расстоянием от точки до отрезка является либо перпендикуляр, опущенный из этой точки на отрезок,
	//либо минимальное расстояние от точки до одного из концов отрезка.

	//Если треугольник, вершинами которого является данная точка и концы заданного отрезка,
	//является тупоугольным(проверка на тупоугольность проводится рассмотрением знака скалярного 
	//произведения соответствующих векторов, построенных на сторонах треугольника(кос тупого угла отрицательный))
	//(т.е.из данной точки невозможно опустить перпендикуляр на данный отрезок), то расстоянием от точки до отрезка
	//считается минимальное расстояние от данной точки, до одного из концов отрезка.

	const _V3D v1(p1, *this);
	const _V3D v2(p2, *this);
	const _V3D vP1P2(p1, p2);
	const _V3D vP2P1(p2, p1);

	if (vP1P2.dotProduct(v1) < 0 || vP2P1.dotProduct(v2) < 0)
	{
		return std::min(v1.length(), v2.length());
	}


	// находим длину перпендикуляра
	// Сначала найдем площадь треугольника
	// это половина площади параллелограмма, построенного на векторах v1 и v2

	// найдем площадь, умноженную на 2 (чтобы потом опять не умножать на 2)
	const double square2x = _V3D(vP1P2.crossProduct(v1)).length();

	// теперь можно выразить высоту, зная площадь и длину основания
	return square2x / vP1P2.length();
}
//-------------------------------------------------------------------------


// class _V3D
//-------------------------------------------------------------------------
_V3D::_V3D(const _P3D& start, const _P3D& end)
{
	m_x = end.x() - start.x();
	m_y = end.y() - start.y();
	m_z = end.z() - start.z();
}
//-------------------------------------------------------------------------
_V3D _V3D::normal() const
{
	const double len = length();
	return _V3D(m_x / len, m_y / len, m_z / len);
}
//-------------------------------------------------------------------------
double _V3D::length() const
{
	return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}
//-------------------------------------------------------------------------
bool _V3D::isNull() const
{
	return abs(m_x) < tolerance_small &&
		abs(m_y) < tolerance_small &&
		abs(m_z) < tolerance_small;
}
//-------------------------------------------------------------------------
_V3D& _V3D::operator*=(double k)
{
	m_x *= k;
	m_y *= k;
	m_z *= k;

	return *this;
}
//-------------------------------------------------------------------------
_V3D _V3D::crossProduct(const _V3D& vecB) const
{
	const double bX = vecB.x();
	const double bY = vecB.y();
	const double bZ = vecB.z();

	return _V3D(m_y * bZ - m_z * bY,
		m_z * bX - m_x * bZ,
		m_x * bY - m_y * bX);
}
//-------------------------------------------------------------------------
double _V3D::dotProduct(const _V3D& vecB) const
{
	return m_x * vecB.x() + m_y * vecB.y() + m_z * vecB.z();
}
//-------------------------------------------------------------------------


// class _Plane3D
//-------------------------------------------------------------------------
_Plane3D::_Plane3D(const _P3D& point, _V3D norm)
{
	// нормируем вектор 
	norm.normal();

	m_A = norm.x();
	m_B = norm.y();
	m_C = norm.z();
	m_D = -m_A * point.x() - m_B * point.y() - m_C * point.z();
}
//-------------------------------------------------------------------------
double _Plane3D::distanceToPoint(const _P3D& point) const
{
	const double normFactor = sqrt(m_A * m_A + m_B * m_B + m_C * m_C);
	return abs(m_A * point.x() + m_B * point.y() + m_C * point.z() + m_D) / normFactor;
}
//-------------------------------------------------------------------------
bool _Plane3D::isOn(const _P3D& point) const
{
	return (abs(m_A * point.x() + m_B * point.y() + m_C * point.z() + m_D) < tolerance_small);
}
//-------------------------------------------------------------------------

