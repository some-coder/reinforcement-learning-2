#ifndef RANDOM_SERVICES_HPP
#define RANDOM_SERVICES_HPP

/* - Normal(mu, st.dev)
 * - Bernoulli(p)
 * - DiscreteUniform(0, l)
 * - ContinuousUniform(min, max)
 */

class RandomServices {
    private:
        constexpr static double epsilon = std::numeric_limits<double>::min();
        constexpr static double twoPi = 2.0 * 3.14159265358979323846;
        static double firstNormal;
        static double secondNormal;
        static bool shouldGenerate;
        static void seed();
        static void initialiseNormalNumberGenerator();

    public:
        static void initialiseRandomServices();
        static double randomValue();
        static double normalSample(double mean, double standardDeviation);
        static bool bernoulliSample(double p);
        static int discreteUniformSample(int ceiling);
        static int discreteUniformSample(int min, int max);
        static double continuousUniformSample(double ceiling);
        static double continuousUniformSample(double min, double max);
};

#endif