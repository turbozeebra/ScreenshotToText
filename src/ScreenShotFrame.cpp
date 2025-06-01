#include "ScreenShotFrame.h"


ScreenShotFrame::ScreenShotFrame(std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates,wxWindowID screenshotID)
        : wxFrame(nullptr, screenshotID,  wxT("ScreenShotwxDC"))
{
    
    drawPane = new BasicPane(this, squareCoordinates);
    // button configurations and initialization 
    //Bind(wxEVT_ERASE_BACKGROUND, &ScreenShotFrame::OnEraseBackground, this);
    Bind(wxEVT_PAINT, &ScreenShotFrame::OnPaint, this);
    Bind(wxEVT_CLOSE_WINDOW, &ScreenShotFrame::OnExit, this, wxID_EXIT);
    //Bind(wxEVT_MENU, [this](wxCommandEvent&) { Close(true); }, wxID_EXIT);
    
}

void ScreenShotFrame::update_screen_size(wxSize size){
    drawPane->update_screen_size(size);
}

 void ScreenShotFrame::update_screenshot(wxImage img){
    m_image = img;
 }

ScreenShotFrame::~ScreenShotFrame(){
    std::cout << "screenshot dtor" << std::endl;
    drawPane->Destroy();
    std::cout << "screenshot dtor exit" << std::endl;
} 

bool ScreenShotFrame::get_mouse_released() {
    return drawPane->get_mouse_released();
}
void ScreenShotFrame::OnEraseBackground(wxEraseEvent&) {
    // Intentionally empty to prevent flicker
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

void ScreenShotFrame::OnExit(wxCloseEvent& event)
{
    /*
    delete button;
    button = nullptr;
    
    delete drawPane;
    drawPane = nullptr;
    */
   std::cout << "we trying to destroy" << std::endl;
    Destroy();
   // Close(true);
}



void ScreenShotFrame::close()
{
    wxWindow::Close();
}