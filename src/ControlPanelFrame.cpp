#include "ControlPanelFrame.h"

ControlPanelFrame::ControlPanelFrame(const wxString &title, wxWindowID cpID)
    : wxFrame(nullptr, cpID, title, wxDefaultPosition, wxSize(300, 150)) {
    state = State::STATE_Select;

    wxWindowID bookID = wxIdManager::ReserveId(1);
    book = new wxSimplebook(this, bookID);

    // Create control panel
    wxWindowID menuControlID = wxIdManager::ReserveId(1);
    auto menuPanel = new wxPanel(book);

    // Create buttons for control panel
    wxSize buttonSize(200, 50);
    auto setAreaButton = new wxButton(menuPanel, ID_Area_Setter, "Set area",
                                      wxDefaultPosition, buttonSize, 0);
    auto latestButton = new wxButton(menuPanel, ID_Latest, "Copy latest",
                                     wxDefaultPosition, buttonSize, 0);

    setAreaButton->Bind(wxEVT_BUTTON, &ControlPanelFrame::OnArea, this);
    latestButton->Bind(wxEVT_BUTTON, &ControlPanelFrame::OnCopy, this);

    auto buttonSizer = new wxBoxSizer(wxVERTICAL);
    // add buttons that should be in the main view
    buttonSizer->AddSpacer(20);
    buttonSizer->Add(setAreaButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
    buttonSizer->Add(latestButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
    menuPanel->SetSizer(buttonSizer);

    wxWindowID screenshotControlID = wxIdManager::ReserveId(1);
    auto screenshotPanel = new wxPanel(book);

    auto clipboardButton =
        new wxButton(screenshotPanel, ID_To_Clip, "To clipboard",
                     wxDefaultPosition, buttonSize, 0);
    auto cancel = new wxButton(screenshotPanel, ID_Cancel, "Cancel",
                               wxDefaultPosition, buttonSize, 0);

    clipboardButton->Bind(wxEVT_BUTTON, &ControlPanelFrame::OnClip, this);
    cancel->Bind(wxEVT_BUTTON, &ControlPanelFrame::OnCancel, this);

    auto screenshotSizer = new wxBoxSizer(wxVERTICAL);
    screenshotSizer->AddSpacer(20);

    screenshotSizer->Add(clipboardButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL,
                         2);
    screenshotSizer->Add(cancel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

    screenshotPanel->SetSizer(screenshotSizer);
    screenshotPanel->Hide();

    book->AddPage(menuPanel, "menu", true); // Show this page by default
    book->AddPage(screenshotPanel, "screenShot");
}

ControlPanelFrame::~ControlPanelFrame() { book->Destroy(); }

void ControlPanelFrame::Set_State(State toState) { state = toState; }

State ControlPanelFrame::Get_State() { return state; }

void ControlPanelFrame::OnArea(wxCommandEvent &event) {
    state = State::STATE_AreaSelect;
    previous_selection = book->SetSelection(1);
    this->Hide();
}
void ControlPanelFrame::OnCancel(wxCommandEvent &event) {
    state = State::STATE_Cancel;
    previous_selection = book->SetSelection(previous_selection);
}
void ControlPanelFrame::OnClip(wxCommandEvent &event) {
    state = State::STATE_ToClipboard;
    previous_selection = book->SetSelection(previous_selection);
    // wxMessageBox("OnClip button clicked!", "Info", wxOK |
    // wxICON_INFORMATION);
}

void ControlPanelFrame::OnCopy(wxCommandEvent &event) {
    state = State::STATE_Latest;
    // wxMessageBox("OnCopy button clicked!", "Info", wxOK |
    // wxICON_INFORMATION);
}