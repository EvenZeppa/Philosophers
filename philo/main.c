#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define TRUE 1

typedef struct s_philosopher {
	int id;
	int meals_eaten;
	long last_meal_time;
	pthread_t thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	struct s_data *data;
} t_philosopher;

typedef struct s_data {
	int num_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int meals_required;
	int stop_simulation;
	long start_time;
	pthread_mutex_t *forks;
	pthread_mutex_t print_lock;
	t_philosopher *philosophers;
} t_data;

long get_timestamp(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void print_status(t_philosopher *philo, const char *status) {
	pthread_mutex_lock(&philo->data->print_lock);
	if (!philo->data->stop_simulation) {
		printf("%ld %d %s\n", get_timestamp() - philo->data->start_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

void *philosopher_routine(void *arg) {
	t_philosopher *philo = (t_philosopher *)arg;

	while (!philo->data->stop_simulation) {
		if (philo->id % 2 == 0)
			usleep(1000); // Décale les philosophes pairs d’une milliseconde
		// Thinking
		print_status(philo, "is thinking");

		// Picking up forks
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");

		// Eating
		print_status(philo, "is eating");
		philo->last_meal_time = get_timestamp();
		usleep(philo->data->time_to_eat * 1000);
		philo->meals_eaten++;

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		// Sleeping
		print_status(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
	}
	return NULL;
}

void monitor_philosophers(t_data *data) {
    while (!data->stop_simulation) {
        int finished_philosophers = 0;
        for (int i = 0; i < data->num_philosophers; i++) {
            t_philosopher *philo = &data->philosophers[i];

            pthread_mutex_lock(&data->print_lock);
            if (get_timestamp() - philo->last_meal_time > data->time_to_die) {
                printf("%ld %d died\n", get_timestamp() - data->start_time, philo->id);
                data->stop_simulation = 1;
            }
            pthread_mutex_unlock(&data->print_lock);

            if (data->meals_required > 0 && philo->meals_eaten >= data->meals_required)
                finished_philosophers++;
        }
        if (data->meals_required > 0 && finished_philosophers == data->num_philosophers)
            data->stop_simulation = 1;

        usleep(500); // Moins gourmand que 1000
    }
}


t_data *initialize_data(int argc, char **argv) {
	t_data *data = malloc(sizeof(t_data));
	if (!data)
		return NULL;

	data->num_philosophers = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	data->meals_required = (argc == 6) ? atoi(argv[5]) : -1;
	data->stop_simulation = 0;
	data->start_time = get_timestamp();

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
	data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);

	if (!data->forks || !data->philosophers)
		return NULL;

	pthread_mutex_init(&data->print_lock, NULL);

	for (int i = 0; i < data->num_philosophers; i++) {
		pthread_mutex_init(&data->forks[i], NULL);
	}

	for (int i = 0; i < data->num_philosophers; i++) {
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].last_meal_time = data->start_time;
		if (i % 2 == 0) {
			data->philosophers[i].left_fork = &data->forks[i];
			data->philosophers[i].right_fork = &data->forks[(i + 1) % data->num_philosophers];
		} else {
			data->philosophers[i].right_fork = &data->forks[i];
			data->philosophers[i].left_fork = &data->forks[(i + 1) % data->num_philosophers];
		}

		data->philosophers[i].data = data;
	}

	return data;
}

void cleanup(t_data *data) {
	for (int i = 0; i < data->num_philosophers; i++) {
		pthread_mutex_destroy(&data->forks[i]);
	}
	pthread_mutex_destroy(&data->print_lock);
	free(data->forks);
	free(data->philosophers);
	free(data);
}

int main(int argc, char **argv) {
	if (argc < 5 || argc > 6) {
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return 1;
	}

	t_data *data = initialize_data(argc, argv);
	if (!data) {
		printf("Error initializing data\n");
		return 1;
	}

	for (int i = 0; i < data->num_philosophers; i++) {
		pthread_create(&data->philosophers[i].thread, NULL, philosopher_routine, &data->philosophers[i]);
	}

	monitor_philosophers(data);

	for (int i = 0; i < data->num_philosophers; i++) {
		pthread_join(data->philosophers[i].thread, NULL);
	}

	cleanup(data);
	return 0;
}
