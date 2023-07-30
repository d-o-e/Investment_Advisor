/**
 *  Name: Deniz Erisgen
 *  Compiler: GCC
 */

#include "MutualFund.hpp"
#include <thread>
#include <iostream>

using namespace std;

void shuffleNavHistoryFile(const string &filename);

void analyze2Funds(MutualFund &one, MutualFund &two);

int main() {
    string ticker1 = "VFIAX";
    string ticker2 = "VTSAX";

    thread t1(&shuffleNavHistoryFile, ticker1);
    thread t2(&shuffleNavHistoryFile, ticker2);
    t1.join();
    t2.join();
    MutualFund vfiax(ticker1);
    MutualFund vtsax(ticker2);
    vtsax.report();
    vfiax.report();
    analyze2Funds(vfiax, vtsax);


    return 0;
}