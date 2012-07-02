#ifndef STRUCT_H
#define STRUCT_H
#include <vector>
#include <string>

struct Vector2
{
	double x;
	double y;
};

struct Vector
{
	double x;
	double y;
	double z;
};

struct HeaderInfo
{
	int part0;
	int part1;
	int part2;
	int part3;
};
struct Layer
{
	unsigned int grains;
	double zLayer;
};

struct Grain
{
	unsigned short area;
	double x;
	double y;
	double z;
};

struct Area //catalog info
{
	double xMin, xMax, yMin, yMax;
	double xStep, yStep;
	unsigned int nFragments;
};

struct KeyStringRepresentation
{
	std::string name;
	std::string value;
};

struct ConfigStringRepresentation
{
	std::string className;
	std::string name;
	std::vector<KeyStringRepresentation> keys;
	unsigned int length;
};

struct SetupStringRepresentation
{
	std::string name;
	std::vector<ConfigStringRepresentation> configs;
	unsigned int length;
};

#endif
