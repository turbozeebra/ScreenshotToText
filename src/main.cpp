#include "MyFrame.h"
#include "BasicPane.h"
#include "ControlPanelFrame.h"

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
    bool render_loop_screenshot_on;

    virtual bool OnInit();
    void onIdle(wxIdleEvent& evt);
    MyFrame* frame_screenshot;
    ControlPanelFrame* cpFrame;
    wxBitmap m_bmpBackground;
    wxImage screenShot;
    private: 
    std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates;
    wxRect* normal_rect(const int x, const int y, const int w, const int h) const;
    void readText(const wxImage subImg) const ;
    
    void initStateHandlers();
    void handleSelect();
    void handleAreaSelect();
    void handleToClipBoard();
    void handleLatest();
    
    void activateRenderLoop(bool on);
    void activateRenderLoopScreenshot(bool on);
    
    std::unordered_map<State, std::function<void()>> stateHandlers;

}; 

bool MyApp::OnInit()
{
    this->render_loop_on = false;
    
    cpFrame = new ControlPanelFrame("Control Panel");
    cpFrame->Show(true);
    activateRenderLoop(true);
    initStateHandlers();
    std::cout << "we ready on init" << std::endl;
    return true;
}

void MyApp::handleSelect(){
    //std::cout << "handling select" << std::endl;
    cpFrame->Set_State(State::STATE_Select);
}
void MyApp::handleAreaSelect(){
   //std::cout << "handling area select" << std::endl;
   activateRenderLoopScreenshot(true);

}
void MyApp::handleToClipBoard(){
    std::cout << "handling ToClipBoard" << std::endl;
    activateRenderLoopScreenshot(false);
    cpFrame->Set_State(State::STATE_Select);
}
void MyApp::handleLatest(){
    std::cout << "handling latest" << std::endl;
    cpFrame->Set_State(State::STATE_Select);
}

void MyApp::initStateHandlers() {
    std::cout << "handling init" << std::endl;
    stateHandlers = {
        { State::STATE_Select,    [this]() { handleSelect(); } },
        { State::STATE_AreaSelect, [this]() { handleAreaSelect(); } },
        { State::STATE_ToClipboard, [this]() { handleToClipBoard(); } },
        { State::STATE_Latest, [this]() { handleLatest(); } }
    };
    std::cout << "done" << std::endl;
}

void MyApp::onIdle(wxIdleEvent& evt)
{
    
    State current = cpFrame->Get_State();
    auto it = stateHandlers.find(cpFrame->Get_State());
    
    if (it != stateHandlers.end()) {
        it->second();
    } else {
        std::cout << "unknown state" << std::endl;
    }

    if(this->render_loop_on)
    {
        //activateRenderLoop(frame->render());        
        evt.RequestMore(); // render continuously, not only once on idle
    }
}


void MyApp::activateRenderLoopScreenshot(bool on)
{
    if (on && !render_loop_screenshot_on)
    {
        cpFrame->Show(false);
        //cpFrame->Show(false);
        // initialize screenshot things 
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
        frame_screenshot = new MyFrame(screenShot, w ,h, squareCoordinates);   

        // Create the button widget
        frame_screenshot->ShowFullScreen(true);
        frame_screenshot->Show(true);

        //cpFrame->Raise();  // bring window to front
        //cpFrame->Show(true); // show the window


        render_loop_screenshot_on = true;
        cpFrame->Raise();
    }

    if(on && render_loop_screenshot_on)
    {
        frame_screenshot->render();
        cpFrame->Raise();
        cpFrame->Show();
          // bring window to front
        //cpFrame->Show(true); // show the window

    }

    if(!on){
        render_loop_screenshot_on = false;
        wxRect *rect = normal_rect(std::get<0>(*squareCoordinates),std::get<1>(*squareCoordinates),std::get<2>(*squareCoordinates),std::get<3>(*squareCoordinates));
        if(rect->GetX() > -1)
        {
            wxImage subImg = screenShot.GetSubImage(*rect); 
            readText(subImg);
        }

        frame_screenshot->close();
        delete frame_screenshot;
        //frame_screenshot = nullptr;
    }
}


void MyApp::activateRenderLoop(bool on)
{
    std::cout<<"we renderloop" << std::endl;
    if(on && !render_loop_on)
    {
        Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
        //render_loop_on = true;
    }
    else if(!on && render_loop_on)
    {
        Disconnect( wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
        render_loop_on = false; 
     
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

