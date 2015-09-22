#include<iostream>
#include <fstream>
#include <string>
#include<CGAL/Polygon_2.h>
#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/enum.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Vector_2 Vector_2;
typedef CGAL::Polygon_2<Kernel>    Polygon_2;//Contour;
typedef CGAL::Triangle_2<Kernel> Triangle_2;
#include "Angle.h"
#include "Shor.h"


typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;

#include "triangulation.h"
/*
#include <vector>
#include <complex>
#include "triangulation.h"
#include "cassert"


extern "C"
{
	#include "triangle.h"
}*/

