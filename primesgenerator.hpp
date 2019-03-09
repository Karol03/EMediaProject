#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>
#include <utility>
#include <vector>

#include <iostream>

template <typename T>
class RandomGenerator
{
public:
    T getRandom(T min, T max) const
    {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        std::uniform_int_distribution<T> dist(min, max);
        return dist(gen);
    }
};

template <typename T>
class Primes
{
public:
    Primes<T>() = default;
    Primes<T>(T max)
    {
        fillPrimes(max);
    }
    Primes<T>(const char* fN)
    {
        fillPrimesFromFile(fN);
    }

    T findPrimeNumber(T min, T max)
    {
        extractUniquePrimes();
        fillPrimes(max);

        auto min_it = std::find_if(primes.begin(), primes.end(),
                                   [&min](const T& x) { return x > min; });
        auto max_it = std::find_if(primes.begin(), primes.end(),
                                   [&max](const T& x) { return x > max; });
        T rand = randGenerator.getRandom(0, max_it-min_it-1);
        return *(min_it+rand);
    }

    std::pair<T, T> findPrimesPair(T min, T max)
    {
        T one = findPrimeNumber(min, max);
        T two = one;
        while (two == one)
            two = findPrimeNumber(min, max);
        return {one, two};
    }

    void extractUniquePrimes()
    {
        std::sort(primes.begin(), primes.end());
        std::unique(primes.begin(), primes.end());
    }

    void fillPrimes(T max)
    {
        if (std::any_of(primes.begin(), primes.end(),
                        [&max](const T& prime)
                        {   return prime > max; }))
            return;

        T newPrime = maxValue() + 2;
        while (newPrime < max)
        {
            for (auto prime : primes)
            {
                if (newPrime%prime == 0)
                    break;
                else if (std::sqrt(newPrime) < prime)
                {
                    primes.push_back(newPrime);
                    break;
                }
            }
            newPrime += 2;
        }
    }

    void fillPrimesFromFile(const char* fileName)
    {
        std::ifstream file(fileName);
        std::string word;
        if (file)
        {
            while (file >> word)
            {
                T number = static_cast<T>(word);
                primes.push_back(number);
            }
            file.close();
        }
    }

    const RandomGenerator<T>& randomGenerator() const
    {   return randGenerator;   }
private:
    T maxValue()
    {
        return *(std::max_element(primes.begin(), primes.end()));
    }

    RandomGenerator<T> randGenerator;
    std::vector<T> primes = {2, 3, 5};
};
