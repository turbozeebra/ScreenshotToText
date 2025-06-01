// wxWidgets "Hello World" Program
 
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wx.h>

#include "ControlPanelFrame.h"
#include "ScreenShotFrame.h"
enum
{
   ID_Hello = 1
};
 
class MyApp : public wxApp
{
public:

    virtual bool OnInit();
    void OnIdle(wxIdleEvent& evt);
    int OnExit();


    void initStateHandlers();
    void handleSelect();
    void handleAreaSelect();
    void handleToClipBoard();
    void handleLatest();
    void handleCancel();

    bool closed_flag=false;
    ControlPanelFrame* cpFrame;
    ScreenShotFrame* frame_screenshot;
    wxBitmap m_bmpBackground;
    wxImage screenShot;

    std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates;

    std::unordered_map<State, std::function<void()>> stateHandlers;

    bool screeshot_taken;
    int pass_once;

};


wxIMPLEMENT_APP(MyApp);
 



int MyApp::OnExit()
{
    frame_screenshot->Destroy();
    return 0;
}


bool MyApp::OnInit() {

    initStateHandlers();
    wxWindowID cpID = wxIdManager::ReserveId(1);
    cpFrame = new ControlPanelFrame("Control Panel", cpID);
    cpFrame->Show(true);

    auto ptr = std::make_shared<std::tuple<int, int, int, int>>(-1, 0, 0, 0);
    squareCoordinates = ptr;
    wxWindowID screenshotID = wxIdManager::ReserveId(1);
    frame_screenshot = new ScreenShotFrame(squareCoordinates, screenshotID);  
        
    screeshot_taken = false;
    pass_once = 0;
    Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::OnIdle) );
    return true;
}
 
void MyApp::initStateHandlers() {
    stateHandlers = {
        { State::STATE_Select,    [this]() { handleSelect(); } },
        { State::STATE_AreaSelect, [this]() { handleAreaSelect(); } },
        { State::STATE_ToClipboard, [this]() { handleToClipBoard(); } },
        { State::STATE_Latest, [this]() { handleLatest(); } },
        { State::STATE_Cancel, [this]() { handleCancel(); } }
    };
}

void MyApp::handleSelect(){
    //std::cout << "handling select" << std::endl;
    cpFrame->Set_State(State::STATE_Select);
}

void MyApp::handleAreaSelect(){
   //std::cout << "handling area select" << std::endl;
   if(!screeshot_taken) {
        cpFrame->Show(false);
        wxScreenDC dc;
        wxSize size = dc.GetSize();
        frame_screenshot->update_screen_size(size);
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
        frame_screenshot->update_screenshot(screenShot); 
        screeshot_taken = true;

        frame_screenshot->ShowFullScreen(true);
        frame_screenshot->Show(true);

    }
    
    if(screeshot_taken && frame_screenshot->get_mouse_released()){
    //    frame_screenshot->ShowFullScreen(true);
        frame_screenshot->Show(true);
        cpFrame->Show(true);
        cpFrame->Raise();
    } 
    if (!frame_screenshot->get_mouse_released()) {
        frame_screenshot->render();
        frame_screenshot->Show(true);
    }
}

void MyApp::handleCancel(){
    frame_screenshot->Show(false);
    screeshot_taken = false;
    cpFrame->Set_State(State::STATE_Select);
}

void MyApp::handleToClipBoard(){
    std::cout << "handling ToClipBoard" << std::endl;
    cpFrame->Set_State(State::STATE_Select);
}
void MyApp::handleLatest(){
    std::cout << "handling latest" << std::endl;
    cpFrame->Set_State(State::STATE_Select);
}


void MyApp::OnIdle(wxIdleEvent& evt) {
     
    if(cpFrame->IsBeingDeleted()){
        Exit();
    }
     
    State current = cpFrame->Get_State();
    
    auto it = stateHandlers.find(cpFrame->Get_State());
    
    if (it != stateHandlers.end()) {
        it->second();
    } else {
        std::cout << "unknown state" << std::endl;
    }
    

}
