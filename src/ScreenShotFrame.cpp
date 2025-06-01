#include "ScreenShotFrame.h"


ScreenShotFrame::ScreenShotFrame(std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates,wxWindowID screenshotID)
        : wxFrame(nullptr, screenshotID,  wxT("ScreenShotwxDC"))
{
    drawPane = new BasicPane(this, squareCoordinates);
    Bind(wxEVT_PAINT, &ScreenShotFrame::OnPaint, this);
}

void ScreenShotFrame::update_screen_size(wxSize size){
    drawPane->update_screen_size(size);
}

 void ScreenShotFrame::update_screenshot(wxImage img){
    m_image = img;
 }

ScreenShotFrame::~ScreenShotFrame(){
    drawPane->Destroy();
} 

bool ScreenShotFrame::get_mouse_released() {
    return drawPane->get_mouse_released();
}

void ScreenShotFrame::render(){
    wxClientDC dc(this);
    drawPane->second_render(dc, m_image);
}

void ScreenShotFrame::OnPaint(wxPaintEvent& event) {
    wxClientDC dc(this);
    wxBitmap bitmap(m_image);
    //wxBufferedDC dc_buffered(dc, bitmap);
    dc.DrawBitmap(bitmap, 0,0);
    
    //drawPane->render(dc);
}
