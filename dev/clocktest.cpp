#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();

    unsigned long long e = 0;
    for (unsigned long i = 0; i < 15000000000; i++) {
        e += i;
    }

    std::chrono::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Start: " << start.time_since_epoch().count() << "\nEnd:   " << end.time_since_epoch().count() << "\n";
    std::cout << "Duration: " << duration.count() << "\n";
    return 0;
}
