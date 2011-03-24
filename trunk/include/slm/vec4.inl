inline vec4::vec4()
{
}

inline vec4::vec4( float v )
{
	assert( check(v) );
	m128() = SLMATH_LOAD_PS1(&v);
}

inline vec4::vec4( float x0, float y0, float z0, float w0 ) : 
	x(x0), y(y0), z(z0), w(w0) 
{
	assert( check(*this) );
}

inline vec4::vec4( const vec2& v0, float z0, float w0 ) :
	x(v0.x), y(v0.y), z(z0), w(w0) 
{
	assert( check(*this) );
}

inline vec4::vec4( const vec3& v0, float w0 ) :
	x(v0.x), y(v0.y), z(v0.z), w(w0) 
{
	assert( check(*this) );
}

inline void vec4::set( float x0, float y0, float z0, float w0 )
{
	x=x0;
	y=y0;
	z=z0;
	w=w0;
	assert( check(*this) );
}

inline vec4& vec4::operator+=( const vec4& o )
{
	m128() = SLMATH_ADD_PS( m128(), o.m128() );
	assert( check(*this) );
	return *this;
}

inline vec4& vec4::operator-=( const vec4& o )
{
	m128() = SLMATH_SUB_PS( m128(), o.m128() );
	assert( check(*this) );
	return *this;
}

inline vec4& vec4::operator*=( float s )
{
	m128() = SLMATH_MUL_PS( m128(), SLMATH_LOAD_PS1(&s) );
	assert( check(*this) );
	return *this;
}

inline vec4& vec4::operator*=( const vec4& o )
{
	m128() = SLMATH_MUL_PS( m128(), o.m128() );
	assert( check(*this) );
	return *this;
}

inline vec4& vec4::operator/=( const vec4& o )
{
	m128() = SLMATH_DIV_PS( m128(), o.m128() );
	assert( check(*this) );
	return *this;
}

inline vec4& vec4::operator/=( float s )
{
	assert( fabsf(s) >= FLT_MIN ); // s must be != 0
	
	const float invs = 1.f/s;
	m128() = SLMATH_MUL_PS( m128(), SLMATH_LOAD_PS1(&invs) );
	return *this;
}

inline vec4 vec4::operator/( float s ) const
{
	assert( fabsf(s) >= FLT_MIN ); // s must be != 0
	const float invs = 1.f/s;
	return vec4( SLMATH_MUL_PS( m128(), SLMATH_LOAD_PS1(&invs) ) );
}

inline float& vec4::operator[]( size_t i )
{
	assert( i < 4 );
	return (&x)[i];
}

inline vec3& vec4::xyz()
{
	return *(vec3*)&x;
}

inline const vec3& vec4::xyz() const
{
	return *(const vec3*)&x;
}

inline vec4 vec4::operator*( const vec4& o ) const
{
	assert( check(o) );
	assert( check(*this) );
	return vec4( SLMATH_MUL_PS( m128(), o.m128() ) );
}

inline vec4 vec4::operator/( const vec4& o ) const
{
	assert( check(*this) );
	assert( fabsf(o.x) > FLT_MIN );
	assert( fabsf(o.y) > FLT_MIN );
	assert( fabsf(o.z) > FLT_MIN );
	assert( fabsf(o.w) > FLT_MIN );
	return vec4( SLMATH_DIV_PS( m128(), o.m128() ) );
}

inline vec4 vec4::operator+( const vec4& o ) const
{
	assert( check(o) );
	assert( check(*this) );
	return vec4( SLMATH_ADD_PS( m128(), o.m128() ) );
}

inline vec4 vec4::operator-( const vec4& o ) const
{
	assert( check(o) );
	assert( check(*this) );
	return vec4( SLMATH_SUB_PS( m128(), o.m128() ) );
}

inline vec4 vec4::operator-() const
{
	assert( check(*this) );
	return vec4( SLMATH_SUB_PS( SLMATH_SETZERO_PS(), m128() ) );
}

inline vec4 vec4::operator*( float s ) const
{
	assert( check(s) );
	assert( check(*this) );
	return vec4( SLMATH_MUL_PS( m128(), SLMATH_LOAD_PS1(&s) ) );
}

inline const float& vec4::operator[]( size_t i ) const
{
	assert( i < 4 );
	return (&x)[i];
}

inline bool vec4::operator==( const vec4& o ) const
{
	return x==o.x && y==o.y && z==o.z && w==o.w;
}

inline bool vec4::operator!=( const vec4& o ) const
{
	return !(*this == o);
}

inline vec4 operator*( float s, const vec4& v )
{
	assert( check(s) );
	assert( check(v) );
	return vec4( SLMATH_MUL_PS( SLMATH_LOAD_PS1(&s), v.m128() ) );
}

inline float length( const vec4& v )
{
	const float res = sqrtf( v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w );
	assert( res >= 0.f && res <= FLT_MAX );
	return res;
}

inline float dot( const vec4& a, const vec4& b )
{
	const float res = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	assert( res >= -FLT_MAX && res <= FLT_MAX );
	return res;
}

inline vec4 max( const vec4& a, const vec4& b )
{
	return vec4( SLMATH_MAX_PS(a.m128(),b.m128()) );
}

inline vec4 min( const vec4& a, const vec4& b )
{
	return vec4( SLMATH_MIN_PS(a.m128(),b.m128()) );
}

inline vec4 abs( const vec4& v )
{
	return vec4( SLMATH_MAX_PS(SLMATH_SUB_PS(SLMATH_SETZERO_PS(),v.m128()),v.m128()) );
}

inline vec4 mix( const vec4& x, const vec4& y, float a )
{
	return vecMix( x, y, a );
}

inline float distance( const vec4& p0, const vec4& p1 )
{
	return length( p0 - p1 );
}

inline vec4 clamp( const vec4& v, const vec4& min, const vec4& max )
{
	return vecClamp( v, min, max );
}

inline vec4 saturate( const vec4& v )
{
	return vecSaturate( v );
}

inline bool check( const vec4& v )
{
#ifdef SLMATH_SIMD
	assert( 0 == (reinterpret_cast<size_t>(&v.x)&0xF) ); // 16B aligned?
#endif
	return check(v.x) && check(v.y) && check(v.z) && check(v.w);
}

inline vec4 neg( const vec4& a )
{
	return -a;
}

inline vec4 add( const vec4& a, const vec4& b )
{
	return a+b;
}

inline vec4 sub( const vec4& a, const vec4& b )
{
	return a-b;
}

inline vec4 mul( const vec4& a, float b )
{
	return a*b;
}

inline vec4 mul( float b, const vec4& a )
{
	return a*b;
}

// This file is part of 'slmath' C++ library. Copyright (C) 2009 Jani Kajala (kajala@gmail.com). See http://sourceforge.net/projects/slmath/
