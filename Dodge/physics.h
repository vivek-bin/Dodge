#ifndef PHYSICS_FUNCTIONS
#define PHYSICS_FUNCTIONS
bool bubbleCollision(shape_Object const * const,shape_Object const * const);
bool cuboidCollision(shape_Object const * const,shape_Object const * const);
bool simpleCuboidCollision(shape_Object const * const,shape_Object const * const);
bool simpleCuboidPointCollision(shape_Object const * const,float const []);

#endif // PHYSICS_FUNCTIONS

