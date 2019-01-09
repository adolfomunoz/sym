#include "../core/expression.h"
#include <random>

namespace sym {

enum Infinity { infinity };
enum MinusInfinity { minus_infinity };


float integral(const expression& f, const symbol& x, float a, float b, unsigned int nsamples = 100) {
	std::random_device seed;
	std::mt19937 random(seed());
   	std::uniform_real_distribution<float> dist(a,b);
	float sum(0);	
	for (unsigned int i = 0; i<nsamples; ++i) sum += f.substitute(x,dist(random)).evaluate();
	return sum/nsamples;	
}

float integral(const expression& f, const symbol& x, const MinusInfinity& a, const Infinity& b, unsigned int nsamples = 100)
{
	symbol t("t");
	return integral(f.substitute(x,t/(1-t*t))*(1+t*t)/pow(1-t*t,2),t,-0.999999f,0.999999f,nsamples);
}

float integral(const expression& f, const symbol& x, float a, const Infinity& b, unsigned int nsamples = 100)
{
	symbol t("t");
	return integral(f.substitute(x,a + t/(1-t))/pow(1-t,2),t,0,0.999999f,nsamples);
}

float integral(const expression& f, const symbol& x, const MinusInfinity& a, float b, unsigned int nsamples = 100)
{
	symbol t("t");
	return integral(f.substitute(x,b - (1 - t)/t)/pow(t,2),t,0,0.999999f,nsamples);
}



}
