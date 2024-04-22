#include "../../includes/objects/material.hpp"

using namespace obj;

material::material()
{
	name = "";
	specularExponent = 0.0f;
	opticalDensity = 0.0f;
	dissolve = 0.0f;
	illumination = 0;
}

material::~material() {}
