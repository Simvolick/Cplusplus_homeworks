/**
* MFE C++ 2022 - Assignment 4
*
* Please see the assignment writeup for an explanation of the Car, Driver and Race classes as well as 
* your coding and submission requirements.
*/

#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>

#include <ctime>
#include <list>
#include <numeric>
#include <algorithm>
#include <vector>
#include <array>
#include <map>


using namespace std;



const bool debug = false;

// A helper function to generate a uniform random number in [0, 1)
double genUniformRand() {
	static std::mt19937 mt_rand(4321);
	static std::uniform_real_distribution<double> dis_unif(0.0, 1.0);
	return dis_unif(mt_rand);
}


/*
  My Random generator algorithm.
*/

// To generate random number
// between x and y ie.. [x, y]
int getRandom(int x, int y)
{
    srand(time(NULL));
    return (x + rand() % (y-x+1));
}
 
// A recursive randomized binary search function.
// It returns location of x in
// given array arr[l..r] is present, otherwise -1
int randomizedBinarySearch(int arr[], int l,
                            int r, int x)
{
    if (r >= l)
    {
        // Here we have defined middle as
        // random index between l and r ie.. [l, r]
        int mid = getRandom(l, r);
 
        // If the element is present at the
        // middle itself
        if (arr[mid] == x)
            return mid;
 
        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (arr[mid] > x)
          return randomizedBinarySearch(arr, l,
                                    mid-1, x);
 
        // Else the element can only be present
        // in right subarray
        return randomizedBinarySearch(arr, mid+1,
                                         r, x);
    }
 
    // We reach here when element is not present
    // in array
    return -1;

}


/*
  IMPLEMENT YOUR Car AND  Driver CLASSES  HERE
*/
class Car {
  protected:
    double TireWear;

  public:
  	Car() {     // Constructor
      TireWear = 1.0;
    }
    double getTireWear()  {
      return TireWear;
    }
	void setTireWear(double tw) {
      TireWear = tw;
    }
};


class Driver {
  protected:
    Car* MyCar;
	double Speed;
	double TireWearThreshold;
	bool HasPitted;

  public:
  	Driver(Car* car, double speed, double tirewear_threshold) {     // Constructor
	  MyCar = car;
	  Speed = speed;
	  TireWearThreshold = tirewear_threshold;
	  HasPitted = false;
    }

	double getHasPitted()  {
      return HasPitted;
    }
	void setHasPitted(bool pitted) {
      HasPitted = pitted;
    }
	double getSpeed()  {
      return Speed;
    }
	void setSpeed(double s) {
      Speed = s;
    }
	Car* getCar()  {
      return MyCar;
    }
	void setCar(Car* c) {
      MyCar = c;
    }
	double getTireWearThreshold()  {
      return TireWearThreshold;
    }
	void setTireWearThreshold(double twt) {
      TireWearThreshold = twt;
    }
};




/*
  The Race class manages the race simulation.  You do not need to change the code in this class.
*/
class Race {
protected:
	// Race class properties

	int Laps;             // the number of laps in the race
	double LapLength;     // the time, in seconds, it takes to drive a lap at FULL speed
	double PitStopDelay;  // the time, in seconds, it takes to stop in the pits and change tires

	Driver* Driver1;      // a driver in the race
	Driver* Driver2;      // the other driver in the race


	// Race class methods not accessible publicly

	/*
	* calcLapTime - estimates the lap time of a driver if there were no other car on the track.
	* The lap time is the sum of the LapLength (the full-speed time of the lap) adjusted by the speed the driver
	* chooses and the tire wear of the car and a random adjustment based also on the speed of the drive.
	*
	* Further, the driver may choose to take a pit stop to change tires at the end of the lap.
	*
	* Finally, tire wear is adjusted based on the speed of the driver.
	*
	* calcLapTime returns the elapsed time for the driver for a single lap.
	*/
	double calcLapTime(Driver* driver) {
		Car* car = driver->getCar();
		double speed = driver->getSpeed();
		if (speed > 1) {
			speed = 1;
		}

		if (speed < 0.5) {
			speed = 0.5;
		}

		bool take_pit_stop = false;
		bool driver_has_pitted = driver->getHasPitted();

		if (!driver_has_pitted) {
			take_pit_stop = driver->getTireWearThreshold() > car->getTireWear() ? true : false;
			driver->setHasPitted(true);
		}

		double tw = car->getTireWear();

		double random_drive_adjustment = 200.0 * genUniformRand() * (speed - 0.95);

		double d_time = LapLength / (0.75 + 0.25 * tw) / speed + random_drive_adjustment;

		// tire wear degrades by 1 + speed/100
		car->setTireWear(tw / (1.0 + speed / 100.0));

		// pit stop comes after the lap
		if (take_pit_stop) {
			car->setTireWear(1.0);
			d_time += PitStopDelay;
		}


		return d_time;
	}

	/**
	* getLaggardElapsedTime - returns the elapsed time for the car that is not in the lead. If the laggard's lap time
	*                         is less (faster) than the leader's lap time, then there is the possibility that the laggard
	*                         will overtake the leader.  This is not guaranteed though and, if the overtake is unsuccessful,
	*                         the laggard will be 0.25 seconds behind the leader (an arbitrary number).
	*
	* Parameters:
	*     leader_elapsed_time_full - after the lap, the total elapsed time the leader has spent racing
	*     laggard_elapsed - the total elapsed time the laggard has spend racing BEFORE the current lap
	*     laggard_lap_time - the PROPOSED lap time for the laggard based on speed/tire wear/etc.
	*
	* The basic logic for determining the elapsed time.
	*      if laggard_elapsed + laggard_lap_time > leader_elapsed_time_full then
	*          no overtaking was attempted.  laggard_elapsed_time_full = laggard_elapsed + laggard_lap_time
	*
	*      else if laggard_elapsed + laggard_lap_time + pit stop delay - 5 seconds < leader_elapsed_time_full  then
	*          leader took a pit stop and laggard did not, laggard_elapsed_time_full = laggard_elapsed + laggard_lap_time
	*
	*      else if potential laggard lead after lap > 0 then
	*
	*          probability of overtake = potential_lead / (5.0 + potential_lead)  thus the bigger the lead the greater then probability
	*          if random uniform < probability then laggard takes the lead
	*             laggard_elapsed_time_full = laggard_elapsed + laggard_lap_time
	*
	*          else
	*             overtaking failed and laggard is 0.25 seconds behind leader
	*             laggard_elapsed_time_full = leader_elapsed_time_full + 0.25
	*
	*
	*  getLaggardElapsedTime returns the full elapsed race time for the laggard after the lap is complete
	*
	*/
	double getLaggardElapsedTime(double leader_elapsed_full, double laggard_elapsed, double laggard_lap_time) {
		if (laggard_elapsed + laggard_lap_time < leader_elapsed_full) {
			// attempted overtaking
			double potential_lead = leader_elapsed_full - laggard_elapsed - laggard_lap_time;
			double probability_of_overtake = potential_lead / (5.0 + potential_lead);
			if ((genUniformRand() < probability_of_overtake) || (potential_lead > PitStopDelay - 5)) {
				// laggard should now be in the lead!
				laggard_elapsed += laggard_lap_time;
			}
			else {
				// overtake failed, we are stuck 0.25 seconds behind leader
				laggard_elapsed = leader_elapsed_full + 0.25;
			}
		}
		else {
			// no chance to overtake
			laggard_elapsed += laggard_lap_time;
		}

		return laggard_elapsed;

	}

public:
	/*
	* A constructor for the Race class.
	* 
	* Parameters:
	* laps - the number of laps for a race.
	* lapLength - the time, in seconds, it would take a driver to complete the lap at FULL SPEED (maximum speed).
	* pitStopDelay - the time, in seconds, it takes to complete a pit stop to change tires.
	* driver1, driver2 - the two drivers in the race.
	* 
	*/
	Race(int laps, double lapLength, double pitStopDelay, Driver* driver1, Driver* driver2) : 
		Laps(laps), LapLength(lapLength), PitStopDelay(pitStopDelay), Driver1(driver1), Driver2(driver2)
	{
	}


	/*
	* runRace - simulates running the race.  The method loops Laps number of times.  For each lap, each driver is polled
	*           for the speed the driver wants to attempt (on a scale of 0.5 to 1, where 1 is FULL SPEED and 0.5 is HALF SPEED) and
	*           whether a pit stop is desired.
	* 
	*           The driver's attempted speed is converted into a potential lap time.  If the driver is in the lead, that is the lap time
	*           recorded for the lap.  If the driver is behind, see getLaggardElapsedTime() for an explanation of
	*           how the lap time is calculated.
	* 
			runRace returns the number of seconds that driver 1 beats driver 2 by (a negative number indicates driver 2 wins)
	*/
	double runRace() {
		double d1_elapsed = 0;
		double d2_elapsed = 0;

		// new tires!
		Driver1->getCar()->setTireWear(1.0);
		Driver2->getCar()->setTireWear(1.0);

		// we only get 1 pit stop
		Driver1->setHasPitted(false);
		Driver2->setHasPitted(false);

		int laps_left = Laps;
		while (laps_left > 0) {
			double d1_time = calcLapTime(Driver1);
			double d2_time = calcLapTime(Driver2);
				
			if (d1_elapsed < d2_elapsed) {
				// driver 1 is in the lead
				d1_elapsed += d1_time;
				d2_elapsed = getLaggardElapsedTime(d1_elapsed, d2_elapsed, d2_time);
			}
			else if (d2_elapsed < d1_elapsed) {
				// driver 2 is in the lead
				d2_elapsed += d2_time;
				d1_elapsed = getLaggardElapsedTime(d2_elapsed, d1_elapsed, d1_time);
			}
			else {
				// drivers are even
				d1_elapsed += d1_time;
				d2_elapsed += d2_time;
			}


			laps_left--;
			if (debug) {
				cout << (Laps - laps_left) << ": [" << (d2_elapsed - d1_elapsed) << "] " << d1_elapsed << " (" << Driver1->getCar()->getTireWear() << "), " << d2_elapsed << " (" << Driver2->getCar()->getTireWear() << ")" << endl;
			}
		}
		return (d2_elapsed - d1_elapsed)/Laps;
	}

};

/*
* Runs the race for the specified number of iterations.  Used to judge the relative fitness of the two driver's strategies.
* 
* Returns the average number of seconds driver 1 beats driver 2 by.  A negative number indicates driver 2 wins on average.
* 
* You may want to use runTestRace with a reasonable number of iterations (like 10000) in your search for an 'ideal' set 
* of driver parameters.
*/
double runTestRace(Race* race, int iterations) {
	double d1_winner_count=0;
	int total = 0;
	for (int i = 0; i < iterations; i++) {
		// runRace method returns the number of seconds driver1 won by.
		d1_winner_count += race->runRace();
		total++;
	}

	return ((double)d1_winner_count) / ((double)total);

}

// This is a "functor", a class object that acts like a function with state





/*
* The main() function for the program.  Make sure to include a screenshot of program execution with your assignment submission.
*/
int main() {
	
	// Here is a test run that demonstrates how to use runTestRace().
	std::cout << "TEST RUN" << std::endl;
	Car car1;
	Car car2;


	// double arr[100];
    // int n = sizeof(arr)/ sizeof(arr[0]);
    // int x = 10;
    // int result = randomizedBinarySearch(arr, 0, n-1, x);
	

	/* Possible way to integrate search
	std::vector<double, 0.75> possibilities(SIZE);
	for (int i = 0; i < SIZE; i++)
	{
		possibilities[i] = i;
	}
	std::iota(speed_array.begin(), speed_array.end(), 0.25);
	double speed_array[] = std::range(0.25, 0.75);
	*/


	// the constructor for a Driver takes a pointer to a Car and Speed and TireWearThreshold parameters
	double speed_1 = 0.8;
	double tw_1 = 0.25;
	// std::map<double, double> d1_wins_speed;
	double d1_wins_speed[10] = {};
	double d1_wins_tw[100] = {};
	double d1_speeds[10] = {};
	double d1_tws[100] = {};

	for (int i = 0; i < 10; i++) {
		speed_1 += 0.02;
		d1_speeds[i] = speed_1;
		Driver d1(&car1, speed_1, tw_1);
		Driver d2(&car2, 1.0, 0.7);

		Race race(100, 50, 20, &d1, &d2);

		d1_wins_speed[i] = runTestRace(&race, 10000);
			for (int i = 0; i < 10; i++) {
				tw_1 += 0.005;
				d1_tws[i] = tw_1;
				Driver d1(&car1, speed_1, tw_1);
				Driver d2(&car2, 1.0, 0.5);

				Race race(100, 50, 20, &d1, &d2);

				d1_wins_tw[i] = runTestRace(&race, 10000);
			}
	};
	/*
	Driver d1(&car1, 1.0, 0.5);
	Driver d2(&car2, 1.0, 0.5);

	Race race(100, 50, 20, &d1, &d2);

	double d1_wins[1];
	d1_wins[0] = runTestRace(&race, 100000);
	*/

	// int n = sizeof(arr)/ sizeof(arr[0]);
    // int x = 10;
    // int result = randomizedBinarySearch(arr, 0, n-1, x);
	

	// This should print a number like +/- 0.003   (I get 0.00270411 but your value may be different)
	// Notice that the two drivers have equivalent speed & threshold parameters but you do not get 0 as the average win time.
	// This is because we are running a simulation and there is always some random variation in results.  If we set
	// the number of iterations to something very high, then d1_wins should get close to 0.
	int count = 10;
	int i;
	double max;
	max = d1_wins_speed[0];
    // search num in inputArray from index 0 to elementCount-1 
    for(i = 0; i < count; i++){
        if(d1_wins_speed[i] > max){
            max = d1_wins_speed[i];
        }
    };

	int x = std::distance(d1_wins_speed, std::find(d1_wins_speed, d1_wins_speed + 5, max));
 
    // cout  << "Minimum Element\n" << endl << x << "What" << max << endl;
	cout << "EVEN TEST" << d1_wins_speed[0] << d1_wins_tw[0] << endl;



	double ideal_speed = d1_speeds[x];   
	double ideal_threshold = 0.5;
	/*
	* 
	*  IMPLEMENT YOUR SEARCH FOR AN IDEAL SET OF PARAMETERS HERE.
	* 
	*  It is fine to create a new function to implement the search and call it from here.
	*  Make sure to store the optimized speed parameter in the 'ideal_speed' variable and
	*  the optimized tire threshold parameter in the 'ideal_threshold' variable.
	* 
	*/
	
	// When your search is complete, you should set ideal_speed and ideal_threshold to your 'optimal' values
	cout << "IDEAL SPEED IS " << ideal_speed << " IDEAL THRESHOLD IS " << ideal_threshold << endl;

	return 0;
}
