#ifndef BASICPANE_H
#define BASICPANE_H

#include <wx/wx.h>
#include <memory>
#include <tuple>

class BasicPane : public wxPanel
{
    
public:
    BasicPane(wxFrame* parent, std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void render(wxDC& dc);
    
    //void render(wxDC& dc);
    
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
private:
    void OnHello(wxCommandEvent& event);
    //void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void MouseReleased(wxMouseEvent& event);
    void MousePressed(wxMouseEvent& event);
    void OnMotion(wxMouseEvent& event);
    void updateSquareCoordinates();


    std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates;
    wxPoint startPos;
    int w, h;
    bool rec_to_draw;
    void setStartPos(const wxPoint sp) noexcept { startPos = sp; }

};

#endif // BASICPANE_H