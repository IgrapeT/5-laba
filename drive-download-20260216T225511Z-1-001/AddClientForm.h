#pragma once

#include "Client.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Text::RegularExpressions;

namespace ATSProject {
    namespace Forms {
        public ref class AddClientForm : public Form {
        private:
            TextBox^ txtLastName;
            TextBox^ txtFirstName;
            TextBox^ txtPhone;
            Button^ btnOK;
            Button^ btnCancel;
            Label^ lblLastName;
            Label^ lblFirstName;
            Label^ lblPhone;

        public:
            String^ LastName;
            String^ FirstName;
            String^ PhoneNumber;

            AddClientForm() {
                InitializeComponent(false);
            }

            // ?????????? ????????????: ????? ?????????????? ?????? ?????????? ??????????? ?????
            AddClientForm(bool isEditMode) {
                InitializeComponent(isEditMode);
            }

            // ?????????? ??? ??????????????: ?????????????? ??????? ???????
            AddClientForm(Models::Client^ client) {
                InitializeComponent(true);
                if (client != nullptr) {
                    txtLastName->Text = client->LastName != nullptr ? client->LastName : L"";
                    txtFirstName->Text = client->FirstName != nullptr ? client->FirstName : L"";
                    txtPhone->Text = client->PhoneNumber != nullptr ? client->PhoneNumber : L"";
                }
            }

        private:
            void InitializeComponent(bool isEditMode) {
                this->Text = isEditMode ? L"\u0420\u0435\u0434\u0430\u043a\u0442\u0438\u0440\u043e\u0432\u0430\u0442\u044c \u043a\u043b\u0438\u0435\u043d\u0442\u0430" : L"\u0414\u043e\u0431\u0430\u0432\u0438\u0442\u044c \u043a\u043b\u0438\u0435\u043d\u0442\u0430";
                this->Size = System::Drawing::Size(400, 250);
                this->StartPosition = FormStartPosition::CenterParent;
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
                this->MaximizeBox = false;
                this->MinimizeBox = false;
                this->BackColor = isEditMode ? System::Drawing::Color::FromArgb(255, 255, 230) : System::Drawing::SystemColors::Control;
                this->Font = gcnew System::Drawing::Font(this->Font->FontFamily, isEditMode ? 10.0f : 8.25f);

                // Labels
                lblLastName = gcnew Label();
                lblLastName->Text = L"\u0424\u0430\u043c\u0438\u043b\u0438\u044f:";
                lblLastName->Location = Point(20, 20);
                lblLastName->Size = System::Drawing::Size(100, 20);

                lblFirstName = gcnew Label();
                lblFirstName->Text = L"\u0418\u043c\u044f:";
                lblFirstName->Location = Point(20, 60);
                lblFirstName->Size = System::Drawing::Size(100, 20);

                lblPhone = gcnew Label();
                lblPhone->Text = L"\u0422\u0435\u043b\u0435\u0444\u043e\u043d:";
                lblPhone->Location = Point(20, 100);
                lblPhone->Size = System::Drawing::Size(100, 20);

                // TextBoxes
                txtLastName = gcnew TextBox();
                txtLastName->Location = Point(130, 20);
                txtLastName->Size = System::Drawing::Size(230, 20);

                txtFirstName = gcnew TextBox();
                txtFirstName->Location = Point(130, 60);
                txtFirstName->Size = System::Drawing::Size(230, 20);

                txtPhone = gcnew TextBox();
                txtPhone->Location = Point(130, 100);
                txtPhone->Size = System::Drawing::Size(230, 20);

                // Buttons
                btnOK = gcnew Button();
                btnOK->Text = L"OK";
                btnOK->Location = Point(180, 150);
                btnOK->Size = System::Drawing::Size(80, 30);
                btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
                btnOK->Click += gcnew EventHandler(this, &AddClientForm::OnOKClick);

                btnCancel = gcnew Button();
                btnCancel->Text = L"\u041e\u0442\u043c\u0435\u043d\u0430";
                btnCancel->Location = Point(280, 150);
                btnCancel->Size = System::Drawing::Size(80, 30);
                btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

                // Add controls
                this->Controls->Add(lblLastName);
                this->Controls->Add(lblFirstName);
                this->Controls->Add(lblPhone);
                this->Controls->Add(txtLastName);
                this->Controls->Add(txtFirstName);
                this->Controls->Add(txtPhone);
                this->Controls->Add(btnOK);
                this->Controls->Add(btnCancel);

                this->AcceptButton = btnOK;
                this->CancelButton = btnCancel;
            }

            void OnOKClick(Object^ sender, EventArgs^ e) {
                if (String::IsNullOrWhiteSpace(txtLastName->Text)) {
                    MessageBox::Show(L"\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u0444\u0430\u043c\u0438\u043b\u0438\u044e!", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    this->DialogResult = System::Windows::Forms::DialogResult::None;
                    return;
                }
                if (String::IsNullOrWhiteSpace(txtFirstName->Text)) {
                    MessageBox::Show(L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0438\u043c\u044f!", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    this->DialogResult = System::Windows::Forms::DialogResult::None;
                    return;
                }
                if (String::IsNullOrWhiteSpace(txtPhone->Text)) {
                    MessageBox::Show(L"\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u0444\u0430\u043c\u0438\u043b\u0438\u044e!", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    this->DialogResult = System::Windows::Forms::DialogResult::None;
                    return;
                }
                String^ phone = txtPhone->Text->Trim();
                String^ digitsOnly = Regex::Replace(phone, L"[^0-9]", L"");
                if (digitsOnly->Length < 10 || digitsOnly->Length > 15) {
                    MessageBox::Show(L"\u0422\u0435\u043b\u0435\u0444\u043e\u043d \u0434\u043e\u043b\u0436\u0435\u043d \u0441\u043e\u0434\u0435\u0440\u0436\u0430\u0442\u044c \u043e\u0442 10 \u0434\u043e 15 \u0446\u0438\u0444\u0440.", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    this->DialogResult = System::Windows::Forms::DialogResult::None;
                    return;
                }
                if (!Regex::IsMatch(phone, L"^[0-9+\\s\\-\\(\\)]+$")) {
                    MessageBox::Show(L"\u0422\u0435\u043b\u0435\u0444\u043e\u043d \u043c\u043e\u0436\u0435\u0442 \u0441\u043e\u0434\u0435\u0440\u0436\u0430\u0442\u044c \u0442\u043e\u043b\u044c\u043a\u043e \u0446\u0438\u0444\u0440\u044b, +, \u043f\u0440\u043e\u0431\u0435\u043b\u044b \u0438 \u0434\u0435\u0444\u0438\u0441\u044b.", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    this->DialogResult = System::Windows::Forms::DialogResult::None;
                    return;
                }

                LastName = txtLastName->Text->Trim();
                FirstName = txtFirstName->Text->Trim();
                PhoneNumber = txtPhone->Text->Trim();
            }
        };
    }
}
