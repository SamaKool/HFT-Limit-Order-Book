#ifndef ORDER_H
#define ORDER_H

enum class OrderType { BUY, SELL };

struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;

    void print() const {}
};

#endif