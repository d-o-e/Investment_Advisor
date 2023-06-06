#include "MutualFund.hpp"
#include <random>
#include <sstream>
#include <utility>

/**
 * Produces a random range
 * @param max limit inclusive
 * @param min limit inclusive
 * @return Range obj
 */
Range randomRange(unsigned short max, unsigned short min) {
  if (max == min) return Range({0, 0});
  Range temp {};
  unsigned randNum;
  random_device randomDevice;
  default_random_engine rEng(randomDevice());
  uniform_int_distribution<unsigned> dist(0, (max - min));
  randNum = dist(rEng);
  temp.start = randNum;
  dist = uniform_int_distribution<unsigned>(min, (max - randNum) - 1);
  randNum = dist(rEng);
  temp.offset = randNum;
  return temp;
}

/**
 * Extracts a specific column from csv line
 * @param line csv string
 * @param colNumber unsigned (default:1)
 * @return Token object with column 0 and column (colNumber)
 */
Token extractDataColumn(const string &line, unsigned colNumber = 1) {
  if (colNumber > 6) return Token({"", ""});
  Token data;
  istringstream stream(line);
  string str;
  getline(stream, str, ',');
  data.first = str;
  colNumber--;
  while (colNumber) {
    getline(stream, str, ',');
    colNumber--;
  }
  getline(stream, str, ',');
  data.second = str;
  return data;
}

/**
 * Date class constructor
 * @param dateStr date string
 */
Date::Date(const string &dateStr) {
  char n;
  tm timeParser {};
  istringstream dateParser(dateStr);
  dateParser >> timeParser.tm_year >> n >> timeParser.tm_mon >> n
    >> timeParser.tm_mday;
  timeParser.tm_year -= 1900;
  timeParser.tm_mon--;
  date = mktime(&timeParser);
}

ostream &operator<<(ostream &os, const Dividend &dividend) {
  os << dividend.date << " " << dividend.div;
  return os;
}

ostream &operator<<(ostream &os, const Date &date) {
  os << put_time(localtime(&(date.date)), "%m/%d/%Y");
  return os;
}

/**
 * MutualFund constructor with default processing
 * @param ticker name (String)
 */
MutualFund::MutualFund(string ticker): ticker(std::move(ticker)) {
  processNavFile();
  putchar('\n');
  processDivFile();
  putchar('\n');
  writeToFile();
  putchar('\n');
  newestNav = (--(navHistory.cend()))->second;
  newestDate = (--(navHistory.cend()))->first;
}

/**
 * Reading the (ticker)shuffled.csv file into a unique sorted map.
 */
void MutualFund::processNavFile() {
  string line;
  string navFile = "../res/" + this->ticker + "shuffled.csv";
  cout << "Reading file " << navFile << " into map ..." << endl;
  Token temp;
  ifstream fin(navFile);
  if (!fin) {
    cerr << "Can't find " << navFile << endl;
    exit(-1);
  }
  getline(fin, line);  // jump over column header assuming it is the first row
  while (fin) {
    if (fin.eof() || line.empty()) break;
    getline(fin, line);
    temp = extractDataColumn(line, 4);  // colNumber can be argument
    if (temp.first.empty()) break;
    navHistory.emplace(Date(temp.first), stof(temp.second));
  }
  fin.close();
  cout << (navHistory.size()) << " records read" << endl;
}

/**
 * Reading the (ticker)div.csv file into a unique sorted set.
 */
void MutualFund::processDivFile() {
  string line;
  string divFile = "../res/" + ticker + "div.csv";
  cout << "Reading file" << divFile << " into set ... " << endl;
  ifstream fin(divFile);
  if (!fin) {
    cerr << "Can't find " << divFile << endl;
    exit(-1);
  }
  while (fin) {
    if (fin.eof()) break;
    getline(fin, line);
    if (isalpha(line[0])) getline(fin, line);
    divHistory.emplace(line);
  }
  fin.close();
  cout << divHistory.size() << " records read" << endl;
}

/**
 * Writing to the (ticker)nav.txt (map) (ticker)div.txt (set) files
 */
void MutualFund::writeToFile() {
  string navFile = "../res/" + ticker + "nav.txt";
  string divFile = "../res/" + ticker + "div.txt";
  cout << "Writing Nav History data into file " << navFile << endl;
  ofstream fout(navFile);
  for (auto &item : navHistory)
    fout << item.first << "\t" << item.second << endl;
  fout.close();
  cout << "Writing Dividend History data into file " << divFile << endl;
  fout.open(divFile);
  for (auto &item : divHistory) fout << item << endl;
}

/**
 * Calculates min and max price in a period of time
 * @param days number of days
 * @return MinMax struck with min and max floats
 */
MinMax MutualFund::daysMinMax(unsigned days) {
  Date startDate = newestDate;
  startDate -= (EPOCH_DAY * days);
  auto navIter = navHistory.find(startDate);
  while (navIter == navHistory.end())
    navIter = navHistory.find(startDate -= EPOCH_DAY);
  navIter--;
  float tempLow = navIter->second;
  float tempHigh = navIter->second;
  while (navIter != navHistory.end()) {
    if (tempLow > navIter->second) tempLow = navIter->second;
    if (tempHigh < navIter->second) tempHigh = navIter->second;
    navIter++;
  }
  MinMax lastXdays {};
  lastXdays.min = tempLow;
  lastXdays.max = tempHigh;
  return lastXdays;
}

/**
 * Calculates moving average in a period of days
 * @param days number of days
 * @return average (float)
 */
float MutualFund::movingAverage(unsigned days) {
  float average = 0.0f;
  unsigned i = days;
  auto navIt = navHistory.crbegin();
  navIt++;
  while (i > 0) {
    navIt++;
    average += navIt->second;
    i--;
  }
  return (average / static_cast<float>(days));
}

/**
 * Divident constructor
 * @param data string including date and price
 */
Dividend::Dividend(const string &data) {
  stringstream stream(data);
  string token;
  getline(stream, token, ',');
  date = Date(token);
  getline(stream, token, '\n');
  div = stof(token);
}

/**
 * Shuffling the data to use unordered sets
 * @param filename filename without extension
 */
void shuffleNavHistoryFile(const string &filename) {
  unordered_set<string> readData;
  string line;
  string header;
  string readFile = "../res/" + filename + ".csv";
  string writeFile = "../res/" + filename + "shuffled.csv";
  cout << "Reading " << readFile << " into unordered set ..." << endl;
  ifstream fin(readFile);
  if (!fin) {
    cerr << "Can't find " << readFile << endl;
    exit(-1);
  }
  getline(fin, header);  // jump over column header assuming it is the first row
  while (fin) {
    if (fin.eof()) break;
    getline(fin, line);
    readData.insert(line);
  }
  cout << "Input data stored in " << readData.size() << " buckets" << endl;
  ofstream fout(writeFile);
  if (!fout) {
    cerr << "File out problem" << endl;
    exit(-1);
  }
  fout << header << endl;
  for (auto &item : readData) fout << item << endl;
  cout << "Writing unordered_set<string> data into file " << writeFile << endl;
  putchar('\n');
}

/**
 * Calculates random returns based on requirments
 * invest in $10.000 for at least a month
 * @param dateRange (Range)
 * @return gained amount (float)
 */
float MutualFund::calculateRandomReturns(Range dateRange) {
  Results calculated {};
  auto it = navHistory.begin();
  while (dateRange.start > 0) {
    it++;
    dateRange.start--;
  }
  calculated.buyDate = it->first;
  auto end = it;
  while (dateRange.offset > 0) {
    end++;
    dateRange.offset--;
  }
  calculated.sellDate = end->first;
  string detailFile = "res/dividend_detail.txt";
  ofstream fout(detailFile, ios_base::app);
  float capital = 10000;
  float price = it->second;
  float shares = (capital / price);
  auto divIt = divHistory.lower_bound(Dividend(calculated.buyDate));
  if (calculated.buyDate.withinMonth(divIt->getDate())) divIt++;
  fout
    << "Dividends for " << ticker << " :" << calculated.buyDate << " - "
    << calculated.sellDate << "\n"
    << fixed << setw(80) << setfill('-') << "\n"
    << "\tDate\t Dividend \t Shares \t Div Am \t Price \t Purchase \t New Owned"
    << setfill(' ') << endl;
  auto lastDiv = divHistory.upper_bound(Dividend(calculated.sellDate));
  while (divIt != lastDiv) {
    if (divIt == divHistory.end()) break;
    float divAmount = shares * divIt->getDiv();
    float dayPrice = navHistory.find(divIt->getDate())->second;
    float newShares = divAmount / dayPrice;
    fout << fixed << showpoint << setprecision(3) << setw(10)
         << divIt->getDate() << setw(10) << divIt->getDiv() << setw(11)
         << shares << setw(12) << setprecision(2) << divAmount << setw(11)
         << dayPrice << setw(10) << setprecision(3) << newShares;
    shares += newShares;
    fout << setw(13) << shares << endl;
    divIt++;
  }
  it = navHistory.find(calculated.sellDate);
  if (it == navHistory.end()) it--;
  fout << fixed << setw(80) << setfill('-') << "\n";
  calculated.value = shares * (it->second);
  calculated.gain = calculated.value - capital;
  cout << fixed << showpoint << setprecision(2) << setw(13) << calculated.value
       << " \t " << setw(10) << calculated.gain;
  return calculated.gain;
}

/**
 * Prints processed reports
 * moving average of 50 and 200 days
 */
void MutualFund::report() {
  auto lastYHL = daysMinMax(365);
  auto average1 = movingAverage(50);
  auto average2 = movingAverage(200);
  cout << setprecision(6) << showbase << "Fund:" << ticker << "\n"
       << "Analysis Date: " << newestDate << "\n"
       << "Latest NAV: $" << newestNav << "\n"
       << "Minimum for the last year = $" << lastYHL.min << "\n"
       << "Maximum for the last year = $" << lastYHL.max << "\n"
       << setprecision(5) << "50 day moving average = $" << average1 << "\n"
       << "200 day moving average = $" << average2 << "\n"
       << setprecision(3) << "% gain over 200 Day Moving Avg: "
       << ((newestNav - average2) / average2) * 100 << "%\n"
       << "% return over the last 3 years: " << setprecision(4)
       << calculateThreeYearReturn() << "%\n";
}

/**
 * Analyze given fund objects
 * @param one reference to one MutualFund
 * @param two reference to another MutualFund
 */
void analyze2Funds(MutualFund &one, MutualFund &two) {
  float gainOne, gainTwo;
  float sumOne = 0;
  float sumTwo = 0;
  cout << endl;
  cout << " Buy Date \tSell Date \t" << one.getTicker() << " Value \t  "
       << one.getTicker() << " Gain  " << two.getTicker() << " Value \t "
       << two.getTicker() << " Gain \t " << one.getTicker() << "-"
       << two.getTicker() << "\n";
  for (int i = 0; i < 100; i++) {
    auto itOne = one.navH().begin();
    auto itTwo = two.navH().begin();
    auto range = randomRange(one.navSize(), 250);
    if (range.start == 0) {
      cerr << "random range problem";
      exit(-1);
    }
    for (int j = 0; j < range.start; j++) {
      itOne++;
      itTwo++;
    }
    cout << itOne->first;
    for (int j = 0; j < range.offset; j++) {
      itOne++;
      itTwo++;
    }
    cout << "\t" << itOne->first;
    gainOne = one.calculateRandomReturns(range);
    sumOne += gainOne;
    gainTwo = two.calculateRandomReturns(range);
    sumTwo += gainTwo;
    cout << fixed << showpoint << setprecision(2) << setw(13)
         << gainOne - gainTwo << endl;
  }
  cout << endl;
  cout << "Average gain for " << one.getTicker() << " " << sumOne / 100 << " %"
       << endl;
  cout << "Average gain for " << two.getTicker() << " " << sumTwo / 100 << " %"
       << endl;
}

/**
 * Calculate a 3 year return with $10.000 invested
 * @return gained amount (float) %
 */
float MutualFund::calculateThreeYearReturn() const {
  auto start = this->newestDate;
  auto navIt = navHistory.find(start -= (3 * 365 * EPOCH_DAY));
  while (navIt == navHistory.end()) navIt = navHistory.find(start -= EPOCH_DAY);
  navIt--;
  auto buyDate = navIt->first;
  float capital = 10000;
  float price = navIt->second;
  float shares = (capital / price);
  auto divIt = divHistory.lower_bound(Dividend(buyDate));
  if (buyDate.withinMonth(divIt->getDate())) divIt++;
  while (divIt != divHistory.end()) {
    float divAmount = shares * divIt->getDiv();
    float dayPrice = navHistory.find(divIt->getDate())->second;
    float newShares = divAmount / dayPrice;
    shares += newShares;
    divIt++;
  }
  auto value = shares * newestNav;
  auto gain = value - capital;
  auto returnOver = ((gain / capital) * 100);
  return returnOver;
}