#include <ctime>
#include <cstdlib>
#include <cmath>
#include "RandomServices.hpp"

static double firstNormal;
static double secondNormal;
static bool shouldGenerate;

/**
 * Provides a sequence-starter for the RNG's pseudorandom number sequence.
 */
void RandomServices::seed() {
    srandom(time(nullptr));
}

/**
 * Initialises the component of the RNG responsible for Gaussian values.
 */
void RandomServices::initialiseNormalNumberGenerator() {
    shouldGenerate = false;
    firstNormal = secondNormal = 0.0;
}

/**
 * Obtains a random integer within the RNG's generation boundaries.
 *
 * @return A random integer.
 */
double RandomServices::randomValue() {
    return (double)random() * (1.0 / RAND_MAX);
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

/**
 * Obtains a sample from the given Bernoulli distribution.
 *
 * @param p The Bernoulli distribution parameter.
 * @return The sample.
 */
bool RandomServices::bernoulliSample(double p) {
    return randomValue() <= p;
}

/**
 * Obtains a sample from the given discrete uniform distribution.
 *
 * Sampling starts from zero (inclusive), and ends at the specified ceiling.
 * Said ceiling is inclusive, too.
 *
 * @param ceiling The ceiling number of the distribution. Inclusive.
 * @return The sample.
 */
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
        isWithinRange = (range[0] <= random && range[1] > random);
        range[0] += 1.0; range[1] += 1.0;
    } while (!isWithinRange && range[0] <= (double)ceiling);
    return sample;
}

/**
 * Obtains a sample from the given discrete uniform distribution.
 *
 * Sampling starts from the minimum, which is inclusive, and ends at the
 * maximum. Said maximum is also inclusive.
 *
 * @param min The minimum integer that can be drawn. Inclusive.
 * @param max The maximum integer that can be drawn. Inclusive.
 * @return The sample.
 */
int RandomServices::discreteUniformSample(int min, int max) {
    return min + discreteUniformSample((max - min));
}

/**
 * Obtains a sample from the specified continuous uniform distribution.
 *
 * Sampling starts from zero (inclusive). The ceiling specifies the maximum
 * continuous value that can be drawn, and is also inclusive.
 *
 * @param ceiling The maximum continuous value that can be sampled. Inclusive.
 * @return The sample.
 */
double RandomServices::continuousUniformSample(double ceiling) {
    return ceiling * randomValue();
}

/**
 * Obtains a sample from the specified continuous uniform distribution.
 *
 * Sampling starts from the minimum, which is inclusive, and ends at the
 * maximum. Said maximum is also inclusive.
 *
 * @param min The minimum continuous value that can be drawn. Inclusive.
 * @param max The maximum continuous value that can be drawn. Inclusive.
 * @return The sample.
 */
double RandomServices::continuousUniformSample(double min, double max) {
    return min + continuousUniformSample((max - min));
}

/**
 * Initialises the RNG.
 *
 * Calling this method at the top of the main program is essential for this
 * class to work effectively.
 */
void RandomServices::initialiseRandomServices() {
    seed();
    initialiseNormalNumberGenerator();
}
