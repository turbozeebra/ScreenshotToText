#ifndef CONTROLPANELFRAME_H
#define CONTROLPANELFRAME_H

#include <wx/wx.h>
#include <wx/overlay.h>
#include <wx/sizer.h>
#include <wx/dcgraph.h>
#include <wx/panel.h>
#include <wx/button.h>


#include <memory>

// IDs for buttons
enum 
{
    ID_Area_Setter = 1,
    ID_To_Clip = 2,
    ID_Latest = 3
};

enum class State : int {
    STATE_Select = 0,
    STATE_AreaSelect = 1,
    STATE_ToClipboard = 2,
    STATE_Latest = 3,
};


class ControlPanelFrame : public wxFrame
{
public:
    ControlPanelFrame(const wxString& title);
    State Get_State();
    void Set_State(State toState);
    ~ControlPanelFrame();
 
private:

    void OnArea(wxCommandEvent& event);
    void OnClip(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    int OnExit();
    State state;
    // controll panel logic
    wxPanel* controlPanel;
    wxButton* setAreaButton;
    wxButton* clipboardButton;
    wxButton* latestButton;
};


#endif // CONTROLPANELFRAME_H 