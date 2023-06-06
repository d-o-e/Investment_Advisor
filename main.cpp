/**
 *  Name: Deniz Erisgen
 *  Compiler: GCC
 */

#include "MutualFund.hpp"

using namespace std;

void shuffleNavHistoryFile(const string &filename);
void analyze2Funds(MutualFund &one, MutualFund &two);

int main() {
  string ticker1 = "VFIAX";
  string ticker2 = "VTSAX";

  //	TODO:Create 2 threads to process 2 items concurrently

  shuffleNavHistoryFile(ticker1);
  MutualFund vfiax(ticker1);
  vfiax.report();
  cout << endl;
  shuffleNavHistoryFile(ticker2);
  MutualFund vtsax(ticker2);
  vtsax.report();
  analyze2Funds(vfiax, vtsax);
  return 0;
}