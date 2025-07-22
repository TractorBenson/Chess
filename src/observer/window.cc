#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) : width{width}, height{height} {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  //////////////////////////////////////////////////
  // below are adjusted in project

  struct RGB {
    string name;
    int r,g,b;
  };
  
  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  RGB color_vals[3] = {
    {"Black", 0,0,0},
    {"White", 255,255,255},
    {"ChessWhite", 235,236,208},
    {"ChessGreen", 115,149,82},
  }

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 4; ++i) {
      xcolor.red = color_vals[i].r * 257;
      xcolor.green = color_vals[i].g * 257; 
      xcolor.blue = color_vals[i].b * 257;
      xcolor.flag = DoRed | DoGreen | DoBlue;
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }
  ////////////////////////////////////////////////////

  XSetForeground(d,gc,colours[black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

int Xwindow::getWidth() const { return width; }
int Xwindow::getHeight() const { return height; }

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg) {
  XDrawString(d, w, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
}

