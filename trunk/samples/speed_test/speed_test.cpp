// Note: This sample compiles on Windows only due to dependency to QueryPerformanceCounter

// To enable Eigen testing, uncomment following line:
//#define TEST_EIGEN

#include <stdio.h>
#include <slmath/slmath.h>

// MSVC doesnt have stdint.h
#ifdef _MSC_VER
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#ifdef TEST_EIGEN
#define EIGEN_SSE2_ON_MSVC_2008_OR_LATER
#include <Eigen/Core>
#endif

static double s_freqInv = 1.0;

static uint64_t now( bool init=false )
{
	if ( init )
	{
		uint64_t freq;
		QueryPerformanceFrequency( (LARGE_INTEGER*)&freq );
		s_freqInv = 1.0 / double(freq);
	}
	uint64_t out; 
	QueryPerformanceCounter( (LARGE_INTEGER*)&out ); 
	return out;
}

static float tick()
{
	static uint64_t t0 = 0;
	if ( !t0 )
		t0 = now();
	uint64_t t1 = now();
	uint64_t dt = t1-t0;
	t0 = t1;
	return float( double(dt)*s_freqInv );
}


SLMATH_USING()
#ifdef TEST_EIGEN
using namespace Eigen;
#endif

template <class T> float det4( const T& m )
{
	return
		m(0,3) * m(1,2) * m(2,1) * m(3,0) - m(0,2) * m(1,3) * m(2,1) * m(3,0) - m(0,3) * m(1,1) * m(2,2) * m(3,0)+m(0,1) * m(1,3) * m(2,2) * m(3,0) +
		m(0,2) * m(1,1) * m(2,3) * m(3,0) - m(0,1) * m(1,2) * m(2,3) * m(3,0) - m(0,3) * m(1,2) * m(2,0) * m(3,1)+m(0,2) * m(1,3) * m(2,0) * m(3,1) +
		m(0,3) * m(1,0) * m(2,2) * m(3,1) - m(0,0) * m(1,3) * m(2,2) * m(3,1) - m(0,2) * m(1,0) * m(2,3) * m(3,1)+m(0,0) * m(1,2) * m(2,3) * m(3,1) +
		m(0,3) * m(1,1) * m(2,0) * m(3,2) - m(0,1) * m(1,3) * m(2,0) * m(3,2) - m(0,3) * m(1,0) * m(2,1) * m(3,2)+m(0,0) * m(1,3) * m(2,1) * m(3,2) +
		m(0,1) * m(1,0) * m(2,3) * m(3,2) - m(0,0) * m(1,1) * m(2,3) * m(3,2) - m(0,2) * m(1,1) * m(2,0) * m(3,3)+m(0,1) * m(1,2) * m(2,0) * m(3,3) +
		m(0,2) * m(1,0) * m(2,1) * m(3,3) - m(0,0) * m(1,2) * m(2,1) * m(3,3) - m(0,1) * m(1,0) * m(2,2) * m(3,3)+m(0,0) * m(1,1) * m(2,2) * m(3,3);
}

float ops( float t, size_t n )
{
	assert( t > FLT_MIN );
	return float(n)/float(t);
}

// Y += alpha * X
void test1()
{
#ifdef _DEBUG
	const size_t n_reps = 10000000;
#else
	const size_t n_reps = 100000000;
#endif

	printf( "--------------------------------------------------------------\n" );
	printf( "Y += alpha * X\n--------------------------------------------------------------\n" );
	for ( int n = 0 ; n < 3 ; ++n )
	{
		vec4 y0 = vec4(0);
		const vec4 x0 = vec4(1.0012345f);
		const float alpha0 = 1.000001f;
		tick();
		for ( size_t i = 0 ; i < n_reps ; ++i )
		{
			y0 += alpha0 * x0;
		}
		float time0 = tick();
		printf( "  ops (slmath) = %g (res=%g)\n", ops(time0,n_reps), length(y0) );

#ifdef TEST_EIGEN
		Vector4f y1;
		y1[0] = 0.f;
		y1[1] = 0.f;
		y1[2] = 0.f;
		y1[3] = 0.f;
		Vector4f x1;
		x1[0] = 1.0012345f;
		x1[1] = 1.0012345f;
		x1[2] = 1.0012345f;
		x1[3] = 1.0012345f;
		const float alpha1 = 1.000001f;
		tick();
		for ( size_t i = 0 ; i < n_reps ; ++i )
		{
			y1 += alpha1 * x1;
		}
		float time1 = tick();
		printf( "  ops (Eigen) = %g (res=%g)\n", ops(time1,n_reps), sqrtf(y1.dot(y1)) );
#endif
	}
}

// matrix * matrix
void test2()
{
#ifdef _DEBUG
	const size_t n_reps = 200000;
#else
	const size_t n_reps = 2000000;
#endif

	printf( "--------------------------------------------------------------\n" );
	printf( "matrix * matrix\n--------------------------------------------------------------\n" );
	for ( int n = 0 ; n < 3 ; ++n )
	{
		const mat4 a0 = rotationX( 1.0012345f ) * rotationY( 1.0012345f );
		mat4 b0 = a0;

		tick();
		for ( size_t i = 0 ; i < n_reps ; ++i )
		{
			b0 = a0 * b0 * a0 * b0;
		}
		float time0 = tick();
		printf( "  ops (slmath) = %g (res=%g)\n", ops(time0,n_reps), det(b0) );

#ifdef TEST_EIGEN
		Matrix4f a1;
		Matrix4f b1;
		for ( int i = 0 ; i < 4 ; ++i )
			for ( int j = 0 ; j < 4 ; ++j )
			{
				a1(i,j) = a0[i][j];
				b1(i,j) = a0[i][j];
			}
		tick();
		for ( size_t i = 0 ; i < n_reps ; ++i )
		{
			b1 = a1 * b1 * a1 * b1;
		}
		float time1 = tick();
		printf( "  ops (Eigen) = %g (res=%g)\n", ops(time1,n_reps), det4(b1) );
#endif
	}
}

int main( int argc, char* argv[] )
{
	if ( !isValidCPU() )
	{
		printf( "slmath was compiled for CPU features not supported by this platform\n" );
		return 1;
	}

	now(true);
	test1();
	test2();
	return 0;
}

