// last update - 16/07 , 15:15
#include "stdafx.h"

Shor::Shor()
	:
		N(0)	
{
}

Shor::~Shor()
{
}

void Shor::load_polygon( Polygon_2 newPoly )
{
	poly = newPoly ;
	N = poly.size();

	Q.resize(N);
	K.resize(N);
	firstAng.resize(N);
	lastAng.resize(N);

	for ( int i = 0 ; i < N ; ++i )
	{
		Q[i].resize(N);
		K[i].resize(N);
		firstAng[i].resize(N);
		lastAng[i].resize(N);
	}

	for ( int i = 0 ; i < N ; ++i )
		for ( int j = 0 ; j < N ; ++j )
			K[i][j] = -1;
}

void Shor::load_rArray ( std::vector<int>& arr )
{
	rArray.clear();
	if ( rArray.size() != N )
		rArray.resize(N);

	for ( int i = 0; i<N ; i++ )
		rArray[i] = arr[i];
}

void Shor::init_tables()
{
	Point_2 a;
	Point_2 b;
	Point_2 c;

	for ( int i = 0 ; i < N ; ++i ) // init
	{
		a = poly[i];
		b = poly[(i+1)%N];
		c = poly[(i+2)%N];

		Q[i][(i+1)%N] = 1;
		firstAng[i][(i+1)%N] = Angle ( b , a , b );
		lastAng[i][(i+1)%N] = Angle ( a , b , a );
		
		Triangle_2 tri(a , b , c );

		if ( tri.orientation() > 0 )
		{
			Q[i][(i+2)%N] = 1;
			K[i][(i+2)%N] = (i + 1)%N;
			firstAng[i][(i+2)%N] = Angle( b , a , c ); //[k , i , j]
			lastAng[i][(i+2)%N] = Angle ( a , c , b ); //[i , j , k]
		}
		else
			Q[i][(i+2)%N] = 0;
	}
}

void Shor::play()
{
	int i , j , k , d; // i - start index, k- spliting index , j- end index, d- distance

	this -> init_tables();

	bool stop = false;
	for ( d = 3 ; d < N ; ++d )			//sweep the distances
	{
		for ( i = 0 ; i < N ; ++i )		//sweep the vertices
		{
			j = (i + d)%N;
			k = (i + 1)%N;
			while ( k != j )			//sweep from v_i to v_j
			{
				if ( Q[i][k] == 1 && Q[k][j] == 1 )
				{
					if  ( this->addToTable( i , k , j ) )
					{
						Q[i][j] = 1;
						K[i][j] = k;

						if ( i == (j + 1)%N )
						{
							stop = true;
							break;
						}
					}
				}
				k = (k + 1)%N;
			}
			if ( stop )
				break;
		}
		if ( stop )
			break;
	}

}

bool Shor::addToTable ( int i , int k , int j )
{
	Point_2 a,b,c,helpP;
	Angle temp1,temp2,tempF,tempL;

	a = poly[i];
	b = poly[k];
	c = poly[j];

	Triangle_2 tri(a , b , c );

	Angle k1 ( poly[ j ] , poly[ k ] , poly[ i ] );
	Angle k2 ( poly[ i ] , poly[ k ] , poly[ ( k-1+N )%N ] );
	Angle k3 ( poly[ ( k-1+N )%N ] , poly[ k ] , poly[ ( k+1 )%N ] );
	Angle k4 ( poly[ ( k+1 )%N ] , poly[ k ] , poly[ j ] );
	Angle kSum = k1 + k2 + k3 + k4;

	bool triFlag = true;
	helpP = poly[(i+1)%N];
	Triangle_2 triI1(a , helpP , b );//[i,i+1,k]
	Triangle_2 triI2(a , c , helpP );//[i,j,i+1]
	if ( triI1.orientation() < 0 )
		if ( triI2.orientation() < 0 )
			triFlag = false;

	helpP = poly[(j-1+N)%N];
	Triangle_2 triJ1(c , b , helpP );//[j,k,j-1]
	Triangle_2 triJ2(a , c , helpP );//[i,j,j-1]
	if ( triJ1.orientation() < 0 )
		if ( triJ2.orientation() < 0 )
			triFlag = false;

	// extend algoritem: check the rotation of the angles
	temp1 = Angle ( b , a , c ); // [k,i,j]
	temp2 = Angle ( a , c , b ); // [i,j,k]
	tempF = firstAng[i][k] + temp1;
	tempL = temp2 + lastAng[k][j];

	//update angle table
	firstAng[i][j] = tempF;	
	lastAng[i][j] = tempL;

	bool kAngleFlag = (kSum.getR() == 0);

	if ( ( tempF.getR() <= rArray[i] ) && ( tempL.getR() <= rArray[j] ) && ( (k4+k1+k2).getR() == rArray[k] ) )
						kAngleFlag = true;
					/*else
						triFlag = false;*/
	if  ( ( tri.orientation() > 0 ) && ( kAngleFlag ) && ( triFlag ) )
		return true;
	else
		return false;
}

void Shor::build_triangulation()
{
	int i;
	bool build = false;
	this->numOfTriangles = 0;
	for ( i = 0 ; i < N ; i++)
	{
		if ( Q[i][ (i-1+N)%N ] == 1 )
		{
			build = true;
			break;
		}
	}

	if (build)
	{
		this->addTriangle( i , (i - 1 + N)%N );
		this->isTriangultae = true;
	}
	else
		this->isTriangultae = false;
}

void Shor::addTriangle( int i , int j )
{
	if ( K[i][j] == -1 )	//( ( std::abs( i - j ) < 2 ) && ( i != (j+1)%N ) )
		return;

	int k = K[i][j];

	triangulated.push_back( Triangle_2( poly[i] , poly[k] , poly[j] ) );

	tri_indices.push_back ( i );
	tri_indices.push_back ( k );
	tri_indices.push_back ( j );

	this->numOfTriangles++;
	addTriangle( i , k );
	addTriangle( k , j );
}

void swap( int &a , int&b )
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}

bool final_check_helper ( std::vector<int>& arr )
{
	arr[2] = -1;
	if ( arr[0] > arr[1] )
			swap ( arr[0] , arr[1] );
		if ( arr[1] > arr[0]+1 )
			return true;
		else
			return false;
}

bool final_check ( bool f1 , bool f2, bool f3, std::vector<int>& arr )
{
	if (!f1)
		arr[0] = -1;
	if (!f2)
		arr[1] = -1;
	if (!f3)
		arr[2] = -1;

	if ( f1&&f2 )
		return (final_check_helper(arr));
	if ( f1&&f3 )
	{
		swap ( arr[1] , arr[2] );
		return (final_check_helper(arr));
	}
	if ( f2&&f3 )
	{
		swap ( arr[0] , arr[2] );
		return (final_check_helper(arr));
	}
	return (false);
}

void Shor::sort_and_check( std::vector<int>& arr, Polygon_2 &res_poly ,  std::vector<int>& temp )
{
	temp[0]=arr[0];
	temp[1]=arr[1];
	temp[2]=arr[2];
	//check if the points in res_poly:
	bool f1 = false , f2 = false , f3 = false ;
	bool check1 = true, check2 = true, check3 = true;
	if ( arr[0] == -1 )
	{
		check1 = false;
		f1 = true;
	}
	if ( arr[1] == -1 )
	{
		check2 = false;
		f2 = true;
	}
	if ( arr[2] == -1 )
	{
		check3 = false;
		f3 = true;
	}

	for ( int i = 0, len = (int)res_poly.size(); i< len; i++ )
	{
		if (check1)
			if ( (!f1)&&( poly[ arr [0] ] == res_poly[i] ) )
			{
				f1 = true;
				arr[0] = i;
			}
		if (check2)
			if ( (!f2)&&( poly[ arr [1] ] == res_poly[i] ) )
			{
				f2 = true;
				arr[1] = i;
			}
		if (check3)
			if ( (!f3)&&( poly[ arr [2] ] == res_poly[i] ) )
			{
				f3 = true;
				arr[2] = i;
			}
		
		if (f1&&f2&&f3)
			break;
	}

	if ( !(f1&&f2&&f3) )
	{
		if ( !final_check( f1,f2,f3,arr ) )
			arr[0] = -1;
		return;
	}

	if ( arr[0] > arr[1] )
	{
		if ( arr[0] > arr[2] )
		{
			swap ( arr[0] , arr[2] );
			swap ( temp[0] , temp[2] );
			if ( arr[0] > arr[1] )
			{
				swap( arr[0], arr[1] );
				swap ( temp[0] , temp[1] );
			}
		}
		else
		{
			swap ( arr[0], arr[1] );
			swap ( temp[0] , temp[1] );
		}
	}
	else
	{
		if ( arr[1] > arr[2] )
		{
			swap ( arr[1] , arr[2] );
			swap ( temp[1] , temp[2] );
			if ( arr[0] > arr[1] )
			{
				swap ( arr[0], arr[1] );
				swap ( temp[0] , temp[1] );
			}
		}	
	}

	//now we search for a diagonal in the original polygon
	if ( arr[1] - arr[0] > 1 )
		return;
	else
		if ( arr[2] - arr[1] > 1 )
		{
			swap ( arr[0] , arr[1] );
			swap ( arr[1] , arr[2] );
			return;
		}

	swap( arr[1] , arr[2]) ;

}


void unite( std::vector<Polygon_2> &simple_list )
{
	if ( simple_list.size() < 2 )
		return;
	Polygon_2 p_new = simple_list[simple_list.size()-1];
	Polygon_2 p_old;
	Polygon_2 p_union;
	bool toQuit = true;

	int nSize = p_new.size();
	int oSize;
	int i,j;	//i - new_p , j - old_p
	int count = 0;

	int len = (int)simple_list.size()-1;
	while ( count < len )
	{
		p_old = simple_list[count];
		oSize = p_old.size();
		for ( i = 0; i < nSize; i++ )
		{
			for ( j = 0; j < oSize; j++ )
			{
				if ( ( p_new[i] == p_old[j] ) && ( p_new[(i+1)%nSize] == p_old[(j-1+oSize)%oSize] ) )
				{
					toQuit = false;
					break;
				}
			}
	
			if ( !toQuit )
				break;
		}

		if ( !toQuit )
			break;
		count++;
	}

	if ( toQuit )
		return;
	Polygon_2 combined_poly;
	int new_index = 0 , old_index = 0;
	bool loop_flag = true;
	for ( new_index = 0; new_index < i ; new_index++ )
		combined_poly.push_back(p_new[new_index]);
	for ( old_index = j; old_index != (j-1+oSize)%oSize ; old_index++ )
		combined_poly.push_back(p_old[old_index]);
	
	for ( new_index = i+1 ; new_index < nSize ; new_index++ )
		combined_poly.push_back(p_new[new_index]);
	
	if ( combined_poly.is_simple() )
	{
		simple_list.pop_back();
		simple_list.erase( simple_list.begin() + count );
		simple_list.push_back(combined_poly);
		unite(simple_list);
	}


}

void check_for_simple( std::vector<Polygon_2> &simple_list , Polygon_2 &res_poly, std::vector<int>& tri, bool& flag )
{
	Polygon_2 poly1,poly2;
	int j;
	
	if ( ( tri[0] == -1 ) || ( tri[1] == -1 ) )
		return;

	for ( int i = tri[0] ; i <= tri[1] ; i++)
		poly1.push_back( res_poly[i] );
	j=tri[1];
	while ( 1 )
	{
		poly2.push_back( res_poly[j] );
		j = (j+1) % ( res_poly.size() );
		if ( j == tri[0]+1 )
			break;
	}

	if ( poly1.is_simple() )
	{
		if ( poly2.is_simple() )
		{
			simple_list.push_back ( poly1 );
			unite(simple_list);
			simple_list.push_back ( poly2 );
			unite(simple_list);
			flag = false;
			return;
		}
		else	// poly2 not simple
		{
			simple_list.push_back ( poly1 );
			unite(simple_list);
			res_poly = poly2;
				
		}
	}
	else	//poly 1 not simple
		if ( poly2.is_simple() )
		{
			simple_list.push_back ( poly2 );
			unite(simple_list);
			res_poly = poly1;
					
		}
				
}

void Shor::simplify_triangulation()
{
	if ( !isTriangultae )
		return;
	std::vector<Polygon_2> simple_list;
	Polygon_2 res_poly = poly;
	int counter = 0 ,hope;
	std::vector<int> tri(3),original_indices(3);
	bool flag, inside_flag=true;
	if ( poly.is_simple() )				//check if the original input is simple
	{
		flag = false;					// if yes- dont enter the loop
		simple_list.push_back ( poly );
	}
	else
		flag = true;

	while ( flag )
	{
		Polygon_2 poly1,poly2;
		tri[0] = tri_indices[ counter * 3 ];
		tri[1] = tri_indices[ counter * 3 + 1 ];
		tri[2] = tri_indices[ counter * 3 + 2 ];
		sort_and_check( tri, res_poly, original_indices );

		if ( tri[0] == -1 )
		{
			counter++;
			continue;
		}
		inside_flag = true;
		hope = 0;
		while ( inside_flag )
		{
			check_for_simple(simple_list,res_poly,tri,flag);
			if (!flag)
				break;
			if ( hope == 0 )
			{
				if ( tri[2] > tri[1]+1 )
				{
					sort_and_check( tri, res_poly, original_indices );
					swap(tri[0],tri[1]);
					swap(tri[1],tri[2]);
					hope++;
					continue;
				}
				else
					if ( tri[0] != (tri[2]+1)%res_poly.size() )
					{
						sort_and_check( tri, res_poly, original_indices );
						swap(tri[1],tri[2]);
						hope=2;
						continue;
					}
					else
						hope++;
			}

			if ( hope == 1 )
			{
				sort_and_check( tri, res_poly, original_indices );
				swap ( tri[1],tri[2] );
				if ( tri[0] != (tri[1]+1)%res_poly.size() )
					{
						hope=2;
						continue;
					}
				else
					hope = 2;
			}

			if ( hope == 2 )
				inside_flag = false;
			
		}

		counter++;
			
	}




	std::ofstream out("outSimple.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

	//std::cout  << simple_list.size() <<std::endl;

	for ( int i = 0 , len = (int)simple_list.size(); i < len ; ++i )
	{
		//std::cout  << simple_list[i].size() <<std::endl;

		for (int j = 0 , innerLen = (int)simple_list[i].size(); j < innerLen ; j++ )
		{
			std::cout  << simple_list[i][j][0] << " " << simple_list[i][j][1] <<std::endl;

		}

		if ( i == simple_list.size()-1 )
			std::cout  << "%" <<std::endl;
		else
			std::cout  << "$" <<std::endl;
	}

	/*
	bool triangulatePolygon(const std::vector<std::complex<double> >& polygonPoints, std::vector<std::complex<double> >& meshVertices,
						std::vector<unsigned int>& triangleIndices, std::vector<std::pair<int, std::complex<double> > >& boundaryVertices,
						double maxTriangleArea, bool subsampleBoundaryEdges)
	*/

	std::vector<std::complex<double>> polygonPoints , meshVertices;
	std::vector<unsigned int> triangleIndices;
	std::vector<std::pair<int, std::complex<double> > > boundaryVertices;
	double maxTriangleArea = 0.01;

	for ( int i = 0 , len = (int)simple_list[0].size() ; i < len ; i++ )
	{
		polygonPoints.push_back ( std::complex<double> ( simple_list[0][i][0] , simple_list[0][i][1] ) );
		boundaryVertices.push_back ( std::pair<int,std::complex<double>> ( i , polygonPoints[i] ) );
	}

	triangulatePolygon(polygonPoints,meshVertices,triangleIndices,boundaryVertices,maxTriangleArea,true);
	
}
