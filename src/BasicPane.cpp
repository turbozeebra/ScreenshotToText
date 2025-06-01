#include "BasicPane.h"

BasicPane::BasicPane(wxFrame* parent, std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates) :
wxPanel(parent), squareCoordinates(squareCoordinates)
{
    rec_to_draw=false;
    mouse_released = false;

    Bind(wxEVT_LEFT_DOWN, &BasicPane::MousePressed, this);
    Bind(wxEVT_LEFT_UP, &BasicPane::MouseReleased, this);
    Bind(wxEVT_MOTION, &BasicPane::OnMotion, this);
    Bind(wxEVT_PAINT, &BasicPane::paintEvent, this);
 
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

 void BasicPane::update_screen_size(wxSize size){
    screen_size = size;
 }

bool BasicPane::get_mouse_released(){
    return mouse_released;
}
void BasicPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void BasicPane::MouseReleased(wxMouseEvent& event) 
{    
    wxPoint pt = wxGetMousePosition();
    int mouseX = pt.x;
    int mouseY = pt.y;
    updateSquareCoordinates();
    mouse_released = true;
    
}
void BasicPane::updateSquareCoordinates()
{

    std::get<0>(*squareCoordinates) = int(startPos.x);
    std::get<1>(*squareCoordinates) = int(startPos.y);
    std::get<2>(*squareCoordinates) = int(w);
    std::get<3>(*squareCoordinates) = int(h);
}


void BasicPane::MousePressed(wxMouseEvent& event) 
{
    bitmap_old_initialized = false;
    rec_to_draw = false;
    wxPoint pt = wxGetMousePosition();
    setStartPos(pt);
    mouse_released = false;
}


/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 *
 * In most cases, this will not be needed at all; simply handling
 * paint events and calling Refresh() when a refresh is needed
 * will do the job.
 */
void BasicPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */

void BasicPane::second_render(wxDC& dc, wxImage img){
    if (rec_to_draw) 
    {
        wxRect red_square = normal_rect(startPos.x, startPos.y, w,h);
        int difference = 10;
       
        wxRect image_square = wxRect(red_square.x - difference, red_square.y - difference, red_square.width + 2*difference,red_square.height + 2*difference);

        int area = red_square.height * red_square.width;
        if(area > 10){
            
            m_subImage = std::move(img.GetSubImage(image_square));
            wxBitmap bitmap_rec(m_subImage);
            if (!bitmap_old_initialized) {
                bitmap_old = bitmap_rec;
                bitmap_old_initialized=true;
                x_old = image_square.x;
                y_old = image_square.y;
            }

            dc.DrawBitmap(bitmap_old, x_old , y_old, false);
            bitmap_old = std::move(bitmap_rec);
            x_old = image_square.x;
            y_old = image_square.y;
            dc.SetPen(*wxRED);
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRectangle(red_square);
               
        }

    }
}

void BasicPane::render(wxDC& dc)
{
    
     if (rec_to_draw) {
        wxRect rect = normal_rect(std::get<0>(*squareCoordinates),std::get<1>(*squareCoordinates),std::get<2>(*squareCoordinates),std::get<3>(*squareCoordinates));
            
        dc.SetPen(*wxRED);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(rect);

    }
}

void BasicPane::OnMotion(wxMouseEvent& event)
{
    if(event.Dragging())
    {
        rec_to_draw = true;
        const wxPoint current = event.GetPosition();
        if (current.x > 10 && current.x < (screen_size.x - 10) && current.y > 10 && current.y < (screen_size.y - 10)){
            w = current.x - startPos.x;
            h = current.y - startPos.y;        

        }
    }
}


wxRect BasicPane::normal_rect( int x,  int y,  int w,  int h) const
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

    return std::move(wxRect(x1,y1,w1,h1));
}

