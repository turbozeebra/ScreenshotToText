// wxWidgets "Hello World" Program

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/clipbrd.h>
#include <wx/wx.h>

#include "ControlPanelFrame.h"
#include "ScreenshotFrame.h"

#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

#include <chrono>
#include <thread>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
    void OnIdle(wxIdleEvent &evt);
    int OnExit();

    void initStateHandlers();
    void handleSelect();
    void handleAreaSelect();
    void handleToClipBoard();
    void handleLatest();
    void handleCancel();

    wxRect *normal_rect(const int x, const int y, const int w,
                        const int h) const;
    std::string readText(const wxImage subImg) const;
    void toClipboard(std::string txt);

    bool closed_flag = false;
    bool cp_frame_closing = false;
    ControlPanelFrame *m_control_panel_frame;
    ScreenshotFrame *m_screenshot_frame;
    wxBitmap m_bmpBackground;
    wxImage m_screenshot;
    wxWindowID m_screenshotID;

    std::string ocrTxt;
    std::shared_ptr<std::tuple<int, int, int, int>> m_square_coordinates;

    std::unordered_map<State, std::function<void()>> state_handlers;

    bool screeshot_taken;
    int pass_once;
};

wxIMPLEMENT_APP(MyApp);

int MyApp::OnExit() {
    if (cp_frame_closing) {
        m_screenshot_frame->Destroy();
    } else {
        m_control_panel_frame->Destroy();
    }
    return 0;
}

bool MyApp::OnInit() {

    initStateHandlers();
    wxWindowID cpID = wxIdManager::ReserveId(1);
    m_control_panel_frame = new ControlPanelFrame("Control Panel", cpID);
    m_control_panel_frame->Show(true);

    auto ptr = std::make_shared<std::tuple<int, int, int, int>>(-1, 0, 0, 0);
    m_square_coordinates = ptr;
    m_screenshotID = wxIdManager::ReserveId(1);
    m_screenshot_frame =
        new ScreenshotFrame(m_square_coordinates, m_screenshotID);

    screeshot_taken = false;
    pass_once = 0;
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::OnIdle));

    wxImage::AddHandler(new wxPNGHandler());
    return true;
}

void MyApp::initStateHandlers() {
    state_handlers = {
        {State::STATE_Select, [this]() { handleSelect(); }},
        {State::STATE_AreaSelect, [this]() { handleAreaSelect(); }},
        {State::STATE_ToClipboard, [this]() { handleToClipBoard(); }},
        {State::STATE_Latest, [this]() { handleLatest(); }},
        {State::STATE_Cancel, [this]() { handleCancel(); }}};
}

void MyApp::handleSelect() {
    m_control_panel_frame->Set_State(State::STATE_Select);
}

void MyApp::handleAreaSelect() {

    if (!m_screenshot_frame->IsBeingDeleted() &&
        !m_control_panel_frame->IsBeingDeleted()) {
        if (!screeshot_taken) {
            m_control_panel_frame->Hide();
            m_control_panel_frame->Refresh();
            m_screenshot_frame->Refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(180));
            wxScreenDC dc;
            wxSize size = dc.GetSize();
            m_screenshot_frame->update_screen_size(size);
            wxBitmap bmp(size.x, size.y);
            wxMemoryDC memDC(bmp);
            memDC.Blit(0, 0, size.x, size.y, &dc, 0, 0);
            memDC.SelectObject(wxNullBitmap);
            m_screenshot = bmp.ConvertToImage();
            // this is how you save an image
            // img.SaveFile("./test.png", wxBITMAP_TYPE_PNG); this is how you
            // save an image these are the coordinates that are going to be used
            // to determine wanted area of the screen
            m_screenshot_frame->update_screenshot(m_screenshot);
            screeshot_taken = true;

            m_screenshot_frame->ShowFullScreen(true);
            m_screenshot_frame->Show(true);
            // m_control_panel_frame->Raise();
        }

        if (screeshot_taken && m_screenshot_frame->get_mouse_released()) {
            m_screenshot_frame->Show(true);
            m_control_panel_frame->Show(true);
            m_control_panel_frame->Raise();
        }
        if (!m_screenshot_frame->get_mouse_released()) {
            m_screenshot_frame->render();
            m_screenshot_frame->Show(true);
        }
    }
}

void MyApp::handleCancel() {
    m_screenshot_frame->Show(false);
    screeshot_taken = false;
    m_control_panel_frame->Set_State(State::STATE_Select);
}

void MyApp::handleToClipBoard() {
    screeshot_taken = false;
    m_screenshot_frame->Show(false);

    wxRect *rect = normal_rect(
        std::get<0>(*m_square_coordinates), std::get<1>(*m_square_coordinates),
        std::get<2>(*m_square_coordinates), std::get<3>(*m_square_coordinates));
    if (rect->GetX() > -1) {
        wxImage subImg = m_screenshot.GetSubImage(*rect);

        ocrTxt = readText(subImg);

        toClipboard(ocrTxt);
        // Write some text to the clipboard
    }
    m_control_panel_frame->Set_State(State::STATE_Select);
}
void MyApp::handleLatest() {

    if (!ocrTxt.empty()) {
        toClipboard(ocrTxt);
    }

    m_control_panel_frame->Set_State(State::STATE_Select);
}

void MyApp::OnIdle(wxIdleEvent &evt) {

    if (m_control_panel_frame->IsBeingDeleted()) {
        cp_frame_closing = true;
        Exit();
    }

    if (m_screenshot_frame->IsBeingDeleted()) {
        cp_frame_closing = false;
        Exit();
    }

    State current = m_control_panel_frame->Get_State();

    auto it = state_handlers.find(m_control_panel_frame->Get_State());

    if (it != state_handlers.end()) {
        it->second();
    } else {
        std::cout << "unknown state" << std::endl;
    }
}

std::string MyApp::readText(const wxImage subImg) const {
    char *outText;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    int width = subImg.GetWidth();
    int height = subImg.GetHeight();
    const unsigned char *rgb_buffer = subImg.GetData();
    api->SetImage(rgb_buffer, width, height, 3, width * 3);

    // Get OCR result
    outText = api->GetUTF8Text();
    // printf("%s", outText);

    // Destroy used object and release memory
    api->End();
    delete api;
    std::string str(outText);
    return std::move(str);
}
void MyApp::toClipboard(std::string txt) {
    if (wxTheClipboard->Open()) {
        // This data objects are held by the clipboard,
        // so do not delete them in the app.
        wxTheClipboard->SetData(new wxTextDataObject(txt.c_str()));
        // wxTheClipboard->Flush();
        wxTheClipboard->Close();
    }
}

wxRect *MyApp::normal_rect(const int x, const int y, const int w,
                           const int h) const {
    int x1, y1, w1, h1;
    x1 = x;
    y1 = y;
    w1 = w;
    h1 = h;

    if (w < 0) {
        x1 += w;
        w1 = std::abs(w);
    }

    if (h < 0) {
        y1 += h;
        h1 = std::abs(h);
    }

    return new wxRect(x1, y1, w1, h1);
}