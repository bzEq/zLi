#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <cstdlib>
#include <cstdio>
#include <iostream>

class MyPaint: public Fl_Window {
public:
  MyPaint(int x, int y): Fl_Window(x,y) { }
  void draw() {
    fl_color(1024);
    fl_circle(320, 240, 10);
  }
};

int main(int argc, char* argv[])
{
  MyPaint window(640, 480);
  window.end();
  window.show();
  return Fl::run();
}
