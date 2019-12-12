#include <ctime>
#include <cstdlib>
#include <cmath>
#include "RandomServices.hpp"

void RandomServices::seed() {
    srand(time(nullptr));
}

void RandomServices::initialiseNormalNumberGenerator() {
    shouldGenerate = false;
    firstNormal = secondNormal = 0.0;
}

double RandomServices::randomValue() {
    return rand() * (1.0 / RAND_MAX);
}

/**
 * Obtains a sample from a normal distribution with the given mu and sigma.
 *
 * Mu is the Gaussian distribution's mean, and sigma its standard deviation.
 *   This method uses the Box-Muller transform, in which two 'proto-normal'
 * values are obtained per generation step. These proto-normal values are
 * then - given the arguments - transformed to proper Gaussian samples (thus,
 * the name).
 *   Code is an adaptation of that supplied on Wikipedia. See
 * https://en.wikipedia.org/wiki/Box–Muller_transform.
 *
 * @param mean The mean of the distribution.
 * @param standardDeviation The standard deviation of the distribution.
 * @return The distribution sample.
 */
double RandomServices::normalSample(double mean, double standardDeviation) {
    double firstFraction, secondFraction;
    shouldGenerate = !shouldGenerate;
    if (!shouldGenerate) {
        /* We have a second random normal value stored. Use that one. */
        return secondNormal * standardDeviation + mean;
    }
    do {
        /* Generate the random fractions; ensure they do not underflow. */
        firstFraction  = randomValue();
        secondFraction = randomValue();
    } while (firstFraction <= epsilon);
    /* Generate two random normal values; use only the first one. */
    firstNormal  = sqrt(-2.0 * log(firstFraction)) * cos(twoPi * secondFraction);
    secondNormal = sqrt(-2.0 * log(firstFraction)) * sin(twoPi * secondFraction);
    return firstNormal;
}

bool RandomServices::bernoulliSample(double p) {
    return randomValue() <= p;
}

int RandomServices::discreteUniformSample(int ceiling) {
    double random;
    double range[2] = {0.0, 1.0};
    int sample;
    bool isWithinRange;
    random = (ceiling + 1) * randomValue();
    sample = -1;
    do {
        /* Find the bin which 'contains' our random value. */
        sample++;
        isWithinRange = (range[0] >= random && range[1] < random);
        range[0] += 1.0; range[1] += 1.0;
    } while (isWithinRange || range[0] <= (double)ceiling);
    return sample;
}

int RandomServices::discreteUniformSample(int min, int max) {
    return min + discreteUniformSample((max - min));
}

double RandomServices::continuousUniformSample(double ceiling) {
    return ceiling * randomValue();
}

double RandomServices::continuousUniformSample(double min, double max) {
    return min + continuousUniformSample((max - min));
}

void RandomServices::initialiseRandomServices() {
    seed();
    initialiseNormalNumberGenerator();
}
