#include "Geometry.h"

// Пользовательский ввод координат точек
//-------------------------------------------------------------------------
void user_input(double& x, double& y, double& z)
{
	static int i = 1;
	std::cout << "Enter " << i++ << " point: " << std::endl;
	std::cin >> x >> y >> z;
}
// Вывод результата
//-------------------------------------------------------------------------
void user_output(double dist)
{
	std::cout << "Minimal distance " << dist << std::endl;
}
//-------------------------------------------------------------------------


int main()
{
	// Получили все координаты точек 
	double x, y, z;

	user_input(x, y, z); const _P3D p1(x, y, z);
	user_input(x, y, z); const _P3D p2(x, y, z);
	user_input(x, y, z); const _P3D p3(x, y, z);
	user_input(x, y, z); const _P3D p4(x, y, z);

	//const _P3D p1(x, y, z);
	//const _P3D p2(x, y, z);
	//const _P3D p3(x, y, z);
	//const _P3D p4(x, y, z);


	const double dist = BaseGeom::calculateMinDistance(p1, p2, p3, p4);
	user_output(dist);

	return 0;
}
