#include "pch.h"
#include "MainFrame.h"
#include "ids.h"
#include "LoadingPanel.h"
#include "MainPanel.h"


/**
 * Initialize the MainFrame window.
 */
void MainFrame::Initialize()
{
	wxSize screenSize = wxGetDisplaySize();
	Create(nullptr, wxID_ANY, L"FaceSpyder", wxDefaultPosition, screenSize);

	// Initialize and show the loading panel
	InitializeLoadingPanel();
}

void MainFrame::InitializeLoadingPanel() {

	// Create panels for different screens
	mLoadingPanel = new LoadingPanel(this);

	// Configure a timer to close the loading screen after a certain time
	wxTimer* loadingTimer = new wxTimer(this, 1);
	loadingTimer->StartOnce(5000);  // Show loading screen for 3 seconds
	Bind(wxEVT_TIMER, &MainFrame::OnLoadingTimer, this);
	
}

void MainFrame::OnLoadingTimer(wxTimerEvent& event) {
	// Hide loading panel and show the main application panel
	mLoadingPanel->Hide();
	mMainPanel = new MainPanel(this);
	mMainPanel->Show();

	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);

	auto menuBar = new wxMenuBar();

	auto fileMenu = new wxMenu();
	auto helpMenu = new wxMenu();

	fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");
	fileMenu->Append(wxID_SAVEAS, "Save &As...\tCtrl-S", L"Save as...");
	fileMenu->Append(wxID_OPEN, "Open &File...\tCtrl-F", L"Open file...");

	helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	SetMenuBar(menuBar);
	CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);
}


/**
 * Exit menu option handlers
 * @param event
 */
void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

/**
 * Exit menu option handlers
 * @param event
 */
void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(L"\tWelcome to the FaceSpyder!\n\n \
		\t\tCreated by Sriram Seelamneni \n\n \
		The Face Recognition and Privacy Analysis Tool is a software application developed with the primary goal of providing advanced face and object recognition capabilities.While its current focus is on recognizing faces and objects in various media elements, including images, videos, and documents, its long - term vision includes becoming a powerful privacy analysis tool.The inspiration for this tool is the growing concern regarding data privacy and the potential misuse of facial recognition technology.",
		L"About Face Recognition",
		wxOK,
		this);
}
