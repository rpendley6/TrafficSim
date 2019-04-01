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
#define	ARRIVAL	5.0
#define	LIGHTTIME	15.0

// number of arrivals to be simulated (used to determine length of simulation run)
#define	NARRIVALS	5

// State Variables of Simulation
int numEnters = 0;
int numFirstMoves = 0;
int numSecondMoves = 0;
int numExits = 0;

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
	int carNumber;
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

	printf ("Car %d arrived at intersection 1: time=%f\n",numEnters, CurrentTime());

	// schedule next arrival event if this is not the last arrival
	if (numEnters < NARRIVALS) {
		if((d=malloc(sizeof(struct EventData)))==NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
		d->EventType = ENTER;
		ts = CurrentTime() + RandExp(ARRIVAL);
		Schedule (ts, d, (void *) Enter);
	}

	//schedule arrival at second light
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = MOVELIGHTS1;
	ts = CurrentTime() + LIGHTTIME;
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

	printf ("Car %d moved to intersection 2: time=%f\n",numFirstMoves, CurrentTime());

	// schedule arrival at third light
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = MOVELIGHTS2;
	ts = CurrentTime() + LIGHTTIME;
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

	printf("Car %d moved to intersection 3: time=%f\n",numSecondMoves, CurrentTime());

	// schedule leaving event
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = LEAVE;
	ts = CurrentTime() + LIGHTTIME;
	Schedule (ts, d, (void *) Leave);

	LastEventTime = CurrentTime();		// time of last event processed
	free (e);				// event parameters
}


// event handler for cars leaving the system
void Leave (struct EventData *e)
{
	numExits++;
	printf ("Car %d exited the last intersection: time=%f\n", numExits, CurrentTime());

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
}
