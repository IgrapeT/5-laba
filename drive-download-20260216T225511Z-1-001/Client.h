#pragma once
using namespace System;

namespace ATSProject {
    namespace Models {
        public ref class Client {
        public:
            int Id;
            String^ LastName;
            String^ FirstName;
            String^ PhoneNumber;

            Client() {}

            Client(String^ lastName, String^ firstName, String^ phone) {
                LastName = lastName;
                FirstName = firstName;
                PhoneNumber = phone;
            }

            property String^ FullName {
                String^ get() { return LastName + " " + FirstName; }
            }
        };
    }
}