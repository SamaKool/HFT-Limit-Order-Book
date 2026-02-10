#ifndef ORDER_H
#define ORDER_H

enum class OrderType { BUY, SELL };

struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;

    // Helper to print order details easily
    void print() const {
        // Simple print for debugging
        // (In a real system, we'd use a logging library, but this works for now)
    }
};

#endif