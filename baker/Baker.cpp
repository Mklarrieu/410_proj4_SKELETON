#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	int numDonuts = anOrder.number_donuts;
	int fullboxes = numDonuts / 12;
	int unfilledBoxes;
	if(numDonuts % 12 > 0){
		unfilledBoxes = 1;
	}
	for(int i = 0; i < fullboxes; i++){
		Box box;
		for( int j = 0; j < 12; j++){
			DONUT d;
			box.addDonut(d);
		}
		anOrder.boxes.push_back(box);
	}
	if(unfilledBoxes > 0){
		Box extraBox;
		int extraDonuts = numDonuts % 12;
		for(int i = 0; i < extraDonuts; i++){
			DONUT d;
			extraBox.addDonut(d);
		}
		anOrder.boxes.push_back(extraBox);
	}
}

void Baker::beBaker() {
	while(true){
		unique_lock<mutex> lck(mutex_order_inQ);
		while(!b_WaiterIsFinished){
			cv_order_inQ.wait(lck);
		}


		if(!order_in_Q.empty()){
			ORDER myorder = order_in_Q.front();
			order_in_Q.pop();
			lck.unlock();
			bake_and_box(myorder);
			lock_guard<mutex> lock(mutex_order_outQ);
			order_out_Vector.push_back(myorder);
		}
		else{
			break;
		}
	}

}
