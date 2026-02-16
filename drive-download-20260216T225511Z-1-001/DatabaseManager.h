#pragma once

#include "Client.h"
#include "Tariff.h"
#include "Call.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Data::SQLite;

namespace ATSProject {
    namespace Database {
        public ref class DatabaseManager {
        private:
            String^ connectionString;
            static DatabaseManager^ instance;

            DatabaseManager();
            void InitializeDatabase();
            String^ EscapeCsv(String^ s);

        public:
            static DatabaseManager^ GetInstance();

            // ?????? ??? ?????? ? ?????????
            void AddClient(Models::Client^ client);
            List<Models::Client^>^ GetAllClients();
            void UpdateClient(Models::Client^ client);
            void DeleteClient(int clientId);
            Models::Client^ FindClient(int clientId);

            // ?????? ??? ?????? ? ????????
            void AddTariff(Models::Tariff^ tariff);
            List<Models::Tariff^>^ GetAllTariffs();
            void UpdateTariff(Models::Tariff^ tariff);
            void DeleteTariff(int tariffId);
            Models::Tariff^ FindTariff(int tariffId);

            // ?????? ??? ?????? ?? ????????
            void AddCall(Models::Call^ call);
            List<Models::Call^>^ GetAllCalls();
            List<Models::Call^>^ GetCallsByClient(int clientId);
            void DeleteCall(int callId);

            // ?????????????? ??????
            void ClearDatabase();
            int GetTotalClients();
            int GetTotalTariffs();
            int GetTotalCalls();

            // ???????/?????? ?????? ? ????
            bool ExportDataToFile(String^ filePath);
            bool ImportDataFromFile(String^ filePath);
        };
    }
}