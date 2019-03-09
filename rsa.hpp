#pragma once

#include <utility>
#include <vector>
#include <exception>

#include "primesgenerator.hpp"


template<typename T>
class RSA final
{
public:
    RSA() : primeGenerator(Primes<T>())
    {}

    RSA(Primes<T> primeGenerator_)
        : primeGenerator(std::move(primeGenerator_))
    {}

    RSA(Primes<T> primeGenerator_, T min, T max)
        : primeGenerator(std::move(primeGenerator_))
    {   createKeys(min, max);   }

    RSA(T min, T max)
        : primeGenerator(Primes<T>())
    {   createKeys(min, max);   }

    void createKeys(T min = 3, T max = 100)
    {
        auto primes = primeGenerator.findPrimesPair(min, max);
        auto phiN = phi(primes.first, primes.second);
        auto n = primes.first*primes.second;
        auto e = findE(n, phiN);
        auto d = findInverseModulo(e, phiN);
        publicKey = {e, n};
        privateKey = {d, n};
    }

    std::pair<T, T> getPublicKey()
    {   return publicKey;   }

    std::pair<T, T> getPrivateKey()
    {   return privateKey;   }

    std::vector<T> encrypt(const std::vector<T>& msg)
    {
        T e = publicKey.first;
        T n = publicKey.second;
        std::vector<T> encryptedMsg;
        encryptedMsg.reserve(msg.size());
        for (const auto& m : msg)
            encryptedMsg.push_back(getResult(m, e, n));
        return encryptedMsg;
    }

    std::vector<T> decrypt(const std::vector<T>& msg)
    {
        T d = privateKey.first;
        T n = privateKey.second;
        std::vector<T> decryptedMsg;
        decryptedMsg.reserve(msg.size());
        for (const auto& m : msg)
            decryptedMsg.push_back(getResult(m, d, n));
        return decryptedMsg;
    }

    T encrypt(T msg)
    {
        T e = publicKey.first;
        T n = publicKey.second;
        return getResult(msg, e, n);
    }

    T decrypt(T msg)
    {
        T d = privateKey.first;
        T n = privateKey.second;
        return getResult(msg, d, n);
    }

private:
    T getResult(T msg, T p, T m)
    {
        if (p==1)
            return msg;
        else if (p==0)
            return 1;

        if (p%2)
            return (getResult(msg, p-1, m)*msg) % m;
        else
        {
            T result = getResult(msg, p/2, m);
            return (result*result) % m;
        }
    }

    T phi(T P, T Q)
    {   return (P-1)*(Q-1);   }

    T getRandom(T min, T max)
    {
        return primeGenerator
                .randomGenerator()
                .getRandom(min, max);
    }

    T NWD(T first, T second)
    {
        if (first == 0) return second;
        if (second == 0) return first;
        while (second != 0)
        {
            T t = second;
            second %= first;
            first = t;
        }
        return first;
    }

    T findE(T N, T phi)
    {
        T e;
        do
        {
            e = getRandom(1, N);
        } while (not e%2 or (NWD(e, phi) != 1));
        return e;
    }

    void applyE(T e)
    {   publicKey.second = e;   }

    T findInverseModulo(T e, T phiN)
    {
        T u = 1;
        T w = e;
        T x = 0;
        T z = phiN;
        T q;

        while(w)
        {
          if(w < z)
          {
            q = u; u = x; x = q;
            q = w; w = z; z = q;
          }
          q = w / z;
          u -= q * x;
          w -= q * z;
        }
        if(z == 1 and x < 0)
              return (x + phiN);
        return 0;
    }


    Primes<T> primeGenerator;
    std::pair<T, T> publicKey;
    std::pair<T, T> privateKey;
};
