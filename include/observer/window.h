#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

//////////////////
// modified in the project

#include <unordered_map>

class Xwindow {
  Display *d;
  Window w;
  int s, width, height;
  GC gc;
  unsigned long colours[5];

  struct PixmapPair {
    Pixmap color; // bmp to draw
    Pixmap mask; // bit map, use for mask
    int w, h;
  };

  // use to store all pictures loaded, {name, PixmapPair}
  std::unordered_map<std::string, PixmapPair> cache;

  // two helper function
  Pixmap makeColorPixmap(const unsigned char *rgba, int w, int h);
  Pixmap makeMaskPixmap(const unsigned char *rgba, int w, int h);

  // function will be called when init, load all images
  void loadPixmap(std::string key, std::string path);

 public:
  Xwindow(int width=500, int height=500);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.



  enum {Black=0, White, ChessWhite, ChessGreen}; // Available colours.

  int getWidth() const;
  int getHeight() const;

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour);

  // Draws a string
  void drawString(int x, int y, std::string msg, int colour);

  void drawPic(std::string key, int x, int y);

//////////////////
};

#endif
