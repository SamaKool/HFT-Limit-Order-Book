#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "Order.h"
#include <map>
#include <vector>
#include <iostream>
#include <functional> // For std::greater

class OrderBook {
private:
    // Bids: Sorted High -> Low (Best price to sell to is the Highest Bid)
    std::map<double, std::vector<Order>, std::greater<double>> bids;

    // Asks: Sorted Low -> High (Best price to buy from is the Lowest Ask)
    std::map<double, std::vector<Order>, std::less<double>> asks;

public:
    // The Core Function: Takes an order and tries to match it
    void addOrder(Order newOrder) {
        if (newOrder.type == OrderType::BUY) {
            matchBuyOrder(newOrder);
        }
        else {
            matchSellOrder(newOrder);
        }
    }

    // Matching Logic for Incoming BUY Orders
    void matchBuyOrder(Order& order) {
        // While the order still needs filling AND there are sellers available
        while (order.quantity > 0 && !asks.empty()) {

            // Look at the best Ask (Cheapest Seller)
            auto bestAskIt = asks.begin();
            double bestAskPrice = bestAskIt->first;
            std::vector<Order>& sellOrders = bestAskIt->second;

            // If the Seller is too expensive, we can't trade. Stop.
            if (bestAskPrice > order.price) {
                break;
            }

            // TRADE EXECUTION LOGIC
            Order& sellOrder = sellOrders.front(); // Priority: First In, First Out (FIFO)

            int quantityTraded = std::min(order.quantity, sellOrder.quantity);

            /*std::cout << "[TRADE] Buy Order " << order.id << " matched with Sell Order "
                << sellOrder.id << " for " << quantityTraded << " @ " << bestAskPrice << "\n";*/

            // Update quantities
            order.quantity -= quantityTraded;
            sellOrder.quantity -= quantityTraded;

            // If the Sell order is fully filled, remove it
            if (sellOrder.quantity == 0) {
                sellOrders.erase(sellOrders.begin());
                // If no more orders at this price, remove the price level
                if (sellOrders.empty()) {
                    asks.erase(bestAskIt);
                }
            }
        }

        // If Buy order is still not filled, add remainder to the Book
        if (order.quantity > 0) {
            bids[order.price].push_back(order); // Add to Bids Map
            //std::cout << "[BOOK] Buy Order " << order.id << " added to Bids @ " << order.price << "\n";
        }
    }

    // Matching Logic for Incoming SELL Orders (Mirror of Buy)
    void matchSellOrder(Order& order) {
        while (order.quantity > 0 && !bids.empty()) {
            auto bestBidIt = bids.begin();
            double bestBidPrice = bestBidIt->first;
            std::vector<Order>& buyOrders = bestBidIt->second;

            if (bestBidPrice < order.price) { // Buyer is offering too little
                break;
            }

            Order& buyOrder = buyOrders.front();
            int quantityTraded = std::min(order.quantity, buyOrder.quantity);

           /* std::cout << "[TRADE] Sell Order " << order.id << " matched with Buy Order "
                << buyOrder.id << " for " << quantityTraded << " @ " << bestBidPrice << "\n";*/

            order.quantity -= quantityTraded;
            buyOrder.quantity -= quantityTraded;

            if (buyOrder.quantity == 0) {
                buyOrders.erase(buyOrders.begin());
                if (buyOrders.empty()) {
                    bids.erase(bestBidIt);
                }
            }
        }

        if (order.quantity > 0) {
            asks[order.price].push_back(order);
            //std::cout << "[BOOK] Sell Order " << order.id << " added to Asks @ " << order.price << "\n";
        }
    }
};

#endif