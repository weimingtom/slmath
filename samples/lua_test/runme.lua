print( "slmath: Running unit tests" );
assert( slmath.clamp(-1,0,1) == 0 );
assert( slmath.clamp(2,0,1) == 1 );
assert( slmath.clamp(0.5,0,1) == 0.5 );

v = slmath.vec2(1,2)
assert( v.x == 1 );
assert( v.y == 2 );

r = slmath.random();
print( "randomFloat: " .. r:randomFloat() );

print( "slmath: OK" );
