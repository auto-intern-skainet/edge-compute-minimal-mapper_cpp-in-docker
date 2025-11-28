#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <iomanip>

volatile sig_atomic_t running = 1;

void signal_handler(int sig) {
    running = 0;
}

int main() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    try {
        sw::redis::Redis redis("tcp://127.0.0.1:6379");

        while (running) {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::tm tm = *std::localtime(&time_t);

            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " - Reading from skAInet:test" << std::endl;

            sw::redis::OptionalString value = redis.get("skAInet:test");
            std::string val = value ? *value : "(nil)";

            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            std::string timestamp = oss.str();
            std::string new_val = val + " " + timestamp;

            redis.set("skAInet:dockertest", new_val);

            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " - Written to skAInet:dockertest: " << new_val << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        std::cout << "Shutting down gracefully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}