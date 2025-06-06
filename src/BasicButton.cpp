#include "BasicButton.h"

BasicButton::BasicButton(wxWindow *parent, int x, int y, int w, int h)
    : wxWindow(parent, wxID_ANY, wxPoint(x, y), wxSize(w, h)) {
  Bind(wxEVT_LEFT_DOWN, &BasicButton::OnClick, this);
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  this->cont = true;
  // Bind(wxEVT_LEFT_DOWN, &BasicPane::MousePressed, this);
}

bool BasicButton::render(wxDC &dc) {
  /*
  wxCoord w, h;
  dc.GetSize(&w, &h);
  float xStart = (float)(w)/ 2.0;
  float yStart = (float)(h) - 300.0;

  int w_box, h_box, scale;
  scale = 3;
  w_box = 90*scale;
  h_box = 30*scale;
  //yStart -= (float)(h_box) - 300.0;
  xStart -= ((float)(w_box) / 2.0);
  //dc.DrawRectangle(, (int)(yStart), w_box, h_box);
  */

  dc.SetPen(*wxBLUE);
  dc.SetBrush(*wxBLUE_BRUSH);
  dc.DrawRectangle((this->x), this->y, this->w, this->h);
  dc.DrawText("copy text", this->x + (this->w / 4.0), this->y);
  return this->cont;
}

void BasicButton::OnClick(wxMouseEvent &event) {
  this->cont = false;
  // std::cout << "mouse pressed" << std::endl;
}