#include <iostream>
#include<bits/stdc++.h>
using namespace std;

enum Frequency{
    HOURLY,
    DAILY,
    WEEKLY,
    MONTHLY,
    YEARLY,
    ONE_TIME
};

enum State{
    TO_BE_DONE,
    IN_PROGRESS,
    WAIT,
    CLOSED
};

enum Priority{
    LOW,
    MEDIUM,
    MAJOR,
    CRITICAL
};

class Time{
    unsigned int day, month, year, hour, minute, second;
};

class Update{
};
class StateUpdate : public Update{
    State prevState, newState;
};
class PriorityUpdate : public Update{
    Priority prevPriority, newPriority;
};
class DueDateUpdate : public Update{
    Time prevDueDate, newDueDate;
};

class Activity{
    Time timeOfUpdate;
    list<Update> listOfUpdates;
};

class Reminder{
    Task task;
    Time timeOfReminder;
};

class Task{
    Time createdOn;
    Frequency freq;
    State state;
    Priority priority;
    list<Activity> listOfActivities;
};
class TaskManager{
    unsigned int userID;
    list<Task> listOfTasks;
    priority_queue<Reminder> pq;
    Task SendReminder(){
    }
};

int main() {

}
