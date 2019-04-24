#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "engine1.h"

// Execution time statistics (for measuring runtime of program); unrelated to sim model
clock_t StartTime, EndTime;	// start and end time of simulation run

/////////////////////////////////////////////////////////////////////////////////////////////
//
// State variables  and other global information
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Simulation constants; all times in minutes
// A = mean car arrival time
// R = time each car waits at an intersection
#define	ARRIVAL	20.0
#define TRAVELBETWEENLIGHTS  25.0

// number of arrivals to be simulated (used to determine length of simulation run)
#define	NARRIVALS	40

// State Variables of Simulation
int numEnters = 0;
int numFirstMoves = 0;
int numSecondMoves = 0;
int numExits = 0;
double minusTotalTime=0;  //used to subtract the time cars are not on the road
double totalTime = 0;		//used to find total time all the cars take to finish

int numReds1 = 0;		//used to make sure cars dont leave red lights at same time
int numReds2 = 0;
int numReds3 = 0;

// State variables used for statistics
double	TotalWaitingTime = 0.0;	// total time waiting to land
double	LastEventTime = 0.0;	// time of last event processed; used to compute TotalWaitingTime

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Data structures for events
//
/////////////////////////////////////////////////////////////////////////////////////////////

// types of events
typedef enum {ENTER, MOVELIGHTS1, MOVELIGHTS2, LEAVE} KindsOfEvents;

// Event parameters
// No event parameters really needed in this simple simulation
struct EventData {
	KindsOfEvents EventType;
};
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Function prototypes
//
/////////////////////////////////////////////////////////////////////////////////////////////

// prototypes for event handlers
void Enter (struct EventData *e);		// car enter event, arrives at light 1
void MoveLights1 (struct EventData *e);	// car moves to traffic light 2
void MoveLights2 (struct EventData *e);	// car moves to traffic light 3
void Leave (struct EventData *e);		// car exits the simulation

// prototypes for other procedures
double RandExp(double M);			// random variable, exponential distribution

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Random Number generator
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Compute exponenitally distributed random number with mean M
double RandExp(double M)
{
	double urand;	// uniformly distributed random number [0,1)
	urand = ((double) rand ()) / (((double) RAND_MAX)+1.0);	// avoid value 1.0
	return (-M * log(1.0-urand));
}

double RandNum(double max) {
	double urand;	// uniformly distributed random number [0,1)
	urand = ((double) rand ()) / (((double) RAND_MAX)+1.0);	// avoid value 1.0
	return urand*max;
}

double random1 = 0;
double random2 = 0;
double random3 = 0;
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Event Handlers
// Parameter is a pointer to the data portion of the event
//
/////////////////////////////////////////////////////////////////////////////////////////////

// event handler for cars entering the system
void Enter (struct EventData *e)
{
	numEnters++;
	struct EventData *d;
	double ts;
	if (numEnters == 1) {
		random1 = RandNum(99.0);
		printf("random1 =%f\n", random1);
	}

	printf ("Car %d arrived at intersection 1: time=%f\n",numEnters, CurrentTime());

	// schedule next arrival event if this is not the last arrival
	if (numEnters < NARRIVALS) {
		if((d=malloc(sizeof(struct EventData)))==NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
		d->EventType = ENTER;
		ts = CurrentTime() + RandExp(ARRIVAL);
		minusTotalTime += ts;
		Schedule (ts, d, (void *) Enter);
	}

	//schedule arrival at second light
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = MOVELIGHTS1;
	double t = fmod(CurrentTime() + random1, 99.0);  //current time % 99
	if (t <= 44) {		//light is green
		ts = CurrentTime() + TRAVELBETWEENLIGHTS;	//car moves through light
		numReds1=0;
	} else {			//light is red
		ts = CurrentTime() + (99-t) + TRAVELBETWEENLIGHTS + numReds1;
		numReds1+=5;  //cars leave red lights 3 seconds apart
	}
	Schedule (ts, d, (void *) MoveLights1);

	LastEventTime = CurrentTime();
	free (e);				// free storage for event parameters
}

// event handler for cars leaving intersection 1 and moving to intersection 2
void MoveLights1 (struct EventData *e)
{
	numFirstMoves++;
	struct EventData *d;
	double ts;

	if (numFirstMoves == 1) {
		random2 = RandNum(100.0);
		printf("random2 =%f\n", random2);
	}

	printf ("Car %d moved to intersection 2: time=%f\n",numFirstMoves, CurrentTime());

	// schedule arrival at third light
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = MOVELIGHTS2;

	double urand;	// uniformly distributed random number [0,1)
	urand = ((double) rand ()) / (((double) RAND_MAX)+1.0);	// avoid value 1.0

	double t = fmod(CurrentTime() + random2, 100.0);  //current time % 100
	if (t <= 64) {		//light is green
		ts = CurrentTime() + TRAVELBETWEENLIGHTS;	//car moves through light
		numReds2 = 0;
	} else {			//light is red
		ts = CurrentTime() + (100-t) + TRAVELBETWEENLIGHTS + numReds2;
		numReds2 +=5;
	}
	Schedule (ts, d, (void *) MoveLights2);

	LastEventTime = CurrentTime();		// time of last event processed
	free (e);				// event parameters
}

// event handler for cars leaving intersection 2 and moving to intersection 3
void MoveLights2 (struct EventData *e)
{
	numSecondMoves++;
	struct EventData *d;
	double ts;

	if (numSecondMoves == 1) {
		random3 = RandNum(86.0);
		printf("random 3 = %f\n", random3);
	}

	printf("Car %d moved to intersection 3: time=%f\n",numSecondMoves, CurrentTime());

	// schedule leaving event
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = LEAVE;
	double t = fmod(CurrentTime() + random3, 84.0);  //current time % 84
	if (t <= 38) {		//light is green
		ts = CurrentTime() + TRAVELBETWEENLIGHTS;	//car moves through light
		numReds3 = 0;
	} else {			//light is red
		ts = CurrentTime() + (84-t) + TRAVELBETWEENLIGHTS + numReds3;
		numReds3+=5;
	}
	Schedule (ts, d, (void *) Leave);

	LastEventTime = CurrentTime();		// time of last event processed
	free (e);				// event parameters
}


// event handler for cars leaving the system
void Leave (struct EventData *e)
{
	numExits++;
	printf ("Car %d exited the last intersection: time=%f\n", numExits, CurrentTime());
	totalTime+=CurrentTime();
	LastEventTime = CurrentTime();		// time of last event processed
	free (e);				// event parameters
}

///////////////////////////////////////////////////////////////////////////////////////
//////////// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////

int main (void)
{
	struct EventData *d;
	double ts;
	double Duration;
	srand(time(NULL));
	// printf("stoplight 1 random starting value = %f\n", rand1);
	// printf("stoplight 2 random starting value = %f\n", rand2);
	// printf("stoplight 3 random starting value = %f\n", rand3);


	// initialize event list with first arrival
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = ENTER;
	ts = RandExp(ARRIVAL);
	Schedule (ts, d, (void *) Enter);

	printf ("Peachtree Street Simulation\n");
	StartTime = clock();
	RunSim();
	EndTime = clock();

	// print final statistics
	printf ("Number of cars = %d\n", NARRIVALS);
	Duration = (double) (EndTime-StartTime) / (double) CLOCKS_PER_SEC;
	int numEvents= numEnters + numFirstMoves + numSecondMoves + numExits;
	printf ("%d events executed in %f seconds (%f events per second)\n", numEvents, Duration, (double)numEvents/Duration);
	printf("Total time =%f\n", totalTime - minusTotalTime);
	printf("Average travel time for each car = %f\n", (totalTime - minusTotalTime) / NARRIVALS);
}