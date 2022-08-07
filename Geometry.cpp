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
	// �������� ��������� ��������� X �� ������� (normVec) � ����� (�������� p1)
	// ����������, ������ ������� ����� � ��������� �
	const _Plane3D plane(p1, dir);

	// ����� ������ ������ �� ����� ���������!
	if (!plane.isOn(p1) || !plane.isOn(p2) || !plane.isOn(p3) || !plane.isOn(p4))
		return false;


	// ��������, ����� ������� ����� ����� �����
	if (haveCmnEnd(p1, p2, p3, p4))
		return true;


	//����� ���� ������- ���� ������� �1�2 � �3�4 ������������, �� �������������� ������� ������� ����� ������� 
	// ������� ������ ������ �� ������ �������

	const _V3D vec1 = (_V3D(p3, p4)).crossProduct(_V3D(p3, p1));
	const _V3D vec2 = (_V3D(p3, p4)).crossProduct(_V3D(p3, p2));
	const _V3D vec3 = (_V3D(p1, p2)).crossProduct(_V3D(p1, p3));
	const _V3D vec4 = (_V3D(p1, p2)).crossProduct(_V3D(p1, p4));


	//���� ������� ������������, �� ���� ����� ���� ����� 0�.��� ��� ������� ���� � 0� ����� �������,
	// �� ��������� ������������ �������������� �������� �������� ������������� �� ���� - > 0

	//���� ������� ���������� � ������ �������, ����� ���� ����� ���� 180�.
	//��� ��� ������� ���� � 180� ����� - 1, �� ��������� ������������ ��������������
	//������������ �������� ����� ������������ �� ����, ������� � �������� ������ - < 0

	const double v1 = vec1.dotProduct(dir);
	const double v2 = vec2.dotProduct(dir);
	const double v3 = vec3.dotProduct(dir);
	const double v4 = vec4.dotProduct(dir);

	//����, ���� v1*v2 < 0 � v3*v4 < 0, �� ������� ������������

	return  (v1 * v2 < 0) && (v3 * v4 < 0);
}
//-------------------------------------------------------------------------
double BaseGeom::distBetweenSegments3D(_P3D& p1, _P3D& p2, _P3D& p3, _P3D& p4, const _V3D& dir)
{
	// �������� ��������� ��������� X �� ������� (dir) � ����� (�������� p1)
	// ����������, ������ ������� ����� � ��������� �
	const _Plane3D plane(p1, dir);

	// ������ ���������� �� ����� p3 ��� �4- ��� � ����� ������� ����������� ����� ��������������� �������
	const double dist = plane.distanceToPoint(p3);
	//const double distance3D11 = plane.distanceToPoint(p4);

	//������ ����� ����� �����������- ������� ��� � ������
	_V3D vecBetweenPlanes = dir.normal();
	vecBetweenPlanes *= dist;

	// ������� �������� ������� ������� �� ���������
	_P3D p3_proj = p3 + vecBetweenPlanes;
	if (!plane.isOn(p3_proj))
		p3_proj = p3 - vecBetweenPlanes;

	_P3D p4_proj = p4 + vecBetweenPlanes;
	if (!plane.isOn(p4_proj))
		p4_proj = p4 - vecBetweenPlanes;


	//// �������� �� ��, ��� ������������� ��������� �������������
	//if (!plane.isOn(p3_proj) || !plane.isOn(p4_proj))
	//{
	//	return 0;
	//}

	//p1 � p2 ��� ����� � ���� ���������, 
	// �� �� ����� ������������
	p3 = p3_proj;
	p4 = p4_proj;

	return dist;
}
//-------------------------------------------------------------------------
double BaseGeom::minDistanceBetweenEnds(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4)
{
	//����������� ���������� ��� ���������� �� 4 ���������� �� ������ �������� �� ������� �������
	const double dist1 = p1.distanseToLineSegment(p3, p4);
	const double dist2 = p2.distanseToLineSegment(p3, p4);
	const double dist3 = p3.distanseToLineSegment(p1, p2);
	const double dist4 = p4.distanseToLineSegment(p1, p2);

	return std::min(std::min(dist1, dist2), std::min(dist3, dist4));
}
//-------------------------------------------------------------------------
double BaseGeom::calculateMinDistance(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4)
{
	// ��������, ����� ������� ����� ����� �����
	if (haveCmnEnd(p1, p2, p3, p4))
		return 0;

	// ������� ������������ �������
	const _V3D v12(p1, p2);
	const _V3D v34(p3, p4);

	// �������� �� ��, ��� ������� �������� ������ (����� ���������)
	if (v12.isNull() || v34.isNull())
	{
		//std::cout << "incorrect points" << std::endl;
		return -1;
	}


	// ������ ������, ���������������� ������ ��������
	const _V3D cross = v12.crossProduct(v34);

	double min_dist{ 0 };

	// ���� ������ �������, �� ������ ������� �����������, � ������������� ������, ���������� �������, �����������
	if (cross.isNull())
	{
		//����������� ���������� ��� ���������� �� 4 ���������� �� ������ �������� �� ������� �������
		min_dist = BaseGeom::minDistanceBetweenEnds(p1, p2, p3, p4);
	}
	else
	{
		// ������ ���������� ����� ��������������� �������. ��� ����, ���� ��� ���������, ����������� ��� ����� �� ���� ���������
		_P3D p1_proj(p1);
		_P3D p2_proj(p2);
		_P3D p3_proj(p3);
		_P3D p4_proj(p4);

		// ������� ���������� ����� ����������� � ���������� ����� �� ���� ���������
		min_dist = BaseGeom::distBetweenSegments3D(p1_proj, p2_proj, p3_proj, p4_proj, cross);

		//�������� �� ����������� ��������������� ��������
		if (!BaseGeom::isLineSegmentsIntersect(p1_proj, p2_proj, p3_proj, p4_proj, cross))
		{
			//���� ��������������� �� ���� ��������� ������� �� ������������, 
			//����������� ���������� ��� ���������� �� 4 ���������� �� ������ �������� �� ������� �������
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
	//����������� �� ����� �� ������� �������� ���� �������������, ��������� �� ���� ����� �� �������,
	//���� ����������� ���������� �� ����� �� ������ �� ������ �������.

	//���� �����������, ��������� �������� �������� ������ ����� � ����� ��������� �������,
	//�������� ������������(�������� �� �������������� ���������� ������������� ����� ���������� 
	//������������ ��������������� ��������, ����������� �� �������� ������������(��� ������ ���� �������������))
	//(�.�.�� ������ ����� ���������� �������� ������������� �� ������ �������), �� ����������� �� ����� �� �������
	//��������� ����������� ���������� �� ������ �����, �� ������ �� ������ �������.

	const _V3D v1(p1, *this);
	const _V3D v2(p2, *this);
	const _V3D vP1P2(p1, p2);
	const _V3D vP2P1(p2, p1);

	if (vP1P2.dotProduct(v1) < 0 || vP2P1.dotProduct(v2) < 0)
	{
		return std::min(v1.length(), v2.length());
	}


	// ������� ����� ��������������
	// ������� ������ ������� ������������
	// ��� �������� ������� ���������������, ������������ �� �������� v1 � v2

	// ������ �������, ���������� �� 2 (����� ����� ����� �� �������� �� 2)
	const double square2x = _V3D(vP1P2.crossProduct(v1)).length();

	// ������ ����� �������� ������, ���� ������� � ����� ���������
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
	// ��������� ������ 
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

