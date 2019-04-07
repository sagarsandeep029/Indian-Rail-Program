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
		int threads_to_reap = MIN(passengers_left, free_seats);
		int threads_reaped = 0;
		while (threads_reaped < threads_to_reap) {
			if (load_train_returned) {
				fprintf(stderr, "Error: station_load_train returned early!\n");
				exit(1);
			}
			if (threads_completed > 0) {
				if ((pass % 2) == 0)
					usleep(random() % 2);
				threads_reaped++;
				station_on_board(&station);
				__sync_sub_and_fetch(&threads_completed, 1);
			}
		}
		for (i = 0; i < 1000; i++) {
			if (i > 50 && load_train_returned)
				break;
			usleep(1000);
		}

		if (!load_train_returned) {
			fprintf(stderr, "Error: station_load_train failed to return\n");
			exit(1);
		}

		while (threads_completed > 0) {
			threads_reaped++;
			__sync_sub_and_fetch(&threads_completed, 1);
		}

		passengers_left -= threads_reaped;
		total_passengers_boarded += threads_reaped;
		printf("Train departed station with %d new passenger(s) (expected %d)%s\n",
			threads_reaped,threads_to_reap,
			(threads_to_reap != threads_reaped) ? " *****" : "");

		if (threads_to_reap != threads_reaped) {
			fprintf(stderr, "Error: Too many passengers on this train!\n");
			exit(1);
		}

		pass++;
	}

	if (total_passengers_boarded == total_passengers) {
		printf("Looks good!\n");
	} else {
		fprintf(stderr, "Error: expected %d total boarded passengers, but got %d!\n",
            total_passengers, total_passengers_boarded);
		cnt ++;
		return 1;
	}
	printf("---------------\n");
	}
	printf("%d",cnt);
	return 0;
	
}
	
	


