#ifndef SCREENSHOTFRAME_H
#define SCREENSHOTFRAME_H

#include <wx/button.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/overlay.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/wx.h>

#include "BasicButton.h"
#include "BasicPane.h"
#include <memory>
class ScreenshotFrame : public wxFrame {
public:
    ScreenshotFrame(
        std::shared_ptr<std::tuple<int, int, int, int>> squareCoordinates,
        wxWindowID screenshotID);
    ~ScreenshotFrame();
    void close();
    void update_screenshot(wxImage img);
    bool get_mouse_released();
    void render();
    void update_screen_size(wxSize size);

private:
    void OnPaint(wxPaintEvent &event);

    BasicPane *drawPane;

    wxImage m_image;
    wxPoint startPos;
    int w, h;
    bool rec_to_draw;
    std::shared_ptr<std::tuple<int, int, int, int>> squareCoordinates;
    void setStartPos(const wxPoint sp) noexcept { startPos = sp; }
};

#endif // SCREENSHOTFRAME_H