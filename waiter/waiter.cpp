#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){

}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	if(myIO.getNext(anOrder) == SUCCESS){
		return SUCCESS;
	}
	b_WaiterIsFinished = true;
	return FAIL;
}

void Waiter::beWaiter() {
	ORDER order;
//	myIO.loadData();
	getNext(order);
	while(!b_WaiterIsFinished){

		mutex_order_inQ.lock();
		order_in_Q.push(order);
		mutex_order_inQ.unlock();

		cv_order_inQ.notify_all();

		getNext(order);
		}
	}


