#pragma once

#include "Tariff.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace ATSProject {
    namespace Forms {
        public ref class AddTariffForm : public Form {
        private:
            TextBox^ txtCity;
            TextBox^ txtBaseCost;
            ComboBox^ cmbStrategyType;
            TextBox^ txtDiscountRate;
            Button^ btnOK;
            Button^ btnCancel;
            Label^ lblCity;
            Label^ lblBaseCost;
            Label^ lblStrategyType;
            Label^ lblDiscountRate;

        public:
            String^ City;
            double BaseCostPerMinute;
            String^ StrategyType;
            Nullable<double> DiscountRate;

            AddTariffForm() {
                InitializeComponent(false);
            }

// ?????????? ????????????: ????? ?????????????? ?????? ?????????? ??????????? ????? 
            AddTariffForm(bool isEditMode) {
                InitializeComponent(isEditMode);
            }

            // ?????????? ??? ??????????????: ?????????????? ??????? ??????
            AddTariffForm(Models::Tariff^ tariff) {
                InitializeComponent(true);
                if (tariff != nullptr) {
                    txtCity->Text = tariff->City != nullptr ? tariff->City : L"";
                    txtBaseCost->Text = tariff->BaseCostPerMinute.ToString();
                    if (tariff->StrategyType != nullptr) {
                        int idx = cmbStrategyType->Items->IndexOf(tariff->StrategyType);
                        if (idx >= 0) cmbStrategyType->SelectedIndex = idx;
                    }
                    txtDiscountRate->Text = tariff->DiscountRate.HasValue ? tariff->DiscountRate.Value.ToString() : L"0";
                    txtDiscountRate->Enabled = (tariff->StrategyType == L"Discounted");
                }
            }

        private:
            void InitializeComponent(bool isEditMode) {
                this->Text = isEditMode ? L"\u0420\u0435\u0434\u0430\u043a\u0442\u0438\u0440\u043e\u0432\u0430\u0442\u044c \u0442\u0430\u0440\u0438\u0444" : L"\u0414\u043e\u0431\u0430\u0432\u0438\u0442\u044c \u0442\u0430\u0440\u0438\u0444";
                this->Size = System::Drawing::Size(400, 280);
                this->StartPosition = FormStartPosition::CenterParent;
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
                this->MaximizeBox = false;
                this->MinimizeBox = false;
                this->BackColor = isEditMode ? System::Drawing::Color::FromArgb(230, 255, 230) : System::Drawing::SystemColors::Control;
                this->Font = gcnew System::Drawing::Font(this->Font->FontFamily, isEditMode ? 10.0f : 8.25f);

                // Labels
                lblCity = gcnew Label();
                lblCity->Text = L"\u0413\u043e\u0440\u043e\u0434:";
                lblCity->Location = Point(20, 20);
                lblCity->Size = System::Drawing::Size(120, 20);

                lblBaseCost = gcnew Label();
                lblBaseCost->Text = L"\u0426\u0435\u043d\u0430 \u0437\u0430 \u043c\u0438\u043d\u0443\u0442\u0443:";
                lblBaseCost->Location = Point(20, 60);
                lblBaseCost->Size = System::Drawing::Size(120, 20);

                lblStrategyType = gcnew Label();
                lblStrategyType->Text = L"\u0422\u0438\u043f \u0442\u0430\u0440\u0438\u0444\u0430:";
                lblStrategyType->Location = Point(20, 100);
                lblStrategyType->Size = System::Drawing::Size(120, 20);

                lblDiscountRate = gcnew Label();
                lblDiscountRate->Text = L"\u0421\u043a\u0438\u0434\u043a\u0430 (%):";
                lblDiscountRate->Location = Point(20, 140);
                lblDiscountRate->Size = System::Drawing::Size(120, 20);

                // TextBoxes
                txtCity = gcnew TextBox();
                txtCity->Location = Point(150, 20);
                txtCity->Size = System::Drawing::Size(210, 20);

                txtBaseCost = gcnew TextBox();
                txtBaseCost->Location = Point(150, 60);
                txtBaseCost->Size = System::Drawing::Size(210, 20);

                // ComboBox for strategy type
                cmbStrategyType = gcnew ComboBox();
                cmbStrategyType->Location = Point(150, 100);
                cmbStrategyType->Size = System::Drawing::Size(210, 20);
                cmbStrategyType->DropDownStyle = ComboBoxStyle::DropDownList;
                cmbStrategyType->Items->Add(L"Regular");
                cmbStrategyType->Items->Add(L"Discounted");
                cmbStrategyType->SelectedIndex = 0;
                cmbStrategyType->SelectedIndexChanged += gcnew EventHandler(this, &AddTariffForm::OnStrategyTypeChanged);

                txtDiscountRate = gcnew TextBox();
                txtDiscountRate->Location = Point(150, 140);
                txtDiscountRate->Size = System::Drawing::Size(210, 20);
                txtDiscountRate->Enabled = false;
                txtDiscountRate->Text = L"0";

                // Buttons
                btnOK = gcnew Button();
                btnOK->Text = L"OK";
                btnOK->Location = Point(180, 190);
                btnOK->Size = System::Drawing::Size(80, 30);
                btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
                btnOK->Click += gcnew EventHandler(this, &AddTariffForm::OnOKClick);

                btnCancel = gcnew Button();
                btnCancel->Text = L"\u041e\u0442\u043c\u0435\u043d\u0430";
                btnCancel->Location = Point(280, 190);
                btnCancel->Size = System::Drawing::Size(80, 30);
                btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

                // Add controls
                this->Controls->Add(lblCity);
                this->Controls->Add(lblBaseCost);
                this->Controls->Add(lblStrategyType);
                this->Controls->Add(lblDiscountRate);
                this->Controls->Add(txtCity);
                this->Controls->Add(txtBaseCost);
                this->Controls->Add(cmbStrategyType);
                this->Controls->Add(txtDiscountRate);
                this->Controls->Add(btnOK);
                this->Controls->Add(btnCancel);

                this->AcceptButton = btnOK;
                this->CancelButton = btnCancel;
            }

            void OnStrategyTypeChanged(Object^ sender, EventArgs^ e) {
                if (cmbStrategyType->SelectedItem->ToString() == L"Discounted") {
                    txtDiscountRate->Enabled = true;
                }
                else {
                    txtDiscountRate->Enabled = false;
                    txtDiscountRate->Text = L"0";
                }
            }

            void OnOKClick(Object^ sender, EventArgs^ e) {
                if (String::IsNullOrWhiteSpace(txtCity->Text)) {
                    MessageBox::Show(L"\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u043d\u0430\u0437\u0432\u0430\u043d\u0438\u0435 \u0433\u043e\u0440\u043e\u0434\u0430!", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    this->DialogResult = System::Windows::Forms::DialogResult::None;
                    return;
                }

                double cost;
                if (!Double::TryParse(txtBaseCost->Text, cost) || cost <= 0) {
                    MessageBox::Show(L"\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u043a\u043e\u0440\u0440\u0435\u043a\u0442\u043d\u0443\u044e \u0446\u0435\u043d\u0443 \u0437\u0430 \u043c\u0438\u043d\u0443\u0442\u0443!", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    this->DialogResult = System::Windows::Forms::DialogResult::None;
                    return;
                }

                City = txtCity->Text->Trim();
                BaseCostPerMinute = cost;
                StrategyType = cmbStrategyType->SelectedItem->ToString();

                if (StrategyType == L"Discounted") {
                    double discount;
                    if (!Double::TryParse(txtDiscountRate->Text, discount) || discount < 0 || discount > 100) {
                        MessageBox::Show(L"\u0412\u0432\u0435\u0434\u0438\u0442\u0435 \u043a\u043e\u0440\u0440\u0435\u043a\u0442\u043d\u0443\u044e \u0441\u043a\u0438\u0434\u043a\u0443 (0-100%)!", L"\u041e\u0448\u0438\u0431\u043a\u0430", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                        this->DialogResult = System::Windows::Forms::DialogResult::None;
                        return;
                    }
                    DiscountRate = discount;
                }
                else {
                    DiscountRate = Nullable<double>();
                }
            }
        };
    }
}
