#include <iostream>
#include <time.h>

using namespace std;

int start_time = 0;

//if in deadlock, returns -1, otherwise, returns 0
int state_of_philosophers (int philosophers[], int chopsticks[], int members) {
    int idle = 0;
    int requesting = 0;
    int waiting = 0;
    int eating = 0;

    for (int i = 0; i < members; i++) {
        switch(philosophers[i]) {
            case -1:
                idle++;
                break;
            case 0:
            case 1:
                requesting++;
                break;
            case 2:
            case 3:
                waiting++;
                break;
            default:
                eating++;
        }
    }
    cout << "At time: " + (clock() - start_time);
    cout << " Idle/Thinking : " << idle << " Requesting : " << requesting << " Waiting : " << waiting << " Eating : " << eating << endl;
    //deadlock
    if (waiting == members)
        return -1;

    return 0;
}

int main()
{
    srand (time(NULL));
    start_time = clock();
    //
    int members = 5;//5 + (rand() % 5) ;
    //each index is an individual philosopher, the value is the state of the philosopher:
    // - -1: is idle/thinking,
    // - 0: is requesting to eat, has zero chopsticks
    // - 1: is requesting to eat, has one chopstick
    // - 2: is waiting to eat, has zero chopsticks (when not enough chopsticks are available)
    // - 3: is waiting to eat, has one chopstick (when not enough chopsticks are available)
    // - any num > 3: is eating/the time needed to eat
    int philosphers[members];
    //each index is the corresponding chopstick in reference to the table and philosophers
    // for nth philosopher, their corresponding chopstick are n and n+1
    // the values correspond to the state of the chopstick:
    // - -1: if the chopstick is available/on the table
    // - positive number: the index of the philosopher who has it
    int chopsticks[members];

    //initializes all the philosophers and chopsticks to idle/on the table
    for (int i = 0; i < members; i++) {
        philosphers[i] = 0;
        chopsticks[i] = -1;
    }

    int next_stick;
    int time_to_eat;
    //how many rounds/how much time do we want this to run for?
    while (clock() - start_time < 10000) {
        for (int i = 0; i < members; i++) {
            next_stick = (i + 1 == members) ? 0: i + 1;

            switch (philosphers[i]) {
                //chance to request to eat
                case -1:
                    //if probability is met, they request to eat
                    if (20 > rand() % 100)
                        philosphers[i] = 0;
                    break;
                //wants to grab a chopstick, if one is available, it is grabbed (if they have both, they start eating)
                case 0:
                    if (chopsticks[i] == -1) {
                        chopsticks[i] = i;
                        philosphers[i] = 1;
                    } else if (chopsticks[next_stick] == -1) {
                        chopsticks[next_stick] = i;
                        philosphers[i] = 1;
                    } else {
                        philosphers[i] = 2;
                    }
                    break;
                case 1:
                    time_to_eat = clock() + (rand() % 1000);
                    if (chopsticks[i] == -1) {
                        chopsticks[i] = i;
                        philosphers[i] = time_to_eat;
                    } else if (chopsticks[next_stick] == -1) {
                        chopsticks[next_stick] = i;
                        philosphers[i] = time_to_eat;
                    } else {
                        philosphers[i] = 3;
                    }
                //waiting - zero chopsticks
                case 2:
                    if (chopsticks[i] == -1) {
                        chopsticks[i] = i;
                        philosphers[i] = 1;
                    } else if (chopsticks[next_stick] == -1) {
                        chopsticks[next_stick] = i;
                        philosphers[i] = 1;
                    } // else, stays at 2

                //waiting (one chopstick)- chance to drop the chopstick (if they get bored)
                case 3:
                    time_to_eat = clock() + (rand() % 1000);
                    if (chopsticks[i] == -1) {
                        chopsticks[i] = i;
                        philosphers[i] = time_to_eat;
                    } else if (chopsticks[next_stick] == -1) {
                        chopsticks[next_stick] = i;
                        philosphers[i] = time_to_eat;
                    } else {
                        //if gets too bored, drops a chopstick and goes back to thinking
                        if (5 > rand() % 100) {
                            if (chopsticks[i] == i) {
                                chopsticks[i] = -1;
                                philosphers[i] = -1;
                            } else if (chopsticks[next_stick] == i) {
                                chopsticks[next_stick] = -1;
                                philosphers[i] = -1;
                            }
                        } //else stays at 3
                    }
                    break;
                //es hora de comer
                default:
                    //done eating
                    if (philosphers[i] < clock()) {
                        philosphers[i] = -1;
                        chopsticks[i] = -1;
                        chopsticks[next_stick] = -1;
                    }

            }
        }

        if (state_of_philosophers(philosphers, chopsticks, members) == -1) {
            cout << "Currently in deadlock state" << endl;
        }
    }
    return 0;
}
