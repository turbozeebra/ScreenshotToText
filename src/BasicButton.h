#ifndef BASICBUTTON_H
#define BASICBUTTON_H

#include <wx/wx.h>

class BasicButton : public wxWindow {

public:
    BasicButton(wxWindow *parent, int x, int y, int w, int h);

    void paintEvent(wxPaintEvent &evt);
    void OnPaint();
    bool render(wxDC &dc);

private:
    void OnClick(wxMouseEvent &event);
    int x, y, w, h;
    bool cont;
};

#endif // BASICBUTTON_H