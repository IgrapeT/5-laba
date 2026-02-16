#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    ATSProject::Forms::MainForm^ form = gcnew ATSProject::Forms::MainForm();
    Application::Run(form);
    return 0;
}