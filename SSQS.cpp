#include<iostream>
#include<math.h>
#include "lcgrand.h"

using namespace std;

#define Q_LIMIT 100
#define BUSY 1
#define IDLE 0

int next_event_type, num_custs_delayed, num_delays_required, num_events, num_in_q, server_status;

float area_num_in_q, area_server_status, mean_interarrival, mean_service, sim_time, time_arrival[Q_LIMIT + 1], time_last_event, time_next_event[3], total_of_delays;

void initialize();
void timing();
void arrive();
void depart();
void report();
void update_time_avg_stats();
float expon(float mean);

int main(){
    num_events = 2;
    cout<<"Enter Mean Interarrival Time: ";
    cin>>mean_interarrival;
    cout<<"Enter Mean Service Time: ";
    cin>>mean_service;
    cout<<"Enter Number of Customers: ";
    cin>>num_delays_required;

    initialize();

    while(num_custs_delayed < num_delays_required){
        timing();

        update_time_avg_stats();

        switch(next_event_type){
            case 1:
                arrive();
                break;
            case 2:
                depart();
                break;
        }
    }
    report();

    return 0;
}

void initialize(){
    sim_time = 0.0;

    server_status = IDLE;
    num_in_q = 0;
    time_last_event = 0.0;

    num_custs_delayed = 0;
    total_of_delays = 0.0;
    area_num_in_q = 0.0;
    area_server_status = 0.0;

    time_next_event[1] = sim_time + expon(mean_interarrival);
    time_next_event[2] = 1.0e+30;
}

void timing(){
    int i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    for(i = 1; i <= num_events; ++i){
        if(time_next_event[i] < min_time_next_event){
            min_time_next_event = time_next_event[i];
            next_event_type = i;
        }
    }
        if(next_event_type == 0){
            cout<<"Event List Empty at Time: "<<sim_time;
            exit(1);
        }

        sim_time = min_time_next_event;
}

void arrive(){
    float delay;

    time_next_event[1] = sim_time + expon(mean_interarrival);

    if(server_status == BUSY){
        ++num_in_q;
    
        if(num_in_q > Q_LIMIT){
            cout<<"Overflow of the array time_arrival at time: "<<sim_time<<endl;
            exit(2);
        }
        time_arrival[num_in_q] = sim_time;
    }

    else{
        delay = 0.0;
        total_of_delays += delay;

        ++num_custs_delayed;
        server_status = BUSY;

        time_next_event[2] = sim_time + expon(mean_service);
    }
}

void depart(){
    int i;
    float delay;

    if(num_in_q == 0){
        server_status = IDLE;
        time_next_event[2] = 1.0e+30;
    }

    else{
        --num_in_q;

        delay = sim_time - time_arrival[1];
        total_of_delays += delay;

        ++num_custs_delayed;
        time_next_event[2] = sim_time + expon(mean_service);

        for(i = 1; i <= num_in_q; ++i){
            time_arrival[i] = time_arrival[i+1];
        }
    }
}


void report(){
    cout<<"\nSIMULATION RESULT"<<endl;
    cout<<"********************************"<<endl;
    cout<<"Total Delay: \t\t\t"<<total_of_delays<<endl;
    cout<<"Average Delay in Queue: \t"<<total_of_delays/num_custs_delayed<<endl<<endl;
    cout<<"Area Number in Queue: \t\t"<<area_num_in_q<<endl;
    cout<<"Average Number in Queue: \t"<<area_num_in_q/sim_time<<endl<<endl;
    cout<<"Area Server Status: \t\t"<<area_server_status<<endl;
    cout<<"Server Utilization: \t\t"<<area_server_status/sim_time<<endl<<endl;
    cout<<"Time Simulation Ended: \t\t"<<sim_time<<endl;
}

void update_time_avg_stats(){
    float time_since_last_event;

    time_since_last_event = sim_time - time_last_event;
    time_last_event = sim_time;

    area_num_in_q += num_in_q*time_since_last_event;

    area_server_status += server_status*time_since_last_event;
}

float expon(float mean){
    return -mean * log(lcgrand(1));
}
