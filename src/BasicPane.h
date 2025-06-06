#ifndef BASICPANE_H
#define BASICPANE_H

#include <memory>
#include <tuple>
#include <wx/wx.h>

class BasicPane : public wxPanel {

public:
    BasicPane(
        wxFrame *parent,
        std::shared_ptr<std::tuple<int, int, int, int>> squareCoordinates);

    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);
    void second_render(wxDC &dc, wxImage img);
    bool get_mouse_released();

    void update_screen_size(wxSize size);
    // void render(wxDC& dc);

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
    wxRect normal_rect(int x, int y, int w, int h) const;

    // void OnExit(wxCommandEvent& event);
    void MouseReleased(wxMouseEvent &event);
    void MousePressed(wxMouseEvent &event);
    void OnMotion(wxMouseEvent &event);
    void updateSquareCoordinates();

    wxBitmap bitmap_old;
    int x_old, y_old;

    wxSize screen_size;

    bool bitmap_old_initialized = false;

    wxImage m_subImage;
    std::shared_ptr<std::tuple<int, int, int, int>> squareCoordinates;
    wxPoint startPos;
    int w, h;
    bool rec_to_draw;
    bool mouse_released;
    void setStartPos(const wxPoint sp) noexcept { startPos = sp; }
};

#endif // BASICPANE_H