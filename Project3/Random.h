#pragma once
#include <random>
#include <time.h>
using namespace std;

class Random {
	static std::mt19937 random;
public:
	static int Int(int min, int max);
	static float Float(float min, float max);
};

std::mt19937 Random::random(time(0));
int Random::Int(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(random);
}
float Random::Float(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random);
}