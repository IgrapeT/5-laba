#pragma once
#include "DatabaseManager.h"
#include "Client.h"
#include "Tariff.h"
#include "Call.h"
#include "AddClientForm.h"
#include "AddTariffForm.h"

namespace ATSProject {
    namespace Forms {
        using namespace System;
        using namespace System::ComponentModel;
        using namespace System::Collections;
        using namespace System::Collections::Generic;
        using namespace System::Windows::Forms;
        using namespace System::Data;
        using namespace System::Drawing;
        using namespace System::IO;

        public ref class MainForm : public System::Windows::Forms::Form {
        private:
            // ???????
            System::Windows::Forms::DataGridView^ dgvClients;
            System::Windows::Forms::DataGridView^ dgvTariffs;
            System::Windows::Forms::DataGridView^ dgvCalls;
            System::Windows::Forms::DataGridView^ dgvDBInfo;

            // ?????? ??? ????????
            System::Windows::Forms::Button^ btnAddClient;
            System::Windows::Forms::Button^ btnEditClient;
            System::Windows::Forms::Button^ btnDeleteClient;
            System::Windows::Forms::Button^ btnMakeCall;

            // ?????? ??? ???????
            System::Windows::Forms::Button^ btnAddTariff;
            System::Windows::Forms::Button^ btnEditTariff;
            System::Windows::Forms::Button^ btnDeleteTariff;

            // ?????? ??? ???????
            System::Windows::Forms::Button^ btnDeleteCall;
            System::Windows::Forms::Button^ btnViewClientCalls;

            // ?????? ??? ??
            System::Windows::Forms::Button^ btnClearDB;
            System::Windows::Forms::Button^ btnRefresh;
            System::Windows::Forms::Button^ btnSaveToFile;
            System::Windows::Forms::Button^ btnLoadFromFile;

            // TabControl
            System::Windows::Forms::TabControl^ tabControl;
            System::Windows::Forms::TabPage^ tabPageClients;
            System::Windows::Forms::TabPage^ tabPageTariffs;
            System::Windows::Forms::TabPage^ tabPageCalls;
            System::Windows::Forms::TabPage^ tabPageDBInfo;

            // ????? ? ???????? ??????????
            System::Windows::Forms::Label^ lblUpdateTime;

        public:
            MainForm() {
                InitializeComponent();
                LoadData();
            }

        protected:
            ~MainForm() {
                if (components) {
                    delete components;
                }
            }

        private:
            System::ComponentModel::Container^ components;

            void InitializeComponent() {
                this->Text = L"\u0410\u0422\u0421 \u0423\u043f\u0440\u0430\u0432\u043b\u0435\u043d\u0438\u0435";
                this->Size = System::Drawing::Size(1000, 650);
                this->StartPosition = FormStartPosition::CenterScreen;

                tabControl = gcnew TabControl();
                tabControl->Dock = DockStyle::Fill;

                tabPageClients = gcnew TabPage(L"\u041a\u043b\u0438\u0435\u043d\u0442\u044b");
                CreateClientsTab();

                tabPageTariffs = gcnew TabPage(L"\u0422\u0430\u0440\u0438\u0444\u044b");
                CreateTariffsTab();

                tabPageCalls = gcnew TabPage(L"\u0417\u0432\u043e\u043d\u043a\u0438");
                CreateCallsTab();

                tabPageDBInfo = gcnew TabPage(L"\u0418\u043d\u0444\u043e\u0440\u043c\u0430\u0446\u0438\u044f \u0411\u0414");
                CreateDBInfoTab();

                tabControl->TabPages->Add(tabPageClients);
                tabControl->TabPages->Add(tabPageTariffs);
                tabControl->TabPages->Add(tabPageCalls);
                tabControl->TabPages->Add(tabPageDBInfo);

                Panel^ bottomPanel = gcnew Panel();
                bottomPanel->Dock = DockStyle::Bottom;
                bottomPanel->Height = 60;

                btnRefresh = CreateButton(L"\u041e\u0431\u043d\u043e\u0432\u0438\u0442\u044c \u0434\u0430\u043d\u043d\u044b\u0435", 10, 15, 150, 30);
                btnRefresh->Click += gcnew EventHandler(this, &MainForm::OnRefresh);
                bottomPanel->Controls->Add(btnRefresh);

                btnClearDB = CreateButton(L"\u041e\u0447\u0438\u0441\u0442\u0438\u0442\u044c \u0411\u0414", 180, 15, 120, 30);
                btnClearDB->Click += gcnew EventHandler(this, &MainForm::OnClearDB);
                bottomPanel->Controls->Add(btnClearDB);

                btnSaveToFile = CreateButton(L"\u0421\u043e\u0445\u0440\u0430\u043d\u0438\u0442\u044c \u0432 \u0444\u0430\u0439\u043b", 320, 15, 130, 30);
                btnSaveToFile->Click += gcnew EventHandler(this, &MainForm::OnSaveToFile);
                bottomPanel->Controls->Add(btnSaveToFile);

                btnLoadFromFile = CreateButton(L"\u0417\u0430\u0433\u0440\u0443\u0437\u0438\u0442\u044c \u0438\u0437 \u0444\u0430\u0439\u043b\u0430", 460, 15, 130, 30);
                btnLoadFromFile->Click += gcnew EventHandler(this, &MainForm::OnLoadFromFile);
                bottomPanel->Controls->Add(btnLoadFromFile);

                lblUpdateTime = gcnew Label();
                lblUpdateTime->Dock = DockStyle::Bottom;
                lblUpdateTime->TextAlign = ContentAlignment::MiddleRight;
                lblUpdateTime->Height = 25;
                lblUpdateTime->BackColor = System::Drawing::SystemColors::ControlDark;
                lblUpdateTime->Padding = System::Windows::Forms::Padding(0, 0, 10, 0);

                this->Controls->Add(tabControl);
                this->Controls->Add(bottomPanel);
                this->Controls->Add(lblUpdateTime);

                UpdateTimestamp();
            }

            void CreateClientsTab() {
                // ??????? ????????
                dgvClients = gcnew DataGridView();
                dgvClients->Dock = DockStyle::Fill;
                dgvClients->AllowUserToAddRows = false;
                dgvClients->AllowUserToDeleteRows = false;
                dgvClients->ReadOnly = true;
                dgvClients->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
                dgvClients->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
                dgvClients->MultiSelect = false;

                // ?????? ? ????????
                Panel^ buttonPanel = gcnew Panel();
                buttonPanel->Dock = DockStyle::Bottom;
                buttonPanel->Height = 50;

                btnAddClient = CreateButton(L"\u0414\u043e\u0431\u0430\u0432\u0438\u0442\u044c", 10, 10, 120, 30);
                btnEditClient = CreateButton(L"\u0418\u0437\u043c\u0435\u043d\u0438\u0442\u044c", 140, 10, 120, 30);
                btnDeleteClient = CreateButton(L"\u0423\u0434\u0430\u043b\u0438\u0442\u044c", 270, 10, 120, 30);
                btnMakeCall = CreateButton(L"\u0421\u043e\u0432\u0435\u0440\u0448\u0438\u0442\u044c \u0437\u0432\u043e\u043d\u043e\u043a", 400, 10, 150, 30);

                btnAddClient->Click += gcnew EventHandler(this, &MainForm::OnAddClient);
                btnEditClient->Click += gcnew EventHandler(this, &MainForm::OnEditClient);
                btnDeleteClient->Click += gcnew EventHandler(this, &MainForm::OnDeleteClient);
                btnMakeCall->Click += gcnew EventHandler(this, &MainForm::OnMakeCall);

                buttonPanel->Controls->Add(btnAddClient);
                buttonPanel->Controls->Add(btnEditClient);
                buttonPanel->Controls->Add(btnDeleteClient);
                buttonPanel->Controls->Add(btnMakeCall);

                tabPageClients->Controls->Add(dgvClients);
                tabPageClients->Controls->Add(buttonPanel);
            }

            void CreateTariffsTab() {
                // ??????? ???????
                dgvTariffs = gcnew DataGridView();
                dgvTariffs->Dock = DockStyle::Fill;
                dgvTariffs->AllowUserToAddRows = false;
                dgvTariffs->AllowUserToDeleteRows = false;
                dgvTariffs->ReadOnly = true;
                dgvTariffs->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
                dgvTariffs->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
                dgvTariffs->MultiSelect = false;

                // ?????? ? ????????
                Panel^ buttonPanel = gcnew Panel();
                buttonPanel->Dock = DockStyle::Bottom;
                buttonPanel->Height = 50;

                btnAddTariff = CreateButton(L"\u0414\u043e\u0431\u0430\u0432\u0438\u0442\u044c", 10, 10, 120, 30);
                btnEditTariff = CreateButton(L"\u0418\u0437\u043c\u0435\u043d\u0438\u0442\u044c", 140, 10, 120, 30);
                btnDeleteTariff = CreateButton(L"\u0423\u0434\u0430\u043b\u0438\u0442\u044c", 270, 10, 120, 30);

                btnAddTariff->Click += gcnew EventHandler(this, &MainForm::OnAddTariff);
                btnEditTariff->Click += gcnew EventHandler(this, &MainForm::OnEditTariff);
                btnDeleteTariff->Click += gcnew EventHandler(this, &MainForm::OnDeleteTariff);

                buttonPanel->Controls->Add(btnAddTariff);
                buttonPanel->Controls->Add(btnEditTariff);
                buttonPanel->Controls->Add(btnDeleteTariff);

                tabPageTariffs->Controls->Add(dgvTariffs);
                tabPageTariffs->Controls->Add(buttonPanel);
            }

            void CreateCallsTab() {
                // ??????? ???????
                dgvCalls = gcnew DataGridView();
                dgvCalls->Dock = DockStyle::Fill;
                dgvCalls->AllowUserToAddRows = false;
                dgvCalls->AllowUserToDeleteRows = false;
                dgvCalls->ReadOnly = true;
                dgvCalls->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
                dgvCalls->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
                dgvCalls->MultiSelect = false;

                // ?????? ? ????????
                Panel^ buttonPanel = gcnew Panel();
                buttonPanel->Dock = DockStyle::Bottom;
                buttonPanel->Height = 50;

                btnDeleteCall = CreateButton(L"\u0423\u0434\u0430\u043b\u0438\u0442\u044c \u0437\u0432\u043e\u043d\u043e\u043a", 10, 10, 150, 30);
                btnViewClientCalls = CreateButton(L"\u0417\u0432\u043e\u043d\u043a\u0438 \u043a\u043b\u0438\u0435\u043d\u0442\u0430", 170, 10, 150, 30);

                btnDeleteCall->Click += gcnew EventHandler(this, &MainForm::OnDeleteCall);
                btnViewClientCalls->Click += gcnew EventHandler(this, &MainForm::OnViewClientCalls);

                buttonPanel->Controls->Add(btnDeleteCall);
                buttonPanel->Controls->Add(btnViewClientCalls);

                tabPageCalls->Controls->Add(dgvCalls);
                tabPageCalls->Controls->Add(buttonPanel);
            }

            void CreateDBInfoTab() {
                // ??????? ??????????
                dgvDBInfo = gcnew DataGridView();
                dgvDBInfo->Dock = DockStyle::Fill;
                dgvDBInfo->AllowUserToAddRows = false;
                dgvDBInfo->AllowUserToDeleteRows = false;
                dgvDBInfo->ReadOnly = true;
                dgvDBInfo->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

                tabPageDBInfo->Controls->Add(dgvDBInfo);
            }

            Button^ CreateButton(String^ text, int x, int y, int width, int height) {
                Button^ btn = gcnew Button();
                btn->Text = text;
                btn->Location = Point(x, y);
                btn->Size = System::Drawing::Size(width, height);
                return btn;
            }

            void UpdateTimestamp() {
                lblUpdateTime->Text = L"\u041e\u0431\u043d\u043e\u0432\u043b\u0435\u043d\u043e: " + DateTime::Now.ToString(L"dd.MM.yyyy HH:mm:ss");
            }

            void LoadData() {
                LoadClients();
                LoadTariffs();
                LoadCalls();
                LoadDBInfo();
            }

            void LoadClients() {
                try {
                    auto db = Database::DatabaseManager::GetInstance();
                    auto clients = db->GetAllClients();

                    DataTable^ dt = gcnew DataTable();
                    dt->Columns->Add("ID", int::typeid);
                    dt->Columns->Add(L"\u0424\u0430\u043c\u0438\u043b\u0438\u044f", String::typeid);
                    dt->Columns->Add(L"\u0418\u043c\u044f", String::typeid);
                    dt->Columns->Add(L"\u0422\u0435\u043b\u0435\u0444\u043e\u043d", String::typeid);

                    for each (auto client in clients) {
                        DataRow^ row = dt->NewRow();
                        row["ID"] = client->Id;
                        row[L"\u0424\u0430\u043c\u0438\u043b\u0438\u044f"] = client->LastName;
                        row[L"\u0418\u043c\u044f"] = client->FirstName;
                        row[L"\u0422\u0435\u043b\u0435\u0444\u043e\u043d"] = client->PhoneNumber;
                        dt->Rows->Add(row);
                    }

                    dgvClients->DataSource = dt;
                    for each (DataGridViewColumn^ col in dgvClients->Columns) {
                        col->SortMode = DataGridViewColumnSortMode::Automatic;
                    }
                }
                catch (Exception^ ex) {
                    MessageBox::Show(L"\u041e\u0448\u0438\u0431\u043a\u0430 \u0437\u0430\u0433\u0440\u0443\u0437\u043a\u0438 \u043a\u043b\u0438\u0435\u043d\u0442\u043e\u0432: " + ex->Message);
                }
            }

            void LoadTariffs() {
                try {
                    auto db = Database::DatabaseManager::GetInstance();
                    auto tariffs = db->GetAllTariffs();

                    DataTable^ dt = gcnew DataTable();
                    dt->Columns->Add("ID", int::typeid);
                    dt->Columns->Add(L"\u0413\u043e\u0440\u043e\u0434", String::typeid);
                    dt->Columns->Add(L"\u0426\u0435\u043d\u0430/\u043c\u0438\u043d", double::typeid);
                    dt->Columns->Add(L"\u0422\u0438\u043f", String::typeid);
                    dt->Columns->Add(L"\u0421\u043a\u0438\u0434\u043a\u0430 %", String::typeid);

                    for each (auto tariff in tariffs) {
                        DataRow^ row = dt->NewRow();
                        row["ID"] = tariff->Id;
                        row[L"\u0413\u043e\u0440\u043e\u0434"] = tariff->City;
                        row[L"\u0426\u0435\u043d\u0430/\u043c\u0438\u043d"] = tariff->BaseCostPerMinute;
                        row[L"\u0422\u0438\u043f"] = tariff->StrategyType;
                        row[L"\u0421\u043a\u0438\u0434\u043a\u0430 %"] = tariff->DiscountRate.HasValue ?
                            tariff->DiscountRate.Value.ToString("F2") : "-";
                        dt->Rows->Add(row);
                    }

                    dgvTariffs->DataSource = dt;
                    for each (DataGridViewColumn^ col in dgvTariffs->Columns) {
                        col->SortMode = DataGridViewColumnSortMode::Automatic;
                    }
                }
                catch (Exception^ ex) {
                    MessageBox::Show(L"\u041e\u0448\u0438\u0431\u043a\u0430 \u0437\u0430\u0433\u0440\u0443\u0437\u043a\u0438 \u0442\u0430\u0440\u0438\u0444\u043e\u0432: " + ex->Message);
                }
            }

            void LoadCalls() {
                try {
                    auto db = Database::DatabaseManager::GetInstance();
                    auto calls = db->GetAllCalls();

                    DataTable^ dt = gcnew DataTable();
                    dt->Columns->Add("ID", int::typeid);
                    dt->Columns->Add(L"ID \u041a\u043b\u0438\u0435\u043d\u0442\u0430", int::typeid);
                    dt->Columns->Add(L"ID \u0422\u0430\u0440\u0438\u0444\u0430", int::typeid);
                    dt->Columns->Add(L"\u0413\u043e\u0440\u043e\u0434", String::typeid);
                    dt->Columns->Add(L"\u0414\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0441\u0442\u044c (\u043c\u0438\u043d)", int::typeid);
                    dt->Columns->Add(L"\u0421\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c", double::typeid);

                    for each (auto call in calls) {
                        DataRow^ row = dt->NewRow();
                        row["ID"] = call->Id;
                        row[L"ID \u041a\u043b\u0438\u0435\u043d\u0442\u0430"] = call->ClientId;
                        row[L"ID \u0422\u0430\u0440\u0438\u0444\u0430"] = call->TariffId;
                        row[L"\u0413\u043e\u0440\u043e\u0434"] = call->City;
                        row[L"\u0414\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0441\u0442\u044c (\u043c\u0438\u043d)"] = call->Duration;
                        row[L"\u0421\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c"] = call->Cost;
                        dt->Rows->Add(row);
                    }

                    dgvCalls->DataSource = dt;
                    for each (DataGridViewColumn^ col in dgvCalls->Columns) {
                        col->SortMode = DataGridViewColumnSortMode::Automatic;
                    }
                }
                catch (Exception^ ex) {
                    MessageBox::Show(L"\u041e\u0448\u0438\u0431\u043a\u0430 \u0437\u0430\u0433\u0440\u0443\u0437\u043a\u0438 \u0437\u0432\u043e\u043d\u043a\u043e\u0432: " + ex->Message);
                }
            }

            void LoadDBInfo() {
                try {
                    auto db = Database::DatabaseManager::GetInstance();

                    DataTable^ dt = gcnew DataTable();
                    dt->Columns->Add(L"\u041f\u0430\u0440\u0430\u043c\u0435\u0442\u0440", String::typeid);
                    dt->Columns->Add(L"\u0417\u043d\u0430\u0447\u0435\u043d\u0438\u0435", String::typeid);

                    DataRow^ row1 = dt->NewRow();
                    row1[L"\u041f\u0430\u0440\u0430\u043c\u0435\u0442\u0440"] = L"\u0412\u0441\u0435\u0433\u043e \u043a\u043b\u0438\u0435\u043d\u0442\u043e\u0432";
                    row1[L"\u0417\u043d\u0430\u0447\u0435\u043d\u0438\u0435"] = db->GetTotalClients().ToString();
                    dt->Rows->Add(row1);

                    DataRow^ row2 = dt->NewRow();
                    row2[L"\u041f\u0430\u0440\u0430\u043c\u0435\u0442\u0440"] = L"\u0412\u0441\u0435\u0433\u043e \u0442\u0430\u0440\u0438\u0444\u043e\u0432";
                    row2[L"\u0417\u043d\u0430\u0447\u0435\u043d\u0438\u0435"] = db->GetTotalTariffs().ToString();
                    dt->Rows->Add(row2);

                    DataRow^ row3 = dt->NewRow();
                    row3[L"\u041f\u0430\u0440\u0430\u043c\u0435\u0442\u0440"] = L"\u0412\u0441\u0435\u0433\u043e \u0437\u0432\u043e\u043d\u043a\u043e\u0432";
                    row3[L"\u0417\u043d\u0430\u0447\u0435\u043d\u0438\u0435"] = db->GetTotalCalls().ToString();
                    dt->Rows->Add(row3);

                    dgvDBInfo->DataSource = dt;
                }
                catch (Exception^ ex) {
                    MessageBox::Show(L"\u041e\u0448\u0438\u0431\u043a\u0430 \u0437\u0430\u0433\u0440\u0443\u0437\u043a\u0438 \u0438\u043d\u0444\u043e\u0440\u043c\u0430\u0446\u0438\u0438 \u0411\u0414: " + ex->Message);
                }
            }

            // === ??????????? ??????? ??????? ===

            void OnAddClient(Object^ sender, EventArgs^ e) {
                AddClientForm^ form = gcnew AddClientForm();
                if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    auto db = Database::DatabaseManager::GetInstance();
                    Models::Client^ client = gcnew Models::Client();
                    client->LastName = form->LastName;
                    client->FirstName = form->FirstName;
                    client->PhoneNumber = form->PhoneNumber;
                    db->AddClient(client);
                    LoadClients();
                    LoadDBInfo();
                    UpdateTimestamp();
                    MessageBox::Show(L"\u041a\u043b\u0438\u0435\u043d\u0442 \u0434\u043e\u0431\u0430\u0432\u043b\u0435\u043d!");
                }
            }

            void OnEditClient(Object^ sender, EventArgs^ e) {
                if (dgvClients->SelectedRows->Count == 0) {
                    MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u043a\u043b\u0438\u0435\u043d\u0442\u0430 \u0434\u043b\u044f \u0440\u0435\u0434\u0430\u043a\u0442\u0438\u0440\u043e\u0432\u0430\u043d\u0438\u044f!");
                    return;
                }

                int clientId = Convert::ToInt32(dgvClients->SelectedRows[0]->Cells["ID"]->Value);
                auto db = Database::DatabaseManager::GetInstance();
                auto client = db->FindClient(clientId);

                if (client != nullptr) {
                    AddClientForm^ form = gcnew AddClientForm(client);

                    if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                        client->LastName = form->LastName;
                        client->FirstName = form->FirstName;
                        client->PhoneNumber = form->PhoneNumber;
                        db->UpdateClient(client);
                        LoadClients();
                        UpdateTimestamp();
                        MessageBox::Show(L"\u041a\u043b\u0438\u0435\u043d\u0442 \u043e\u0431\u043d\u043e\u0432\u043b\u0451\u043d!");
                    }
                }
            }

            void OnDeleteClient(Object^ sender, EventArgs^ e) {
                if (dgvClients->SelectedRows->Count == 0) {
                    MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u043a\u043b\u0438\u0435\u043d\u0442\u0430 \u0434\u043b\u044f \u0443\u0434\u0430\u043b\u0435\u043d\u0438\u044f!");
                    return;
                }

                auto result = MessageBox::Show(L"\u0423\u0434\u0430\u043b\u0438\u0442\u044c \u0432\u044b\u0431\u0440\u0430\u043d\u043d\u043e\u0433\u043e \u043a\u043b\u0438\u0435\u043d\u0442\u0430?", L"\u041f\u043e\u0434\u0442\u0432\u0435\u0440\u0436\u0434\u0435\u043d\u0438\u0435",
                    MessageBoxButtons::YesNo, MessageBoxIcon::Question);

                if (result == System::Windows::Forms::DialogResult::Yes) {
                    int clientId = Convert::ToInt32(dgvClients->SelectedRows[0]->Cells["ID"]->Value);
                    auto db = Database::DatabaseManager::GetInstance();
                    db->DeleteClient(clientId);
                    LoadClients();
                    LoadCalls();
                    LoadDBInfo();
                    UpdateTimestamp();
                    MessageBox::Show(L"\u041a\u043b\u0438\u0435\u043d\u0442 \u0443\u0434\u0430\u043b\u0451\u043d!");
                }
            }

            void OnMakeCall(Object^ sender, EventArgs^ e) {
                if (dgvClients->SelectedRows->Count == 0) {
                    MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u043a\u043b\u0438\u0435\u043d\u0442\u0430!");
                    return;
                }

                auto db = Database::DatabaseManager::GetInstance();
                auto tariffs = db->GetAllTariffs();

                if (tariffs->Count == 0) {
                    MessageBox::Show(L"\u0421\u043d\u0430\u0447\u0430\u043b\u0430 \u0434\u043e\u0431\u0430\u0432\u044c\u0442\u0435 \u0442\u0430\u0440\u0438\u0444\u044b!");
                    return;
                }

                int clientId = Convert::ToInt32(dgvClients->SelectedRows[0]->Cells["ID"]->Value);

                // ??????? inline ????? ??? ?????? ?????? ? ????????????
                Form^ callForm = gcnew Form();
                callForm->Text = L"\u0421\u043e\u0432\u0435\u0440\u0448\u0438\u0442\u044c \u0437\u0432\u043e\u043d\u043e\u043a";
                callForm->Size = System::Drawing::Size(400, 250);
                callForm->StartPosition = FormStartPosition::CenterParent;
                callForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
                callForm->MaximizeBox = false;
                callForm->MinimizeBox = false;

                Label^ lblTariff = gcnew Label();
                lblTariff->Text = L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0442\u0430\u0440\u0438\u0444:";
                lblTariff->Location = Point(20, 20);
                lblTariff->Size = System::Drawing::Size(120, 20);

                ComboBox^ cmbTariff = gcnew ComboBox();
                cmbTariff->Location = Point(150, 20);
                cmbTariff->Size = System::Drawing::Size(210, 20);
                cmbTariff->DropDownStyle = ComboBoxStyle::DropDownList;

                for each (auto tariff in tariffs) {
                    String^ item = String::Format(L"{0} - {1} \u0440\u0443\u0431/\u043c\u0438\u043d ({2})",
                        tariff->City, tariff->BaseCostPerMinute, tariff->StrategyType);
                    cmbTariff->Items->Add(item);
                }
                if (cmbTariff->Items->Count > 0) {
                    cmbTariff->SelectedIndex = 0;
                }

                Label^ lblDuration = gcnew Label();
                lblDuration->Text = L"\u0414\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0441\u0442\u044c (\u043c\u0438\u043d):";
                lblDuration->Location = Point(20, 60);
                lblDuration->Size = System::Drawing::Size(120, 20);

                TextBox^ txtDuration = gcnew TextBox();
                txtDuration->Location = Point(150, 60);
                txtDuration->Size = System::Drawing::Size(210, 20);
                txtDuration->Text = L"10";

                Label^ lblCost = gcnew Label();
                lblCost->Text = L"\u0421\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c: 0.00 \u0440\u0443\u0431";
                lblCost->Location = Point(20, 100);
                lblCost->Size = System::Drawing::Size(340, 20);
                lblCost->Font = gcnew System::Drawing::Font(lblCost->Font->FontFamily, 10, FontStyle::Bold);

                // ?????????? ??? ??????? ?????????
                EventHandler^ calculateCost = gcnew EventHandler(
                    this, &MainForm::CalculateCallCost
                );

                // ????????? ?????? ? Tag ??? ??????? ?? ???????????
                array<Object^>^ controls = gcnew array<Object^>(4);
                controls[0] = cmbTariff;
                controls[1] = txtDuration;
                controls[2] = lblCost;
                controls[3] = tariffs;
                callForm->Tag = controls;

                cmbTariff->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnCallFormDataChanged);
                txtDuration->TextChanged += gcnew EventHandler(this, &MainForm::OnCallFormDataChanged);

                // ????????? ??????
                UpdateCallCost(cmbTariff, txtDuration, lblCost, tariffs);

                Button^ btnOK = gcnew Button();
                btnOK->Text = L"\u0421\u043e\u0432\u0435\u0440\u0448\u0438\u0442\u044c";
                btnOK->Location = Point(180, 140);
                btnOK->Size = System::Drawing::Size(80, 30);
                btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;

                Button^ btnCancel = gcnew Button();
                btnCancel->Text = L"\u041e\u0442\u043c\u0435\u043d\u0430";
                btnCancel->Location = Point(280, 140);
                btnCancel->Size = System::Drawing::Size(80, 30);
                btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

                callForm->Controls->Add(lblTariff);
                callForm->Controls->Add(cmbTariff);
                callForm->Controls->Add(lblDuration);
                callForm->Controls->Add(txtDuration);
                callForm->Controls->Add(lblCost);
                callForm->Controls->Add(btnOK);
                callForm->Controls->Add(btnCancel);
                callForm->AcceptButton = btnOK;
                callForm->CancelButton = btnCancel;

                if (callForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    try {
                        int duration = Int32::Parse(txtDuration->Text);
                        if (duration <= 0) {
                            MessageBox::Show(L"\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u043a\u043e\u0440\u0440\u0435\u043a\u0442\u043d\u0443\u044e \u0434\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0441\u0442\u044c!");
                            return;
                        }

                        int tariffIndex = cmbTariff->SelectedIndex;
                        if (tariffIndex < 0) {
                            MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0442\u0430\u0440\u0438\u0444!");
                            return;
                        }

                        auto tariff = tariffs[tariffIndex];
                        double cost = duration * tariff->BaseCostPerMinute;

                        if (tariff->StrategyType == L"Discounted" && tariff->DiscountRate.HasValue) {
                            double discount = cost * (tariff->DiscountRate.Value / 100.0);
                            cost -= discount;
                        }

                        Models::Call^ call = gcnew Models::Call();
                        call->ClientId = clientId;
                        call->TariffId = tariff->Id;
                        call->City = tariff->City;
                        call->Duration = duration;
                        call->Cost = cost;

                        db->AddCall(call);
                        LoadCalls();
                        LoadDBInfo();
                        UpdateTimestamp();
                        MessageBox::Show(String::Format(L"\u0417\u0432\u043e\u043d\u043e\u043a \u0434\u043e\u0431\u0430\u0432\u043b\u0435\u043d! \u0421\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c: {0:F2} \u0440\u0443\u0431", cost));
                    }
                    catch (Exception^ ex) {
                        MessageBox::Show(L"\u041e\u0448\u0438\u0431\u043a\u0430: " + ex->Message);
                    }
                }
            }

            void OnCallFormDataChanged(Object^ sender, EventArgs^ e) {
                Form^ form = safe_cast<Form^>(safe_cast<Control^>(sender)->FindForm());
                if (form != nullptr && form->Tag != nullptr) {
                    array<Object^>^ controls = safe_cast<array<Object^>^>(form->Tag);
                    ComboBox^ cmbTariff = safe_cast<ComboBox^>(controls[0]);
                    TextBox^ txtDuration = safe_cast<TextBox^>(controls[1]);
                    Label^ lblCost = safe_cast<Label^>(controls[2]);
                    List<Models::Tariff^>^ tariffs = safe_cast<List<Models::Tariff^>^>(controls[3]);

                    UpdateCallCost(cmbTariff, txtDuration, lblCost, tariffs);
                }
            }

            void UpdateCallCost(ComboBox^ cmbTariff, TextBox^ txtDuration, Label^ lblCost, List<Models::Tariff^>^ tariffs) {
                try {
                    int duration = Int32::Parse(txtDuration->Text);
                    int tariffIndex = cmbTariff->SelectedIndex;

                    if (tariffIndex >= 0 && tariffIndex < tariffs->Count) {
                        auto tariff = tariffs[tariffIndex];
                        double cost = duration * tariff->BaseCostPerMinute;

                        if (tariff->StrategyType == L"Discounted" && tariff->DiscountRate.HasValue) {
                            double discount = cost * (tariff->DiscountRate.Value / 100.0);
                            cost -= discount;
                        }

                        lblCost->Text = String::Format(L"\u0421\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c: {0:F2} \u0440\u0443\u0431", cost);
                    }
                }
                catch (...) {
                    lblCost->Text = L"\u0421\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c: 0.00 \u0440\u0443\u0431";
                }
            }

            void CalculateCallCost(Object^ sender, EventArgs^ e) {
                // ?????? ?????????? - ???????? ?????? ? OnCallFormDataChanged
            }

            // === ??????????? ??????? ?????? ===

            void OnAddTariff(Object^ sender, EventArgs^ e) {
                AddTariffForm^ form = gcnew AddTariffForm();
                if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    auto db = Database::DatabaseManager::GetInstance();
                    Models::Tariff^ tariff = gcnew Models::Tariff();
                    tariff->City = form->City;
                    tariff->BaseCostPerMinute = form->BaseCostPerMinute;
                    tariff->StrategyType = form->StrategyType;
                    tariff->DiscountRate = form->DiscountRate;
                    db->AddTariff(tariff);
                    LoadTariffs();
                    LoadDBInfo();
                    UpdateTimestamp();
                    MessageBox::Show(L"\u0422\u0430\u0440\u0438\u0444 \u0434\u043e\u0431\u0430\u0432\u043b\u0435\u043d!");
                }
            }

            void OnEditTariff(Object^ sender, EventArgs^ e) {
                if (dgvTariffs->SelectedRows->Count == 0) {
MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0442\u0430\u0440\u0438\u0444 \u0434\u043b\u044f \u0440\u0435\u0434\u0430\u043a\u0442\u0438\u0440\u043e\u0432\u0430\u043d\u0438\u044f!");
                return;
            }

            int tariffId = Convert::ToInt32(dgvTariffs->SelectedRows[0]->Cells["ID"]->Value);
                auto db = Database::DatabaseManager::GetInstance();
                auto tariff = db->FindTariff(tariffId);

                if (tariff != nullptr) {
                    AddTariffForm^ form = gcnew AddTariffForm(tariff);
                    if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                        tariff->City = form->City;
                        tariff->BaseCostPerMinute = form->BaseCostPerMinute;
                        tariff->StrategyType = form->StrategyType;
                        tariff->DiscountRate = form->DiscountRate;
                        db->UpdateTariff(tariff);
                        LoadTariffs();
                        UpdateTimestamp();
                        MessageBox::Show(L"\u0422\u0430\u0440\u0438\u0444 \u043e\u0431\u043d\u043e\u0432\u043b\u0451\u043d!");
                    }
                }
            }

            void OnDeleteTariff(Object^ sender, EventArgs^ e) {
                if (dgvTariffs->SelectedRows->Count == 0) {
                    MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0442\u0430\u0440\u0438\u0444 \u0434\u043b\u044f \u0443\u0434\u0430\u043b\u0435\u043d\u0438\u044f!");
                    return;
                }

                auto result = MessageBox::Show(L"\u0423\u0434\u0430\u043b\u0438\u0442\u044c \u0432\u044b\u0431\u0440\u0430\u043d\u043d\u044b\u0439 \u0442\u0430\u0440\u0438\u0444?", L"\u041f\u043e\u0434\u0442\u0432\u0435\u0440\u0436\u0434\u0435\u043d\u0438\u0435",
                    MessageBoxButtons::YesNo, MessageBoxIcon::Question);

                if (result == System::Windows::Forms::DialogResult::Yes) {
                    int tariffId = Convert::ToInt32(dgvTariffs->SelectedRows[0]->Cells["ID"]->Value);
                    auto db = Database::DatabaseManager::GetInstance();
                    db->DeleteTariff(tariffId);
                    LoadTariffs();
                    LoadCalls();
                    LoadDBInfo();
                    UpdateTimestamp();
                    MessageBox::Show(L"\u0422\u0430\u0440\u0438\u0444 \u0443\u0434\u0430\u043b\u0451\u043d!");
                }
            }

            void OnDeleteCall(Object^ sender, EventArgs^ e) {
                if (dgvCalls->SelectedRows->Count == 0) {
                    MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0437\u0432\u043e\u043d\u043e\u043a \u0434\u043b\u044f \u0443\u0434\u0430\u043b\u0435\u043d\u0438\u044f!");
                    return;
                }

                auto result = MessageBox::Show(L"\u0423\u0434\u0430\u043b\u0438\u0442\u044c \u0432\u044b\u0431\u0440\u0430\u043d\u043d\u044b\u0439 \u0437\u0432\u043e\u043d\u043e\u043a?", L"\u041f\u043e\u0434\u0442\u0432\u0435\u0440\u0436\u0434\u0435\u043d\u0438\u0435",
                    MessageBoxButtons::YesNo, MessageBoxIcon::Question);

                if (result == System::Windows::Forms::DialogResult::Yes) {
                    int callId = Convert::ToInt32(dgvCalls->SelectedRows[0]->Cells["ID"]->Value);
                    auto db = Database::DatabaseManager::GetInstance();
                    db->DeleteCall(callId);
                    LoadCalls();
                    LoadDBInfo();
                    UpdateTimestamp();
MessageBox::Show(L"\u0417\u0432\u043e\u043d\u043e\u043a \u0443\u0434\u0430\u043b\u0451\u043d!");
                }
            }

            void OnViewClientCalls(Object^ sender, EventArgs^ e) {
                if (dgvCalls->SelectedRows->Count == 0) {
                    MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0437\u0432\u043e\u043d\u043e\u043a!");
                return;
            }

            int clientId = Convert::ToInt32(dgvCalls->SelectedRows[0]->Cells[L"ID \u041a\u043b\u0438\u0435\u043d\u0442\u0430"]->Value);
                auto db = Database::DatabaseManager::GetInstance();
                auto client = db->FindClient(clientId);
                auto calls = db->GetCallsByClient(clientId);

                if (client != nullptr) {
                    String^ message = String::Format(L"\u041a\u043b\u0438\u0435\u043d\u0442: {0} {1}\n\n\u0412\u0441\u0435\u0433\u043e \u0437\u0432\u043e\u043d\u043a\u043e\u0432: {2}\n\n",
                        client->FirstName, client->LastName, calls->Count);

                    double totalCost = 0;
                    int totalDuration = 0;

                    for each (auto call in calls) {
                        message += String::Format(L"\u0413\u043e\u0440\u043e\u0434: {0}, \u0414\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0441\u0442\u044c: {1} \u043c\u0438\u043d, \u0421\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c: {2:F2} \u0440\u0443\u0431\n",
                            call->City, call->Duration, call->Cost);
                        totalCost += call->Cost;
                        totalDuration += call->Duration;
                    }

                    message += String::Format(L"\n\u041e\u0431\u0449\u0430\u044f \u0434\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0441\u0442\u044c: {0} \u043c\u0438\u043d\n\u041e\u0431\u0449\u0430\u044f \u0441\u0442\u043e\u0438\u043c\u043e\u0441\u0442\u044c: {1:F2} \u0440\u0443\u0431",
                        totalDuration, totalCost);

                    MessageBox::Show(message, L"\u0417\u0432\u043e\u043d\u043a\u0438 \u043a\u043b\u0438\u0435\u043d\u0442\u0430", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
            }

            // === ????? ??????????? ===

            void OnClearDB(Object^ sender, EventArgs^ e) {
                auto result = MessageBox::Show(L"\u041e\u0447\u0438\u0441\u0442\u0438\u0442\u044c \u0432\u0441\u044e \u0431\u0430\u0437\u0443 \u0434\u0430\u043d\u043d\u044b\u0445?", L"\u041f\u043e\u0434\u0442\u0432\u0435\u0440\u0436\u0434\u0435\u043d\u0438\u0435",
                    MessageBoxButtons::YesNo, MessageBoxIcon::Warning);

                if (result == System::Windows::Forms::DialogResult::Yes) {
                    auto db = Database::DatabaseManager::GetInstance();
                    db->ClearDatabase();
                    LoadData();
                    UpdateTimestamp();
                    MessageBox::Show(L"\u0411\u0430\u0437\u0430 \u0434\u0430\u043d\u043d\u044b\u0445 \u043e\u0447\u0438\u0449\u0435\u043d\u0430!");
                }
            }

            void OnRefresh(Object^ sender, EventArgs^ e) {
                LoadData();
                UpdateTimestamp();
            }

            void OnSaveToFile(Object^ sender, EventArgs^ e) {
                SaveFileDialog^ sfd = gcnew SaveFileDialog();
                sfd->Filter = L"Data files (*.dat)|*.dat|All files (*.*)|*.*";
                sfd->DefaultExt = L"dat";
                sfd->Title = L"\u0421\u043e\u0445\u0440\u0430\u043d\u0438\u0442\u044c \u0434\u0430\u043d\u043d\u044b\u0435 \u0432 \u0444\u0430\u0439\u043b";
                if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    auto db = Database::DatabaseManager::GetInstance();
                    if (db->ExportDataToFile(sfd->FileName)) {
                        LoadData();
                        UpdateTimestamp();
                        MessageBox::Show(L"\u0414\u0430\u043d\u043d\u044b\u0435 \u0441\u043e\u0445\u0440\u0430\u043d\u0435\u043d\u044b \u0432 \u0444\u0430\u0439\u043b.");
                    }
                }
            }

            void OnLoadFromFile(Object^ sender, EventArgs^ e) {
                OpenFileDialog^ ofd = gcnew OpenFileDialog();
                ofd->Filter = L"Data files (*.dat)|*.dat|All files (*.*)|*.*";
                ofd->Title = L"\u0417\u0430\u0433\u0440\u0443\u0437\u0438\u0442\u044c \u0434\u0430\u043d\u043d\u044b\u0435 \u0438\u0437 \u0444\u0430\u0439\u043b\u0430";
                if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    auto db = Database::DatabaseManager::GetInstance();
                    auto result = MessageBox::Show(L"\u0422\u0435\u043a\u0443\u0449\u0438\u0435 \u0434\u0430\u043d\u043d\u044b\u0435 \u0431\u0443\u0434\u0443\u0442 \u0437\u0430\u043c\u0435\u043d\u0435\u043d\u044b. \u041f\u0440\u043e\u0434\u043e\u043b\u0436\u0438\u0442\u044c?", L"\u041f\u043e\u0434\u0442\u0432\u0435\u0440\u0436\u0434\u0435\u043d\u0438\u0435", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);
                    if (result == System::Windows::Forms::DialogResult::Yes && db->ImportDataFromFile(ofd->FileName)) {
                        LoadData();
                        UpdateTimestamp();
                        MessageBox::Show(L"\u0414\u0430\u043d\u043d\u044b\u0435 \u0437\u0430\u0433\u0440\u0443\u0436\u0435\u043d\u044b \u0438\u0437 \u0444\u0430\u0439\u043b\u0430.");
                    }
                }
            }
        };
    }
}
