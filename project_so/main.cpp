#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ncurses.h>
#include <chrono>

using namespace std;

#define NUM_OF_PHILOSOPHERS 10
int numPhilosophers;


enum enumPhilosopherStatus {THINKING, HUNGRY, EATING};
enumPhilosopherStatus philosophersStatus[NUM_OF_PHILOSOPHERS];

int leftFork;
int rightFork;

mutex mutexStatus;
condition_variable cv[NUM_OF_PHILOSOPHERS];
mutex forks[NUM_OF_PHILOSOPHERS];

int randTime(int bottomRange, int topRange) {
    return rand() % (topRange - bottomRange + 1) + bottomRange;
}
void thinkState(int i) {
    unique_lock<mutex> lock(mutexStatus);
    philosophersStatus[i] = enumPhilosopherStatus::THINKING;
}
void eatState(int i) {
    unique_lock<mutex> lock(mutexStatus);
    philosophersStatus[i] = enumPhilosopherStatus::EATING;
}
void hungryState(int i) {
    unique_lock<mutex> lock(mutexStatus);
    philosophersStatus[i] = enumPhilosopherStatus::HUNGRY;
}



void philosopher(int id) {
    while(true) {

        //THINKING
        thinkState(id);
        //cout << "Philosopher " << id << " thinking" << endl;
        //this_thread::sleep_for(chrono::milliseconds (randTime(1000, 2000)));


        //HUNGRY
        hungryState(id);
        //cout << "Philosopher " << id << " hungry" << endl;
        this_thread::sleep_for(chrono::milliseconds (randTime(1, 2)));

        unique_lock<mutex> leftForkMtx(forks[id]);
        unique_lock<mutex> rightForkMtx(forks[(id + numPhilosophers - 1)%numPhilosophers]);

        while(philosophersStatus[(id + numPhilosophers -1)%numPhilosophers] == EATING) {
            cv->wait(leftForkMtx);
        }

        while(philosophersStatus[(id + 1)%numPhilosophers] == EATING) {
            cv->wait(rightForkMtx);
        }

        leftFork = id;
        rightFork = (id + 1) % numPhilosophers;



        //EATING
        eatState(id);
        //cout << "Philosopher " << id << " eating" << endl;
        this_thread::sleep_for(chrono::milliseconds (randTime(1000, 2000)));


        leftForkMtx.unlock();
        rightForkMtx.unlock();


        //NOTIFY NEIGHBOURS THAT FORKS ARE AVAILABLE
        cv[(id + 1) % numPhilosophers].notify_one();
        cv[(id + numPhilosophers - 1)%numPhilosophers].notify_one();
    }
}





int main(int argv, char* argc[]) {
    numPhilosophers = atoi(argc[1]);

    thread philosophers[numPhilosophers];


    for (int i = 0; i < numPhilosophers; i++) {
        philosophers[i] = thread(philosopher, i);
    }

    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();

    init_pair(1, COLOR_YELLOW, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_GREEN, -1);


    while(true) {
        clear();
        attron(A_BOLD);
        mvprintw(0,0, "Philosopher");
        mvprintw(0,20, "State");
        mvprintw(0,35, "Left Fork");
        mvprintw(0,52, "Right Fork");
        attroff(A_BOLD);
        for (int i = 0; i < numPhilosophers; i++) {
            mvprintw(i+2,0, "Philosopher %d", i+1);
            if (philosophersStatus[i] == THINKING) {
                attron(COLOR_PAIR(1));
                mvprintw(i+2, 20, "THINKING");
                attroff(COLOR_PAIR(1));
            } else if(philosophersStatus[i] == HUNGRY) {
                attron(COLOR_PAIR(2));
                mvprintw(i+2, 20, "HUNGRY");
                attroff(COLOR_PAIR(2));
            } else if(philosophersStatus[i] == EATING) {
                attron(COLOR_PAIR(3));
                mvprintw(i+2, 20, "EATING");
                attroff(COLOR_PAIR(3));
                mvprintw(i+2, 35, "%d", i);
                mvprintw(i+2, 55, "%d", (i + 1)%numPhilosophers);

            }
        }
        refresh();
        this_thread::sleep_for(chrono::milliseconds(330));
    }


    for (int i = 0; i < numPhilosophers; i++) {
        philosophers[i].join();
    }

    endwin();
    return 0;

}
