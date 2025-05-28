#include "BasicPane.h"

BasicPane::BasicPane(wxFrame* parent, std::shared_ptr<std::tuple<int, int, int,int>> squareCoordinates) :
wxPanel(parent), squareCoordinates(squareCoordinates)
{
    this->rec_to_draw=false;
   
    Bind(wxEVT_LEFT_DOWN, &BasicPane::MousePressed, this);
    Bind(wxEVT_LEFT_UP, &BasicPane::MouseReleased, this);
    Bind(wxEVT_MOTION, &BasicPane::OnMotion, this);
    Bind(wxEVT_ERASE_BACKGROUND, &BasicPane::OnEraseBackground, this);
    
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

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
}
void BasicPane::updateSquareCoordinates()
{
    std::get<0>(*squareCoordinates) = int(this->startPos.x);
    std::get<1>(*squareCoordinates) = int(this->startPos.y);
    std::get<2>(*squareCoordinates) = int(this->w);
    std::get<3>(*squareCoordinates) = int(this->h);
}


void BasicPane::MousePressed(wxMouseEvent& event) 
{
    this->rec_to_draw = false;
    wxPoint pt = wxGetMousePosition();
    this->setStartPos(pt);   
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
    this->render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */


void BasicPane::OnEraseBackground(wxEraseEvent&) {
    // Intentionally empty to prevent flicker
}


void BasicPane::render(wxDC&  dc)
{
     if (this->rec_to_draw) 
    {
        
       wxPen pen(*wxRED, 0.3);
       dc.SetPen(*wxRED);
       dc.SetBrush(*wxTRANSPARENT_BRUSH);
       wxRect recToDraw(this->startPos.x, this->startPos.y, this->w,this->h);
       dc.DrawRectangle(recToDraw);

    }
}

void BasicPane::OnMotion(wxMouseEvent& event)
{
    if(event.Dragging())
    {
        this->rec_to_draw = true;
        const wxPoint current = event.GetPosition();
        this->w = current.x - this->startPos.x;
        this->h = current.y - this->startPos.y;
        
    }
}
