#include "Timer.h"
//#include <windows.h>
//#pragma comment(lib, "Winmm.lib")

#include <iostream>
#include <string>
#include <fstream>

void MyFrame1::startTimer() {
	auto previous_time = std::chrono::steady_clock::now();
	while (timerToggle) {
		auto current_time = std::chrono::steady_clock::now();
		wxYield();
		if (std::chrono::duration_cast<std::chrono::seconds>(current_time - previous_time).count() >= 1) {
			if (seconds != 0) --seconds;
			if (seconds == 0) {
				if (minutes != 0) {
					--minutes;
					seconds = 59;
				}
				else {
					timerToggle = false;
					//PlaySound(TEXT("Sound/alarm.wav"), NULL, SND_ASYNC);
				}
			}
			timer->SetLabel(std::to_string(minutes) + ":" + std::string(2 - std::min(2, int(std::to_string(seconds).length())), '0') + std::to_string(seconds));
			previous_time = current_time;
		}
		
	}
}

void MyFrame1::writeSettings() {
	std::ofstream file("cfg.ini");
	if (file.is_open())
	{
		file << "Minutes=" + timerMinutes->GetTextValue() + "\n";
		file << "Seconds=" + timerSeconds->GetTextValue();
		file.close();
	}
}

MyFrame1::MyFrame1(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	std::ifstream file("cfg.ini");
	int defaultSeconds = 0, defaultMinutes = 10;
	std::string line, varName, defaultValue;
	std::string delimiter = "=";

	while (std::getline(file, line)) {
		varName = line.substr(0, line.find(delimiter));
		defaultValue = line.substr(line.find(delimiter) + 1);
		if (varName == "Minutes") {
			defaultMinutes = std::stoi(defaultValue);
			continue;
		}
		else if (varName == "Seconds") {
			defaultSeconds = std::stoi(defaultValue);
			continue;
		}
	}
	file.close();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetBackgroundColour(wxColour(0, 0, 0));

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	topToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
	topToolBar->SetToolPacking(15);
	topToolBar->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));

	saveButton = new wxButton(topToolBar, wxID_ANY, wxT("Save Settings"), wxDefaultPosition, wxDefaultSize, 0);
	topToolBar->AddControl(saveButton);
	topToolBar->Realize();

	bSizer2->Add(topToolBar, 0, wxEXPAND, 5);

	timerPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	timerPanel->SetBackgroundColour(wxColour(0, 0, 0));

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);


	bSizer3->Add(0, 0, 1, wxEXPAND, 5);

	timer = new wxStaticText(timerPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	timer->Wrap(-1);
	timer->SetFont(wxFont(48, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas")));
	timer->SetForegroundColour(wxColour(0, 255, 0));
	timer->SetBackgroundColour(wxColour(0, 0, 0));

	bSizer3->Add(timer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	bSizer3->Add(0, 0, 1, wxEXPAND, 5);


	timerPanel->SetSizer(bSizer3);
	timerPanel->Layout();
	bSizer3->Fit(timerPanel);
	bSizer2->Add(timerPanel, 1, wxEXPAND | wxALL, 5);

	bottomToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
	bottomToolBar->SetToolPacking(15);
	bottomToolBar->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));

	minutesText = new wxStaticText(bottomToolBar, wxID_ANY, wxT("Minutes"), wxDefaultPosition, wxDefaultSize, 0);
	minutesText->Wrap(-1);
	minutesText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

	bottomToolBar->AddControl(minutesText);
	timerMinutes = new wxSpinCtrl(bottomToolBar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 60, defaultMinutes);
	bottomToolBar->AddControl(timerMinutes);
	secondsText = new wxStaticText(bottomToolBar, wxID_ANY, wxT("Seconds"), wxDefaultPosition, wxDefaultSize, 0);
	secondsText->Wrap(-1);
	secondsText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

	bottomToolBar->AddControl(secondsText);
	timerSeconds = new wxSpinCtrl(bottomToolBar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 60, defaultSeconds);
	bottomToolBar->AddControl(timerSeconds);
	startButton = new wxButton(bottomToolBar, wxID_ANY, wxT("Start/Stop"), wxDefaultPosition, wxDefaultSize, 0);
	bottomToolBar->AddControl(startButton);
	resetButton = new wxButton(bottomToolBar, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	bottomToolBar->AddControl(resetButton);
	bottomToolBar->Realize();
	updateTimer();
	bSizer2->Add(bottomToolBar, 0, wxEXPAND, 5);


	this->SetSizer(bSizer2);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	saveButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::saveSettings), NULL, this);
	timerMinutes->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MyFrame1::minuteChange), NULL, this);
	timerSeconds->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MyFrame1::secondChange), NULL, this);
	startButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::toggleTimer), NULL, this);
	resetButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::resetTimer), NULL, this);
}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	saveButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::saveSettings), NULL, this);
	timerMinutes->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MyFrame1::minuteChange), NULL, this);
	timerSeconds->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MyFrame1::secondChange), NULL, this);
	startButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::toggleTimer), NULL, this);
	resetButton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::resetTimer), NULL, this);

}

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame1* frame = new MyFrame1(NULL);
	frame->Show(true);
	return true;
}
