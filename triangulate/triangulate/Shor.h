#pragma once

#include <memory>
#include <vector>
class Shor
{
private:
	int N ;
//	int** Q , **K ;
//	int *rArray;
//	Angle** firstAng , **lastAng ;
	std::vector<int> rArray;
	std::vector<std::vector<int>> Q, K;
	std::vector<std::vector<Angle>> firstAng, lastAng;
	Polygon_2 poly;
	int numOfTriangles;
	std::vector<int> tri_indices;
	bool isTriangultae;


public:
	std::vector<Triangle_2> triangulated;

	Shor();
	~Shor();
	void load_polygon ( Polygon_2 poly );
	void load_rArray ( std::vector<int>& arr );
	void init_tables();
	void play();
	bool addToTable ( int i , int k , int j );
	void build_triangulation();
	void addTriangle( int i , int j );
	void simplify_triangulation();
	void sort_and_check( std::vector<int>& arr, Polygon_2 &res_poly ,  std::vector<int>& temp );
};

