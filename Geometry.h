#pragma once
#include <iostream>


const double tolerance_small = 0.0001;

class _P3D;     // �����
class _V3D;     //������
class _Plane3D; // ���������


// ����� ������
//-------------------------------------------------------------------------
namespace BaseGeom
{
	// �������� �� ��, ����� �� ������� ����� �����
	bool haveCmnEnd(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4);

	// ������������ �� 2 �������.
	// ������������ ��� 2 ������� ����� � ����� ���������, ������� ������� dir
	bool isLineSegmentsIntersect(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4, const _V3D& dir);

	// ���� 2 ������� ����� � ������ ���������� , ����� ���������� ���������� ����� ������������� ����������� 
	//� ����� ���������� ���� ������� �� ���������, � ������� ��������� ������ �������
	// dir- ������, ���������������� ���� ��������

	double distBetweenSegments3D(_P3D& p1, _P3D& p2, _P3D& p3, _P3D& p4, const _V3D& dir);

	// ������� ������� ����������� ���������� ����� ������� ��������, ������� �� ���������
	double minDistanceBetweenEnds(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4);


	// �������� �������, ��������� ��������� ����� ����������� ���������
	// ���� ������ ������� ������� �� ���������� �����, ���������� -1
	double calculateMinDistance(const _P3D& p1, const _P3D& p2, const _P3D& p3, const _P3D& p4);
};
//-------------------------------------------------------------------------



// �����
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


	double distanseToLineSegment(const _P3D& pA, const _P3D& pB) const; // ����������� ���������� �� ������� (pA � pB - ����� �������)

private:
	double m_x;
	double m_y;
	double m_z;
};
//-------------------------------------------------------------------------


// ������
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


	_V3D normal() const;  // ���������� ����������������� ������
	double length() const;  // ����� �������	
	bool isNull() const;   // ������� ������ ��� ���
	_V3D& operator *=(double k);  // ��������� �� �����

	_V3D crossProduct(const _V3D& vecB) const;  // ��������� ���������� ������������ ���� ��������- ������
	double dotProduct(const _V3D& vecB) const;  // ��������� ���������� ������������ ���� ��������- �����

private:
	double m_x;
	double m_y;
	double m_z;
};
//-------------------------------------------------------------------------


// ���������
//-------------------------------------------------------------------------
class _Plane3D
{
public:

	_Plane3D() :m_A(1), m_B(0), m_C(0), m_D(0) {};
	_Plane3D(const _P3D& point, _V3D norm);

	~_Plane3D() {};

	double distanceToPoint(const _P3D& point) const;  // ���������� �� ��������� �� ������������ �����	
	bool isOn(const _P3D& point) const;  // ����� �� ����� �� ������ ���������

private:

	double m_A;
	double m_B;
	double m_C;
	double m_D;
};
//-------------------------------------------------------------------------



