#pragma once

#include <wx/wx.h>
#include <wx/msw/spinctrl.h>
#include <chrono>
#include <thread>

class MyFrame1 : public wxFrame
{
private:
	int seconds, minutes;
	bool timerToggle = false;
	void updateTimer() {
		if (timerSeconds->GetValue() == 60) {
			if (timerMinutes->GetValue() != timerMinutes->GetMax()) {
				timerMinutes->SetValue(timerMinutes->GetValue() + 1);
				timerSeconds->SetValue(0);
			}
			else timerSeconds->SetValue(59);
		}
		seconds = timerSeconds->GetValue();
		minutes = timerMinutes->GetValue();
		timer->SetLabel(std::to_string(minutes) + ":" + std::string(2 - std::min(2, int(std::to_string(seconds).length())), '0') + std::to_string(seconds));
	}
	void startTimer();

	void writeSettings();

protected:
	wxToolBar* topToolBar;
	wxButton* saveButton;
	wxPanel* timerPanel;
	wxStaticText* timer;
	wxToolBar* bottomToolBar;
	wxStaticText* minutesText;
	wxSpinCtrl* timerMinutes;
	wxStaticText* secondsText;
	wxSpinCtrl* timerSeconds;
	wxButton* startButton;
	wxButton* resetButton;

	// Virtual event handlers, override them in your derived class
	virtual void saveSettings(wxCommandEvent& event) { writeSettings(); }
	virtual void minuteChange(wxCommandEvent& event) { updateTimer(); }
	virtual void secondChange(wxCommandEvent& event) { updateTimer(); }
	virtual void toggleTimer(wxCommandEvent& event) { if (timerToggle) timerToggle = false; else { timerToggle = true; startTimer(); } }
	virtual void resetTimer(wxCommandEvent& event) { updateTimer(); }


public:

	MyFrame1(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Timer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(374, 199), long style = wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxTAB_TRAVERSAL);

	~MyFrame1();

};


class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};