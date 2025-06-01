#ifndef SCREENSHOTFRAME_H
#define SCREENSHOTFRAME_H

#include <wx/wx.h>
#include <wx/overlay.h>
#include <wx/sizer.h>
#include <wx/dcgraph.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dcbuffer.h>

#include "BasicPane.h"
#include "BasicButton.h"
#include <memory>
class ScreenShotFrame : public wxFrame
{
public:
    ScreenShotFrame(std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates,wxWindowID screenshotID);
    ~ScreenShotFrame();
    void close();
    void update_screenshot(wxImage img);
    bool get_mouse_released();
    void render();
    void update_screen_size(wxSize size);
    
private:
    void OnDraw(wxPaintEvent &event);
    void OnPaint(wxPaintEvent& event);
    void OnExit(wxCloseEvent& event);
    void OnEraseBackground(wxEraseEvent&);


    
    //wxButton* m_ButtonHello;
    
    BasicPane* drawPane; 
    //BasicPane* drawPane;
    BasicButton* button;
    wxImage m_image;
    wxPoint startPos;
    int w, h;
    bool rec_to_draw;
    std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates;
    void setStartPos(const wxPoint sp) noexcept { startPos = sp; }
};

#endif // SCREENSHOTFRAME_H 