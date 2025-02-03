#include <iostream>
#include <queue>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>

class Task {
private:
    std::string title;
    std::string description;
    std::time_t due_time;

public:
    Task() : title(""), description(""), due_time(0) {} // Default constructor

    Task(const std::string& t, const std::string& d, std::time_t dt) : title(t), description(d), due_time(dt) {}

    std::time_t getDueTime() const {
        return due_time;
    }

    void display() const {
        std::cout << "\nTask: " << title << "\nDescription: " << description
                  << "\nDue Time: " << std::ctime(&due_time) << std::endl;
    }

    bool operator<(const Task& other) const {
        return due_time > other.due_time;
    }
};

class TaskScheduler {
private:
    std::priority_queue<Task> taskQueue;

    std::time_t getCurrentTime() const {
        return std::time(nullptr);
    }

public:
    void addTask() {
        std::string title, description;
        int minutes;

        std::cout << "Enter task title: ";
        std::getline(std::cin, title);
        std::cout << "Enter task description: ";
        std::getline(std::cin, description);
        std::cout << "Enter due time in minutes from now: ";
        std::cin >> minutes;
        std::cin.ignore();

        std::time_t due_time = getCurrentTime() + minutes * 60;
        Task newTask(title, description, due_time);
        taskQueue.push(newTask);
        std::cout << "Task added successfully!\n";
    }

    void viewNextTask() const {
        if (!taskQueue.empty()) {
            taskQueue.top().display();
        } else {
            std::cout << "No tasks available.\n";
        }
    }

    void runScheduler() {
        while (!taskQueue.empty()) {
            Task nextTask = taskQueue.top();
            std::time_t currentTime = getCurrentTime();

            if (currentTime >= nextTask.getDueTime()) {
                std::cout << "\n*** Task Notification ***\n";
                nextTask.display();
                taskQueue.pop();
            } else {
                int waitTime = static_cast<int>(nextTask.getDueTime() - currentTime);
                std::cout << "Waiting for next task... (" << waitTime << " seconds remaining)\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));  // Explicit namespace reference
            }
        }
        std::cout << "All tasks completed!\n";
    }

    void start() {
        int choice;

        while (true) {
            std::cout << "\n--- Task Scheduler ---\n";
            std::cout << "1. Add Task\n2. View Next Task\n3. Run Scheduler\n4. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    addTask();
                    break;
                case 2:
                    viewNextTask();
                    break;
                case 3:
                    runScheduler();
                    break;
                case 4:
                    std::cout << "Exiting Task Scheduler. Goodbye!\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    TaskScheduler scheduler;
    scheduler.start();
    return 0;
}
