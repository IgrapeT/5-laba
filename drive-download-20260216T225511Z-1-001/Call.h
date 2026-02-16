#pragma once
using namespace System;

namespace ATSProject {
    namespace Models {
        public ref class Call {
        public:
            int Id;
            int ClientId;
            int TariffId;
            String^ City;
            int Duration;
            double Cost;

            Call() {}
        };
    }
}