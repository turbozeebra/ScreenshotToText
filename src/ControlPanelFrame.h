#ifndef CONTROLPANELFRAME_H
#define CONTROLPANELFRAME_H

#include <wx/button.h>
#include <wx/dcgraph.h>
#include <wx/overlay.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/wx.h>

#include <memory>

#include <wx/simplebook.h>

// IDs for buttons
enum {
  ID_Area_Setter = 1,
  ID_Latest = 2,
  ID_To_Clip = 3,
  ID_Cancel = 4,

};

enum class State : int {
  STATE_Select = 0,
  STATE_AreaSelect = 1,
  STATE_ToClipboard = 2,
  STATE_Latest = 3,
  STATE_Cancel = 4,
};

enum class PanelID : int {
  ID_MenuPanel = 0,
  ID_ScreenshotPanel = 1

};

class ControlPanelFrame : public wxFrame {
public:
  ControlPanelFrame(const wxString &title, wxWindowID cpID);
  ~ControlPanelFrame();
  State Get_State();
  void Set_State(State toState);

private:
  void OnCancel(wxCommandEvent &event);
  void OnArea(wxCommandEvent &event);
  void OnClip(wxCommandEvent &event);
  void OnCopy(wxCommandEvent &event);

  int previous_selection;
  State state;
  wxSimplebook *book;
};

#endif // CONTROLPANELFRAME_H