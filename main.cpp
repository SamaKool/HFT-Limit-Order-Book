#include <iostream>
#include "OrderBook.h"
#include <chrono> // For benchmarking

int main() {
    OrderBook book;

    std::cout << "--- HFT Order Book Simulation ---\n";

    // 1. Add some liquidity (Sellers sitting in the book)
    book.addOrder({ 1, OrderType::SELL, 100.50, 10 });
    book.addOrder({ 2, OrderType::SELL, 101.00, 20 });

    // 2. Aggressive Buyer comes in
    // He wants to buy 15 shares at 101.00.
    // He should eat the 10 shares at 100.50 first, then take 5 from 101.00.
    std::cout << "\n--- Incoming Aggressive Buy Order ---\n";
    book.addOrder({ 3, OrderType::BUY, 101.00, 15 });

    // 3. Benchmarking Section (The "Citadel" Flex)
    std::cout << "\n--- Performance Test ---\n";
    auto start = std::chrono::high_resolution_clock::now();

    // Simulate 10,000 orders
    for (int i = 0; i < 10000; i++) {
        book.addOrder({ i + 100, OrderType::BUY, 99.00, 1 }); // Passive Buyers
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Processed 10,000 orders in " << elapsed.count() << " seconds.\n";
    std::cout << "Throughput: " << 10000 / elapsed.count() << " orders/second.\n";

    return 0;
}