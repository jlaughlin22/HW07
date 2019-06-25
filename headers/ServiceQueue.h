#ifndef SERVICE_Q_H_
#define SERVICE_Q_H_

#include <queue>
#include "Plane.h"
#include "LandingQueue.h"
#include "DepartureQueue.h"
#include "Random.h"

extern Random my_random; // Enables us to access the global variable declared in Simulator.h

/** The service queue takes a plane from the landing queue and adds it to its queue.
    When a plane in service queue has finished being serviced, it will be placed in the departure queue.
*/
class ServiceQueue
{
private:
	int min_service_time, max_service_time;  // range of service times
	LandingQueue *landing_queue;             // pointer to the landing queue
	DepartureQueue *departure_queue;         // pointer to the departure queue
	std::queue<Plane *> the_queue;           // queue of planes (just ONE) in the service queue
public:
	ServiceQueue() {}

	void set_service_times(int min_service_time, int max_service_time) {
		this->min_service_time = min_service_time;
		this->max_service_time = max_service_time;
	}

	void set_landing_queue(LandingQueue *landing_queue) {
		this->landing_queue = landing_queue;
	}

	void set_departure_queue(DepartureQueue *departure_queue) {
		this->departure_queue = departure_queue;
	}

	void update(int clock) 
	{
		// there is a plane at the gate
		if (!the_queue.empty()) {

			Plane *plane = the_queue.front();

			// check if a plane is ready to move from the service queue to the departure queue
			if ((clock - plane->start_service_time) > plane->service_time) {  
				the_queue.pop();
				plane->enter_departure_time = clock;
				departure_queue->the_queue.push(plane);
			}
		}

		// the gate is empty - ready to serve!
		if (the_queue.empty()) {

			// move a plane from the landing queue to the service queue
			if (!landing_queue->the_queue.empty()) {

				Plane *plane = landing_queue->the_queue.front();
				landing_queue->the_queue.pop();
				double wait_time = (clock - plane->arrival_time);//wait time is time from landing to service
				landing_queue->num_served++;
				landing_queue->total_wait += wait_time;
				plane->start_service_time = clock;
				plane->service_time = ( my_random.next_int( max_service_time - min_service_time ) ) + min_service_time;
				the_queue.push(plane);
				
			}
		}
	}

};

#endif