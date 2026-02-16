#include "DatabaseManager.h"

using namespace System::IO;
using namespace System::Windows::Forms;

namespace ATSProject {
    namespace Database {

        DatabaseManager::DatabaseManager() {
            String^ dbPath = Path::Combine(Application::StartupPath, "ats_v2.db");
            connectionString = "Data Source=" + dbPath + ";Version=3;";
            InitializeDatabase();
        }

        DatabaseManager^ DatabaseManager::GetInstance() {
            if (instance == nullptr) {
                instance = gcnew DatabaseManager();
            }
            return instance;
        }

        void DatabaseManager::InitializeDatabase() {
            try {
                String^ dbPath = Path::Combine(Application::StartupPath, "ats_v2.db");
                if (!File::Exists(dbPath)) {
                    SQLiteConnection::CreateFile(dbPath);
                }

                SQLiteConnection^ connection = gcnew SQLiteConnection(connectionString);
                connection->Open();

                String^ createClients = "CREATE TABLE IF NOT EXISTS Clients (Id INTEGER PRIMARY KEY AUTOINCREMENT, LastName TEXT, FirstName TEXT, PhoneNumber TEXT)";
                String^ createTariffs = "CREATE TABLE IF NOT EXISTS Tariffs (Id INTEGER PRIMARY KEY AUTOINCREMENT, City TEXT, BaseCostPerMinute REAL, StrategyType TEXT, DiscountRate REAL)";
                String^ createCalls = "CREATE TABLE IF NOT EXISTS Calls (Id INTEGER PRIMARY KEY AUTOINCREMENT, ClientId INTEGER, TariffId INTEGER, City TEXT, Duration INTEGER, Cost REAL)";

                (gcnew SQLiteCommand(createClients, connection))->ExecuteNonQuery();
                (gcnew SQLiteCommand(createTariffs, connection))->ExecuteNonQuery();
                (gcnew SQLiteCommand(createCalls, connection))->ExecuteNonQuery();

                connection->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("DB Init Error: " + ex->Message);
            }
        }

        // --- Clients ---
        void DatabaseManager::AddClient(Models::Client^ client) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("INSERT INTO Clients (LastName, FirstName, PhoneNumber) VALUES (@ln, @fn, @ph)", conn);
                cmd->Parameters->AddWithValue("@ln", client->LastName);
                cmd->Parameters->AddWithValue("@fn", client->FirstName);
                cmd->Parameters->AddWithValue("@ph", client->PhoneNumber);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error adding client: " + ex->Message);
            }
        }

        List<Models::Client^>^ DatabaseManager::GetAllClients() {
            List<Models::Client^>^ list = gcnew List<Models::Client^>();
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT * FROM Clients ORDER BY Id", conn);
                SQLiteDataReader^ r = cmd->ExecuteReader();
                while (r->Read()) {
                    Models::Client^ c = gcnew Models::Client();
                    c->Id = r->GetInt32(0);
                    c->LastName = r->GetString(1);
                    c->FirstName = r->GetString(2);
                    c->PhoneNumber = r->GetString(3);
                    list->Add(c);
                }
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error getting clients: " + ex->Message);
            }
            return list;
        }void DatabaseManager::UpdateClient(Models::Client^ client) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("UPDATE Clients SET LastName=@ln, FirstName=@fn, PhoneNumber=@ph WHERE Id=@id", conn);
                cmd->Parameters->AddWithValue("@ln", client->LastName);
                cmd->Parameters->AddWithValue("@fn", client->FirstName);
                cmd->Parameters->AddWithValue("@ph", client->PhoneNumber);
                cmd->Parameters->AddWithValue("@id", client->Id);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error updating client: " + ex->Message);
            }
        }

        void DatabaseManager::DeleteClient(int clientId) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                // Сначала удаляем все звонки клиента
                SQLiteCommand^ cmdCalls = gcnew SQLiteCommand("DELETE FROM Calls WHERE ClientId=@id", conn);
                cmdCalls->Parameters->AddWithValue("@id", clientId);
                cmdCalls->ExecuteNonQuery();

                // Затем удаляем самого клиента
                SQLiteCommand^ cmd = gcnew SQLiteCommand("DELETE FROM Clients WHERE Id=@id", conn);
                cmd->Parameters->AddWithValue("@id", clientId);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error deleting client: " + ex->Message);
            }
        }

        Models::Client^ DatabaseManager::FindClient(int clientId) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT * FROM Clients WHERE Id=@id", conn);
                cmd->Parameters->AddWithValue("@id", clientId);
                SQLiteDataReader^ r = cmd->ExecuteReader();
                if (r->Read()) {
                    Models::Client^ c = gcnew Models::Client();
                    c->Id = r->GetInt32(0);
                    c->LastName = r->GetString(1);
                    c->FirstName = r->GetString(2);
                    c->PhoneNumber = r->GetString(3);
                    conn->Close();
                    return c;
                }
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error finding client: " + ex->Message);
            }
            return nullptr;
        }

        // --- Tariffs ---
        void DatabaseManager::AddTariff(Models::Tariff^ tariff) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("INSERT INTO Tariffs (City, BaseCostPerMinute, StrategyType, DiscountRate) VALUES (@ct, @cost, @st, @dr)", conn);
                cmd->Parameters->AddWithValue("@ct", tariff->City);
                cmd->Parameters->AddWithValue("@cost", tariff->BaseCostPerMinute);
                cmd->Parameters->AddWithValue("@st", tariff->StrategyType);
                cmd->Parameters->AddWithValue("@dr", tariff->DiscountRate.HasValue ? (Object^)tariff->DiscountRate.Value : nullptr);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error adding tariff: " + ex->Message);
            }
        }List<Models::Tariff^>^ DatabaseManager::GetAllTariffs() {
            List<Models::Tariff^>^ list = gcnew List<Models::Tariff^>();
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT * FROM Tariffs ORDER BY Id", conn);
                SQLiteDataReader^ r = cmd->ExecuteReader();
                while (r->Read()) {
                    Models::Tariff^ t = gcnew Models::Tariff();
                    t->Id = r->GetInt32(0);
                    t->City = r->GetString(1);
                    t->BaseCostPerMinute = r->GetDouble(2);
                    t->StrategyType = r->GetString(3);
                    if (!r->IsDBNull(4)) t->DiscountRate = r->GetDouble(4);
                    list->Add(t);
                }
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error getting tariffs: " + ex->Message);
            }
            return list;
        }

        void DatabaseManager::UpdateTariff(Models::Tariff^ tariff) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("UPDATE Tariffs SET City=@ct, BaseCostPerMinute=@cost, StrategyType=@st, DiscountRate=@dr WHERE Id=@id", conn);
                cmd->Parameters->AddWithValue("@ct", tariff->City);
                cmd->Parameters->AddWithValue("@cost", tariff->BaseCostPerMinute);
                cmd->Parameters->AddWithValue("@st", tariff->StrategyType);
                cmd->Parameters->AddWithValue("@dr", tariff->DiscountRate.HasValue ? (Object^)tariff->DiscountRate.Value : nullptr);
                cmd->Parameters->AddWithValue("@id", tariff->Id);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error updating tariff: " + ex->Message);
            }
        }

        void DatabaseManager::DeleteTariff(int tariffId) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                // Сначала удаляем все звонки с этим тарифом
                SQLiteCommand^ cmdCalls = gcnew SQLiteCommand("DELETE FROM Calls WHERE TariffId=@id", conn);
                cmdCalls->Parameters->AddWithValue("@id", tariffId);
                cmdCalls->ExecuteNonQuery();

                // Затем удаляем сам тариф
                SQLiteCommand^ cmd = gcnew SQLiteCommand("DELETE FROM Tariffs WHERE Id=@id", conn);
                cmd->Parameters->AddWithValue("@id", tariffId);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error deleting tariff: " + ex->Message);
            }
        }Models::Tariff^ DatabaseManager::FindTariff(int tariffId) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT * FROM Tariffs WHERE Id=@id", conn);
                cmd->Parameters->AddWithValue("@id", tariffId);
                SQLiteDataReader^ r = cmd->ExecuteReader();
                if (r->Read()) {
                    Models::Tariff^ t = gcnew Models::Tariff();
                    t->Id = r->GetInt32(0);
                    t->City = r->GetString(1);
                    t->BaseCostPerMinute = r->GetDouble(2);
                    t->StrategyType = r->GetString(3);
                    if (!r->IsDBNull(4)) t->DiscountRate = r->GetDouble(4);
                    conn->Close();
                    return t;
                }
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error finding tariff: " + ex->Message);
            }
            return nullptr;
        }

        // --- Calls ---
        void DatabaseManager::AddCall(Models::Call^ call) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("INSERT INTO Calls (ClientId, TariffId, City, Duration, Cost) VALUES (@cid, @tid, @ct, @dur, @cost)", conn);
                cmd->Parameters->AddWithValue("@cid", call->ClientId);
                cmd->Parameters->AddWithValue("@tid", call->TariffId);
                cmd->Parameters->AddWithValue("@ct", call->City);
                cmd->Parameters->AddWithValue("@dur", call->Duration);
                cmd->Parameters->AddWithValue("@cost", call->Cost);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error adding call: " + ex->Message);
            }
        }

        List<Models::Call^>^ DatabaseManager::GetAllCalls() {
            List<Models::Call^>^ list = gcnew List<Models::Call^>();
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT * FROM Calls ORDER BY Id", conn);
                SQLiteDataReader^ r = cmd->ExecuteReader();
                while (r->Read()) {
                    Models::Call^ c = gcnew Models::Call();
                    c->Id = r->GetInt32(0);
                    c->ClientId = r->GetInt32(1);
                    c->TariffId = r->GetInt32(2);
                    c->City = r->GetString(3);
                    c->Duration = r->GetInt32(4);
                    c->Cost = r->GetDouble(5);
                    list->Add(c);
                }
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error getting calls: " + ex->Message);
            }
            return list;
        }List<Models::Call^>^ DatabaseManager::GetCallsByClient(int clientId) {
            List<Models::Call^>^ list = gcnew List<Models::Call^>();
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT * FROM Calls WHERE ClientId=@cid ORDER BY Id", conn);
                cmd->Parameters->AddWithValue("@cid", clientId);
                SQLiteDataReader^ r = cmd->ExecuteReader();
                while (r->Read()) {
                    Models::Call^ c = gcnew Models::Call();
                    c->Id = r->GetInt32(0);
                    c->ClientId = r->GetInt32(1);
                    c->TariffId = r->GetInt32(2);
                    c->City = r->GetString(3);
                    c->Duration = r->GetInt32(4);
                    c->Cost = r->GetDouble(5);
                    list->Add(c);
                }
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error getting calls by client: " + ex->Message);
            }
            return list;
        }

        void DatabaseManager::DeleteCall(int callId) {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("DELETE FROM Calls WHERE Id=@id", conn);
                cmd->Parameters->AddWithValue("@id", callId);
                cmd->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error deleting call: " + ex->Message);
            }
        }

        // --- Additional Methods ---
        void DatabaseManager::ClearDatabase() {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                (gcnew SQLiteCommand("DELETE FROM Calls", conn))->ExecuteNonQuery();
                (gcnew SQLiteCommand("DELETE FROM Clients", conn))->ExecuteNonQuery();
                (gcnew SQLiteCommand("DELETE FROM Tariffs", conn))->ExecuteNonQuery();
                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error clearing database: " + ex->Message);
            }
        }

        int DatabaseManager::GetTotalClients() {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT COUNT(*) FROM Clients", conn);
                int count = Convert::ToInt32(cmd->ExecuteScalar());
                conn->Close();
                return count;
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error getting total clients: " + ex->Message);
                return 0;
            }
        }

        int DatabaseManager::GetTotalTariffs() {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT COUNT(*) FROM Tariffs", conn);
                int count = Convert::ToInt32(cmd->ExecuteScalar());
                conn->Close();
                return count;
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error getting total tariffs: " + ex->Message);
                return 0;
            }
        }int DatabaseManager::GetTotalCalls() {
            try {
                SQLiteConnection^ conn = gcnew SQLiteConnection(connectionString);
                conn->Open();
                SQLiteCommand^ cmd = gcnew SQLiteCommand("SELECT COUNT(*) FROM Calls", conn);
                int count = Convert::ToInt32(cmd->ExecuteScalar());
                conn->Close();
                return count;
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error getting total calls: " + ex->Message);
                return 0;
            }
        }

        bool DatabaseManager::ExportDataToFile(String^ filePath) {
            try {
                StreamWriter^ sw = gcnew StreamWriter(filePath, false, System::Text::Encoding::UTF8);
                auto clients = GetAllClients();
                sw->WriteLine(L"[CLIENTS]");
                sw->WriteLine(L"Id;LastName;FirstName;PhoneNumber");
                for each (Models::Client^ c in clients) {
                    sw->WriteLine(String::Format(L"{0};{1};{2};{3}", c->Id, EscapeCsv(c->LastName), EscapeCsv(c->FirstName), EscapeCsv(c->PhoneNumber)));
                }
                auto tariffs = GetAllTariffs();
                sw->WriteLine(L"[TARIFFS]");
                sw->WriteLine(L"Id;City;BaseCostPerMinute;StrategyType;DiscountRate");
                for each (Models::Tariff^ t in tariffs) {
                    String^ dr = t->DiscountRate.HasValue ? t->DiscountRate.Value.ToString() : L"";
                    sw->WriteLine(String::Format(L"{0};{1};{2};{3};{4}", t->Id, EscapeCsv(t->City), t->BaseCostPerMinute, EscapeCsv(t->StrategyType), dr));
                }
                auto calls = GetAllCalls();
                sw->WriteLine(L"[CALLS]");
                sw->WriteLine(L"Id;ClientId;TariffId;City;Duration;Cost");
                for each (Models::Call^ c in calls) {
                    sw->WriteLine(String::Format(L"{0};{1};{2};{3};{4};{5}", c->Id, c->ClientId, c->TariffId, EscapeCsv(c->City), c->Duration, c->Cost));
                }
                sw->Close();
                return true;
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"?????? ????????: " + ex->Message);
                return false;
            }
        }

        String^ DatabaseManager::EscapeCsv(String^ s) {
            if (String::IsNullOrEmpty(s)) return s;
            return s->Replace(L";", L",")->Replace(L"\r", L" ")->Replace(L"\n", L" ");
        }

        bool DatabaseManager::ImportDataFromFile(String^ filePath) {
            try {
                if (!File::Exists(filePath)) {
                    MessageBox::Show(L"???? ?? ??????.");
                    return false;
                }
                StreamReader^ sr = gcnew StreamReader(filePath, System::Text::Encoding::UTF8);
                ClearDatabase();
                String^ section = L"";
                while (sr->Peek() >= 0) {
                    String^ line = sr->ReadLine();
                    if (String::IsNullOrWhiteSpace(line)) continue;
                    if (line->StartsWith(L"[")) {
                        section = line->Trim()->ToUpper();
                        continue;
                    }
                    if (section == L"[CLIENTS]") {
                        if (line->Contains(L"LastName")) continue;
                        array<String^>^ parts = line->Split(L';');
                        if (parts->Length >= 4) {
                            Models::Client^ c = gcnew Models::Client();
                            c->LastName = parts[1]->Trim();
                            c->FirstName = parts[2]->Trim();
                            c->PhoneNumber = parts[3]->Trim();
                            AddClient(c);
                        }
                    }
                    else if (section == L"[TARIFFS]") {
                        if (line->Contains(L"City")) continue;
                        array<String^>^ parts = line->Split(L';');
                        if (parts->Length >= 5) {
                            Models::Tariff^ t = gcnew Models::Tariff();
                            t->City = parts[1]->Trim();
                            Double::TryParse(parts[2]->Trim(), t->BaseCostPerMinute);
                            t->StrategyType = parts[3]->Trim();
                            double dr;
                            if (Double::TryParse(parts[4]->Trim(), dr)) t->DiscountRate = dr;
                            AddTariff(t);
                        }
                    }
                    else if (section == L"[CALLS]") {
                        if (line->Contains(L"ClientId")) continue;
                        array<String^>^ parts = line->Split(L';');
                        if (parts->Length >= 6) {
                            Models::Call^ call = gcnew Models::Call();
                            Int32::TryParse(parts[1]->Trim(), call->ClientId);
                            Int32::TryParse(parts[2]->Trim(), call->TariffId);
                            call->City = parts[3]->Trim();
                            Int32::TryParse(parts[4]->Trim(), call->Duration);
                            Double::TryParse(parts[5]->Trim(), call->Cost);
                            AddCall(call);
                        }
                    }
                }
                sr->Close();
                return true;
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"?????? ???????: " + ex->Message);
                return false;
            }
        }
    }
}