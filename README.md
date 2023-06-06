# Analyzing two mutual funds and produce reports that will aid in a recommendation to a customer.

**Compares the two mutual funds by evaluating the performance of the funds over 100 random time periods. Dividends are
reinvested. Note, the fund must be held for at least 30 days before a dividend is paid. Determines the gain of each
fund using an initial investment of $10,000 during the time period after applying the dividends awarded during the
period. Finally, it determines the average gains for both funds over the 100 random time periods.**

- The 50-day moving average is the average over the last 50 market offsetDays.
- The 200-day moving average is the average over the last 200 market offsetDays.
- The % gain over 200-Day Moving Avg is the percent gain of the Latest Close over the 200 day moving average.
- The gain over the last 3 years is the difference between the current value after applying dividends and your initial
  $10000 investment.
- The % gain is the gain divided by the initial investment, then expressed as a percent. To determine the last 3 years,
  subtract 3 from the year of the analysis date.

NAV => Net Asset Value = the value of one share of a mutual fund on a particular date

Input Files: Mutual Fund history data (9/1/2004 to 12/28/2020)

VFIAX.csv--NAV history file

VTSAX.csv--NAV history file

VFIAXdiv.csv--dividend history file

VTSAXdiv.csv--dividend history file

```
	VFIAXnav.txt                  VFIAXdiv.txt
	
	09/01/2004    102.52          09/24/2004    0.431
	09/02/2004    103.67          12/23/2004    0.858
	09/03/2004    103.23          03/23/2005    0.454
	09/07/2004    103.95          06/24/2005    0.444
	09/08/2004    103.50          09/23/2005    0.557
	09/09/2004    103.70          12/28/2005    0.627
	09/10/2004    104.21          03/17/2006    0.516
	09/13/2004    104.42          06/23/2006    0.509
	09/14/2004    104.65          09/22/2006    0.547
	09/15/2004    103.91          12/26/2006    0.680
		
```

For example, if the analysis date is 4/6/20, use 4/6/17 as a day date for the 3 year period. If 4/6/17 is not a market
day, use the next market day after that date.

## Program Output

```
Reading ../res/VFIAX.csv into unordered set ...
Input data stored in 4151 buckets
Writing unordered_set<string> data into file ../res/VFIAXshuffled.csv

Reading file ../res/VFIAXshuffled.csv into map ...
4151 records read

Reading file../res/VFIAXdiv.csv into set ...
66 records read

Writing Nav History data into file ../res/VFIAXnav.txt
Writing Dividend History data into file ../res/VFIAXdiv.txt

Fund:VFIAX
Analysis Date: 02/26/2021
Latest NAV: $352.54
Minimum for the last year = $206.42
Maximum for the last year = $363.71
50 day moving average = $351.21
200 day moving average = $317.87
% gain over 200 Day Moving Avg: 10.9%
% return over the last 3 years: 44.42%

Reading ../res/VTSAX.csv into unordered set ...
Input data stored in 4151 buckets
Writing unordered_set<string> data into file ../res/VTSAXshuffled.csv

Reading file ../res/VTSAXshuffled.csv into map ...
4151 records read

Reading file../res/VTSAXdiv.csv into set ...
66 records read

Writing Nav History data into file ../res/VTSAXnav.txt
Writing Dividend History data into file ../res/VTSAXdiv.txt

Fund:VTSAX
Analysis Date: 02/26/2021
Latest NAV: $97.44
Minimum for the last year = $54.49
Maximum for the last year = $100.94
50 day moving average = $96.724
200 day moving average = $85.745
% gain over 200 Day Moving Avg: 13.6%
% return over the last 3 years: 48.01%

 Buy Date       Sell Date       VFIAX Value       VFIAX Gain  VTSAX Value        VTSAX Gain      VFIAX-VTSAX
07/22/2019      01/05/2021     12826.37             2826.37     13067.04            3067.04      -240.67
06/17/2015      03/21/2019     14604.52             4604.52     14345.77            4345.77       258.75
04/10/2015      01/09/2018     13856.10             3856.10     13622.25            3622.25       233.85

.....

08/01/2013      11/24/2017     16669.82             6669.82     16434.20            6434.20       235.62
09/28/2007      02/04/2015     15688.20             5688.20     16185.27            6185.27      -497.07

Average gain for VFIAX 6900.23 %
Average gain for VTSAX 6968.21 %

Process finished with exit code 0

```
