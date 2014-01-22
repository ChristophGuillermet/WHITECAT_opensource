#include "Point.hpp"
#include <sstream>


std::string ol::Point::
ToString() const {
   return "Point: " + pos.ToString();
}


void ol::Point::
TransformBy( const Placement &placement ) {
   pos += placement.GetPosition();
}
