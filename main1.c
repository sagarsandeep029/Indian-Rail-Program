#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>

struct station {
	int empty_seat_train;
	int waiting_train_passangers;
	int on_board_passangers;
	pthread_mutex_t train_lock;
    pthread_cond_t arrive_of_train;
	pthread_cond_t all_passangers_on_board;
};
void station_init(struct station *station)
{
	station->waiting_train_passangers = 0;
	station->on_board_passangers = 0;
	pthread_mutex_init(&station->train_lock, NULL);
	pthread_cond_init(&station->arrive_of_train, NULL);
	pthread_cond_init(&station->all_passangers_on_board, NULL);
}

void station_load_train(struct station *station,int count)
{
	pthread_mutex_lock(&station->train_lock);
  	station->empty_seat_train = count;
	while (station->empty_seat_train > 0 && station->waiting_train_passangers > 0){
        pthread_cond_broadcast(&station->arrive_of_train);
        pthread_cond_wait(&station->all_passangers_on_board, &station->train_lock);
	}
	station->empty_seat_train = 0;
	pthread_mutex_unlock(&station->train_lock);
}
void station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&station->train_lock);
    station->waiting_train_passangers ++;
    while(station->on_board_passangers == station->empty_seat_train)
    	pthread_cond_wait(&station->arrive_of_train, &station->train_lock);
    station->on_board_passangers ++;
    station->waiting_train_passangers --;
	pthread_mutex_unlock(&station->train_lock);
}
void station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->train_lock);
    station->on_board_passangers --;
    station->empty_seat_train --;
    if (station->empty_seat_train == 0 ||(station->on_board_passangers == 0 &&
        station->waiting_train_passangers == 0))
        pthread_cond_signal(&station->all_passangers_on_board);
    pthread_mutex_unlock(&station->train_lock);
}

int main()
{
	struct station station;
}


