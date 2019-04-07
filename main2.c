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
	int cnt=0;
	int i = 50;
	while(i--){
		struct station station;
		station_init(&station);

		srandom(getpid() ^ time(NULL));

		signal(SIGALRM, alarm_handler);
	
		_alarm(1, "station_load_train() did not return immediately when no waiting passengers");
		station_load_train(&station, 0);
		station_load_train(&station, 10);
		_alarm(0, NULL);
		int i;
		const int total_passengers = 100;
		int passengers_left = total_passengers;
		for (i = 0; i < total_passengers; i++) {
			pthread_t tid;
			int ret = pthread_create(&tid, NULL, passenger_thread, &station);
			if (ret != 0) {
				perror("pthread_create");
				exit(1);
			}
		}
	}
	
		_alarm(2, "station_load_train() did not return immediately when no free seats");
		station_load_train(&station, 0);
		_alarm(0, NULL);
		int total_passengers_boarded = 0;
		const int max_free_seats_per_train = 50;
		int pass = 0;
		while (passengers_left > 0) {
			_alarm(2, "Some more complicated issue appears to have caused passengers not to board when given the opportunity");

		int free_seats = random() % max_free_seats_per_train;
		printf("Train entering station with %d free seats\n", free_seats);
		load_train_returned = 0;
		struct load_train_args args = { &station, free_seats };
		pthread_t lt_tid;
		int ret = pthread_create(&lt_tid, NULL, load_train_thread, &args);
		if (ret != 0) {
			perror("pthread_create");
			exit(1);
		}	
		}
	

}
	
	


