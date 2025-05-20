#include "MyFrame.h"
#include "BasicPane.h"
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/imagpng.h>
#include <wx/clipbrd.h>

#include <memory>
#include <tuple>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

class MyApp : public wxApp
{
public:
    bool render_loop_on;
    virtual bool OnInit();
    void activateRenderLoop(bool on);
    void onIdle(wxIdleEvent& evt);
    MyFrame* frame;
    wxBitmap m_bmpBackground;
    wxImage screenShot;
private: 
    std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates;

    wxRect* normal_rect(const int x, const int y, const int w, const int h) const;
    void readText(const wxImage subImg) const ;
};

bool MyApp::OnInit()
{
    this->render_loop_on = false;
    
    
    // screenshot things 
    wxScreenDC dc;
	wxSize size = dc.GetSize();
	wxBitmap bmp(size.x, size.y);
	wxMemoryDC memDC(bmp);
	memDC.Blit(0, 0, size.x, size.y, &dc, 0, 0);
	memDC.SelectObject(wxNullBitmap);
    screenShot = bmp.ConvertToImage();
    //wxInitAllImageHandlers();
    wxImage::AddHandler(new wxPNGHandler());
	//img.SaveFile("./test.png", wxBITMAP_TYPE_PNG); this is how you save an image
    wxCoord w, h;
    dc.GetSize(&w, &h);
    // these are the coordinates that are going to be used to determine wanted area of the screen
    auto ptr = std::make_shared<std::tuple<int, int, int, int>>(-1, 0, 0, 0);
    squareCoordinates = ptr;
    frame = new MyFrame(screenShot, w ,h, squareCoordinates);   
    
    // Create the button widget
    frame->ShowFullScreen(true);
    frame->Show(true);

    activateRenderLoop(true);
    return true;
}

void MyApp::onIdle(wxIdleEvent& evt)
{
    
    if(this->render_loop_on)
    {
        activateRenderLoop(frame->render());        
        evt.RequestMore(); // render continuously, not only once on idle
    }
}

void MyApp::activateRenderLoop(bool on)
{
    if(on && !render_loop_on)
    {
        Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
        render_loop_on = true;
    }
    else if(!on && render_loop_on)
    {
        Disconnect( wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
        render_loop_on = false; 
        wxRect *rect = normal_rect(std::get<0>(*squareCoordinates),std::get<1>(*squareCoordinates),std::get<2>(*squareCoordinates),std::get<3>(*squareCoordinates));
        if(rect->GetX() > -1)
        {
            wxImage subImg = screenShot.GetSubImage(*rect); 
            readText(subImg);
        }

        frame->close();
    }
}

wxRect* MyApp::normal_rect(const int x, const int y, const int w, const int h) const
{
    int x1,y1,w1,h1;
    x1 = x;
    y1 = y;
    w1 = w;
    h1 = h;

    if(w < 0 )
    {
        x1 += w;
        w1 = std::abs(w);
    }

    if(h < 0)
    {
        y1 += h;
        h1 = std::abs(h);
    }

    return new wxRect(x1,y1,w1,h1);
}

void MyApp::readText(const wxImage subImg) const 
{
    char *outText;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) 
    {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    int width = subImg.GetWidth();
    int height = subImg.GetHeight();
    const unsigned char* rgb_buffer = subImg.GetData(); 
    api->SetImage(rgb_buffer, width, height, 3, width * 3);
    
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("%s", outText);

    // Destroy used object and release memory
    
    api->End();
    delete api;
    delete [] outText;
    //pixDestroy(&image);

}


IMPLEMENT_APP(MyApp)

