#include "ControlPanelFrame.h"

ControlPanelFrame::ControlPanelFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    state = State::STATE_Select;
    // Create control panel
    controlPanel = new wxPanel(this, wxID_ANY);

    // Create buttons for control panel
    wxSize buttonSize(200,50); 
    setAreaButton = new wxButton(controlPanel, ID_Area_Setter, "Set area", wxDefaultPosition, buttonSize, 0);
    clipboardButton = new wxButton(controlPanel, ID_To_Clip, "To clipboard", wxDefaultPosition, buttonSize, 0);
    latestButton = new wxButton(controlPanel, ID_To_Clip, "Copy latest", wxDefaultPosition, buttonSize, 0);

    setAreaButton->Bind(wxEVT_BUTTON, &ControlPanelFrame::OnArea, this);
    clipboardButton->Bind(wxEVT_BUTTON, &ControlPanelFrame::OnClip, this);
    latestButton->Bind(wxEVT_BUTTON, &ControlPanelFrame::OnCopy, this);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->AddSpacer(20);
    buttonSizer->Add(setAreaButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);
    buttonSizer->Add(clipboardButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);
    buttonSizer->Add(latestButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

    controlPanel->SetSizer(buttonSizer);
}

void ControlPanelFrame::Set_State(State toState){
    state=toState;
}

State ControlPanelFrame::Get_State(){
    return state;
}
ControlPanelFrame::~ControlPanelFrame(){

   
    delete clipboardButton;
    clipboardButton = nullptr;
   
    delete setAreaButton;
    setAreaButton = nullptr;
    
    delete latestButton;
    latestButton = nullptr;
    
    delete controlPanel;
    controlPanel = nullptr;

}
int ControlPanelFrame::OnExit()
{
    std::cout << "exiting" << std::endl;
    delete controlPanel;
    controlPanel = nullptr;

    delete setAreaButton;
    setAreaButton = nullptr;
    
    delete clipboardButton;
    clipboardButton = nullptr;
    
    delete latestButton;
    latestButton = nullptr;
    std::cout << "exited" << std::endl;
    return 0;
    
}

void ControlPanelFrame::OnArea(wxCommandEvent& event)
{
    state = State::STATE_AreaSelect;
    //wxMessageBox("OnArea button clicked!", "Info", wxOK | wxICON_INFORMATION);
}

void ControlPanelFrame::OnClip(wxCommandEvent& event)
{
    state = State::STATE_ToClipboard;
    //wxMessageBox("OnClip button clicked!", "Info", wxOK | wxICON_INFORMATION);
}


void ControlPanelFrame::OnCopy(wxCommandEvent& event)
{
    state = State::STATE_Latest;
    //wxMessageBox("OnCopy button clicked!", "Info", wxOK | wxICON_INFORMATION);
}