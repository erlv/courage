/* POJ 1082 */
/* The problem is ask for a strategy to achieve the goal, it is not just a judgement */
/* So This code is wrong   */
#include<iostream>

#define TARG_YEAR 2001
#define TARG_MON 11
#define TARG_DAY 4

using namespace std;

int month_days[12]={ 31,  // Jan
		      28, // Feb
		      31, // Mar
		      30, // Apr
		      31, // May
		      30, // Jun
		      31, // Jul
		      31, // Aug
		      30, // Sep
		      31, // Oct
		      30, // Nov
		      31, // Dec
};

struct date {
  int year;
  int mon;
  int day;
};

bool is_leap_year( int year) {
  return ( year % 4 == 0 && year % 100 != 0 ) || (year % 400 == 0);
}

bool can_increase_month( struct date* curr_date) {
  bool result=false;
  
  if( (curr_date->mon == 1) && (curr_date->day == 29) && is_leap_year(curr_date->year) )
    result = true; 
  else if( month_days[(curr_date->mon+1)%12] >= curr_date->day )
    result = true;
  
  return result;
}

void increase_month( struct date* curr_date) {
  if(curr_date->mon == 12 ) {
    curr_date->year++;
    curr_date->mon = 1;
  }
  else
    curr_date->mon++;
}

void increase_day( struct date* curr_date ) {
  if(curr_date->day < month_days[(curr_date->mon)] )
    curr_date->day++;
  else if( curr_date->mon == 2 && curr_date->day == 28 && is_leap_year(curr_date->year))
    curr_date->day++;
  else if((curr_date->day == month_days[curr_date->mon]) 
    || (curr_date->mon == 2 && curr_date->day == 29 && is_leap_year(curr_date->year))) {
    curr_date->day = 1;
    increase_month(curr_date);
  }
  
}


bool will_win(int year, int mon, int day) {
  bool result;
  struct date curr;
  curr.year = year;
  curr.mon = mon;
  curr.day = day;
  
  while(1) {
    if(curr.year == TARG_YEAR && curr.mon == TARG_MON && curr.day == TARG_DAY) {
      result = true;
      break;
    }
    else if( curr.year > TARG_YEAR || ( curr.year == TARG_YEAR && curr.mon > TARG_MON )
      || (curr.year == TARG_YEAR && curr.mon == TARG_MON && curr.day > TARG_DAY)) {
	result = false;
	break;
      }
    else {
      if( can_increase_month( &curr) )
	increase_month(&curr);
      else
	increase_day(&curr);
    }
  }
  
  
  
  return result;
}

int main() {
  int tc;
  cin >> tc;
  
  int j;
  for(j=0; j < tc; j++) {
    int year, mon, day;
    cin >> year >> mon >> day;
    
    bool res = will_win(year, mon, day);
    if(res == true ) 
      cout << "YES" << endl;
    else
      cout << "NO" << endl;
  }
  return 0;
}