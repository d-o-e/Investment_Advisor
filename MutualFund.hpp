#ifndef MUTUALFUND_H
#define MUTUALFUND_H

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>

constexpr unsigned EPOCH_DAY = 86400;

using namespace std;

class Date {
  time_t date;

  public:
  Date() = default;
  explicit Date(time_t date): date(date) {}
  Date(const Date &rhs) { date = rhs.date; }
  explicit Date(const string &dateStr);
  friend ostream &operator<<(ostream &os, const Date &dat);
  Date &operator-=(const unsigned timeFrame) {
    this->date -= timeFrame;
    return *this;
  }
  bool operator==(const Date &rhs) const { return date == rhs.date; }
  bool operator!=(const Date &rhs) const { return !(rhs == *this); }
  bool operator<(const Date &rhs) const { return date < rhs.date; }
  bool operator>(const Date &rhs) const { return rhs < *this; }
  bool operator<=(const Date &rhs) const { return !(rhs < *this); }
  bool operator>=(const Date &rhs) const { return !(*this < rhs); }
  bool withinMonth(const Date &end) const {
    return ((difftime(end.date, date) / EPOCH_DAY) <= 30);
  }
  bool isValid(time_t cmp, unsigned minValidDays) const {
    return (difftime(date, cmp) > (minValidDays * EPOCH_DAY));
  }
  time_t getDate() const { return this->date; }
};

struct Results {
  Date buyDate;
  Date sellDate;
  float value;
  float gain;
  float returnOver;
  friend ostream &operator<<(ostream &os, const Results &results) {
    os << results.buyDate << "\t" << results.sellDate
       << " value: " << setprecision(3) << results.value
       << " gain: " << setprecision(3) << results.gain
       << " returnOver: " << results.returnOver;
    return os;
  }
};

class Dividend {
  Date date;
  float div;

  public:
  const Date &getDate() const { return date; }
  float getDiv() const { return div; }
  explicit Dividend(const string &data);
  friend ostream &operator<<(ostream &os, const Dividend &dividend);
  bool operator<(const Dividend &rhs) const { return date < rhs.date; }
  explicit Dividend(const Date &newdate): date(newdate), div(0) {};
};

struct MinMax {
  float min;
  float max;
};

struct Range {
  unsigned short start;
  unsigned short offset;
};

class MutualFund {
  string ticker;
  Date newestDate {};  // optional
  float newestNav;  // optional
  map<Date, float> navHistory;
  set<Dividend> divHistory;
  void processDivFile();
  void processNavFile();
  void writeToFile();

  public:
  MinMax daysMinMax(unsigned days);
  float movingAverage(unsigned days);
  void report();
  explicit MutualFund(string ticker);
  float calculateRandomReturns(Range dateRange);
  const string &getTicker() const { return ticker; }
  unsigned navSize() { return navHistory.size(); }
  unsigned divSize() { return divHistory.size(); }
  const map<Date, float> &navH() { return navHistory; };
  float calculateThreeYearReturn() const;
};

/**
 * Token struct for date and price
 */
struct Token {
  string first;
  string second;
};

#endif  // MUTUALFUND_H
