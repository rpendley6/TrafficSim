#include <stdio.h>
#include <stdlib.h>
#include "engine1.h"

// Data srtucture for an event; this is independent of the application domain
struct Event {
	double timestamp;		// event timestamp
	void *AppData;			// pointer to event parameters
	void (*callback)(void *);	// callback, parameter is event
	struct Event *Next;		// priority queue pointer
	int carID;
} ;

// Future Event List
// Use an event structure as the header for the future event list (priority queue)
struct Event FEL ={-1.0, NULL, NULL, NULL};

// Function to print timestamps of events in event list
void PrintList (struct Event *List);


struct Event *Remove (void);


double Now = 0.0;


double CurrentTime (void)
{
	return (Now);
}

void Schedule (double ts, void *data, void (*cb)(void *))
{
	struct Event *e, *p, *q;

	// create event data structure and fill it in
	if ((e = malloc (sizeof (struct Event))) == NULL) exit(1);
	e->timestamp = ts;
	e->AppData = data;
	e->callback = cb;

	for (q=&FEL, p=FEL.Next; p!=NULL; p=p->Next, q=q->Next) {
		if (p->timestamp >= e->timestamp) break;
		}
	e->Next = q->Next;
	q->Next = e;
}

// Remove smallest timestamped event from FEL
struct Event *Remove (void)
{
	struct Event *e;

	if (FEL.Next==NULL) return (NULL);
	e = FEL.Next;
	FEL.Next = e->Next;
	return (e);
}



void RunSim (void) 
{
	struct Event *e;

	while ((e=Remove()) != NULL) {
		Now = e->timestamp;
		e->callback(e->AppData);
		free (e);
	}
}