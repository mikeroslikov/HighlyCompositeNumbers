#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <math.h>


using namespace std;

inline bool IsPrime( unsigned long long number ) {
    if (((number-1)%6 == 0) || ((number+1)%6 == 0)) {
        bool is_prime = true;
        unsigned long long lim = sqrt(number)+1;
        #pragma omp parallel for
        for (unsigned long long i=2;i<lim;i+=1) {
            if (number%i == 0) {
                i = number+1;
                is_prime = false;
            }
        }
        return is_prime;
    } else {
        return false;
    }

}

int main() {




    unsigned long long prevAntiPrime = 0;
    unsigned long long antiPrime = 0;
    unsigned long long antiPrimeCount = 0;
    unsigned long long maxDivisors = 0;
    unsigned long long nextPrime = 5;
    unsigned long long incrementProduct = 6;

    ofstream file_antiprime_distribution("anti-prime_distribution.antiprimes", ios::app);
    ofstream file_antiprimes("anti-primes.antiprimes", ios::app);

    unsigned long long antiprime_max;
    string max = "10000";


    cout << "Roof of highly composite number search range? (-1 to exit)\n";
    cin >> antiprime_max;
    //antiprime_max = max;

    //The floor of the prime number search
    unsigned long long antiprime_min;
    string min = "";

    cout << "Floor of highly composite number search range?\n";
    cin >> antiprime_min;
    //antiprime_min = min;

    time_t start_milli;
    time(&start_milli);

    if (file_antiprime_distribution.is_open() && file_antiprimes.is_open()) {

        file_antiprime_distribution << endl;
        file_antiprimes << endl;

        for (unsigned long long i = antiprime_min; i < antiprime_max + 1; i+=incrementProduct) {

            unsigned long long divisors = 0;
            #pragma omp parallel for reduction (+:divisors)
            for (unsigned long long j = 2; j < (i/2)+2; j++) {
                if (i % j == 0) {
                    ++divisors;
                }
            }
            if (divisors > maxDivisors) {

                antiPrime = i;
                cout << antiPrimeCount << ":" << antiPrime << ";d:" << divisors <<"\n";
                file_antiprime_distribution << (antiPrime - prevAntiPrime) << ";" << divisors << ";"<< difftime(time(NULL),start_milli) <<"\n";
                file_antiprimes << antiPrime << "\n";
                prevAntiPrime = antiPrime;
                maxDivisors = divisors;

                //Update increment amount
                bool prime_not_divisible = false;
                while (!prime_not_divisible) {
                    if (i % nextPrime == 0) {
                        prime_not_divisible = false;
                        incrementProduct *= nextPrime;
                        bool found_next_prime = false;
                        unsigned long long try_for_prime = nextPrime + 2;

                        while (!found_next_prime) {
                            if (IsPrime(try_for_prime)) {
                                nextPrime = try_for_prime;
                                found_next_prime = true;
                                cout << "Found new prime. Now divisible by: " << try_for_prime << endl;
                            } else {
                                found_next_prime = false;
                            }
                            try_for_prime += 2;
                        }

                    } else {
                        prime_not_divisible = true;
                    }
                }

                antiPrimeCount+=1;

            }
        }
    } else {
        cout << "Unable to open file.";
    }

    file_antiprime_distribution.close();
    file_antiprimes.close();

    time_t now;
    time(&now);

    double duration = difftime(now,start_milli);

    cout << "Duration: " << duration << "\n";

    system("pause");

    return 0;
}