#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>
#include <wx/overlay.h>
#include <wx/sizer.h>
#include <wx/dcgraph.h>
#include <wx/panel.h>
#include <wx/button.h>
#include "BasicPane.h"
#include "BasicButton.h"

#include <memory>
class MyFrame : public wxFrame
{
public:
    MyFrame(wxImage img,int w, int h,std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates);
    void OnDraw(wxPaintEvent &event);
    bool render();
    void close();
 
private:
    void OnExit(wxCommandEvent& event);
    
    //wxButton* m_ButtonHello;
    BasicPane* drawPane;
    BasicButton* button;
    wxImage m_image;
    wxPoint startPos;
    int w, h;
    bool rec_to_draw;
    std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates;
    void setStartPos(const wxPoint sp) noexcept { startPos = sp; }
};

#endif // GREETER_H 