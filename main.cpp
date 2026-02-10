#include <iostream>
#include "OrderBook.h"
#include "ObjectPool.h"
#include <chrono>

int main() {
    OrderBook book;

    // Initialize the Pool (Allocate 100,000 orders upfront)
    std::cout << "Initializing Memory Pool...\n";
    ObjectPool<Order> orderPool(100000);

    std::cout << "--- HFT Order Book Simulation ---\n";

    // Benchmarking with the Pool
    std::cout << "\n--- Performance Test (Zero Allocation) ---\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100000; i++) {
        Order* newOrder = orderPool.acquire();

        if (newOrder != nullptr) {
            newOrder->id = i;
            newOrder->type = OrderType::BUY;
            newOrder->price = 99.00;
            newOrder->quantity = 1;

            book.addOrder(*newOrder);

            orderPool.release(newOrder);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Processed 100,000 orders in " << elapsed.count() << " seconds.\n";
    std::cout << "Throughput: " << 100000 / elapsed.count() << " orders/second.\n";

    return 0;
}