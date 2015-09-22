// last update - 15/7 , 23:00
#include "stdafx.h"
#define DEBUG 0

void debug_func();

void main()
{
	if ( DEBUG )
	{
		debug_func();
		return;
	}

	Polygon_2 poly;

	int numOfPoints;
	double x,y;
	std::fstream myfile;
	myfile.open ("out.txt");
	myfile >> numOfPoints;

	int* rArray = new int [ numOfPoints ];
	int r;

	for (int i=0; i<numOfPoints; ++i)
	{
		  myfile >> x >> y >> r;
		  poly.push_back( Point_2(x,y) );
		  rArray [i] = r;
	 }

	myfile.close();

	Shor shor;
	shor.load_polygon (poly);
	shor.load_rArray (rArray);
	shor.play();
	shor.build_triangulation();
	shor.simplify_triangulation();

	std::ofstream out("outC.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

	for ( int i = 0 ; i < shor.triangulated.size() ; ++i )
	{
		std::cout  << shor.triangulated[i][0][0] << " " << shor.triangulated[i][0][1] <<std::endl;

		std::cout  << shor.triangulated[i][1][0] << " " << shor.triangulated[i][1][1] <<std::endl;
														
		std::cout  << shor.triangulated[i][2][0] << " " << shor.triangulated[i][2][1] <<std::endl;

	}

	
	delete[] rArray;
	//system("pause");

}



void debug_func()
{
	Angle ang1( Point_2( 1 , 0 ) , Point_2( 0 , 0 ) , Point_2( -1 , 0 ) );
	Angle ang2( Point_2( -1 , 0 ) , Point_2( 0 , 0 ) , Point_2( 0 , -1 ) );
	Angle ang3( Point_2( 0 , -1 ) , Point_2( 0 , 0 ) , Point_2( 1 , 0 ) );
	Angle ang4 = ( ang1 + ang2 ) + ang3;
	std::cout << "ang1 = "<< ang1.getVal() << std::endl;
	std::cout << "ang2 = "<< ang2.getVal() << std::endl;
	std::cout << "ang3 = "<< ang3.getVal() << std::endl;
	std::cout << "ang4 = "<< ang4.getVal() << std::endl;
	std::cout << "rotation of ang4 is: R = "<< ang4.getR() << std::endl;
	system("pause");

						/*//debug
					if ( !tempL.isValid() )
					{
						std::cout<<"tempL problem\n";
						system ("pause");
					}*/
}
