#pragma once
using namespace System;

namespace ATSProject {
    namespace Models {
        public ref class Tariff {
        public:
            int Id;
            String^ City;
            double BaseCostPerMinute;
            String^ StrategyType;
            Nullable<double> DiscountRate;

            Tariff() {}

            Tariff(String^ city, double cost, String^ strategy) {
                City = city;
                BaseCostPerMinute = cost;
                StrategyType = strategy;
            }
        };
    }
}