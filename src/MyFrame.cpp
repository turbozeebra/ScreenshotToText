#include "MyFrame.h"


MyFrame::MyFrame(wxImage sharedBitmap, int w, int h, std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates)
        : wxFrame(nullptr, -1,  wxT("ScreenShotwxDC")), m_image(sharedBitmap)
{
    
    drawPane = new BasicPane(this, squareCoordinates);
    // button configurations and initialization 
    float xStart = (float)(w)/ 2.0;
    float yStart = (float)(h) - 300.0; 
    int w_box, h_box, scale;
    scale = 3;
    w_box = 90*scale;
    h_box = 30*scale;
    xStart -= ((float)(w_box) / 2.0);
    //button = new BasicButton(drawPane, xStart, yStart, w_box, h_box);
    Bind(wxEVT_ERASE_BACKGROUND, &MyFrame::OnEraseBackground, this);
}


MyFrame::~MyFrame(){
    delete button;
    button = nullptr;
    
    delete drawPane;
    drawPane = nullptr;
}

void MyFrame::OnEraseBackground(wxEraseEvent&) {
    // Intentionally empty to prevent flicker
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    delete button;
    button = nullptr;
    
    delete drawPane;
    drawPane = nullptr;
    
    Close(true);
}

bool MyFrame::render()
{
    wxClientDC dc(this);
    wxBitmap bitmap(m_image);
    //wxBufferedDC dc_buffered(dc, bitmap);
    dc.DrawBitmap(bitmap, 5, 5, false);
    drawPane->render(dc);
    return true;
}

void MyFrame::close()
{
    wxWindow::Close();
}