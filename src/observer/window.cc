#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
// #include <memory>
#include <unistd.h>
#include "observer/window.h"


#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

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
  RGB color_vals[4] = {
    {"Black", 0,0,0},
    {"Yellow", 254,164,67},
    {"ChessWhite", 235,236,208},
    {"ChessGreen", 115,149,82}
  };

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 4; ++i) {
      xcolour.red = color_vals[i].r * 257;
      xcolour.green = color_vals[i].g * 257; 
      xcolour.blue = color_vals[i].b * 257;
      xcolour.flags = DoRed | DoGreen | DoBlue;
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }
  ////////////////////////////////////////////////////

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

#ifndef NOICON
  // load all images
  loadPixmap("BlackBishop", "assets/BlackBishop.png");
  loadPixmap("BlackKing", "assets/BlackKing.png");
  loadPixmap("BlackKnight", "assets/BlackKnight.png");
  loadPixmap("BlackPawn", "assets/BlackPawn.png");
  loadPixmap("BlackQueen", "assets/BlackQueen.png");
  loadPixmap("BlackRook", "assets/BlackRook.png");
  loadPixmap("WhiteBishop", "assets/WhiteBishop.png");
  loadPixmap("WhiteKing", "assets/WhiteKing.png");
  loadPixmap("WhiteKnight", "assets/WhiteKnight.png");
  loadPixmap("WhitePawn", "assets/WhitePawn.png");
  loadPixmap("WhiteQueen", "assets/WhiteQueen.png");
  loadPixmap("WhiteRook", "assets/WhiteRook.png");
#endif

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

void Xwindow::drawString(int x, int y, string msg, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
  XSetForeground(d, gc, colours[Black]);
}



/*
Sad story
I tried to use X11 to handle picture loading
but I failed to do resize
so I think it's a little dumb
so use char repre first
But I miss these work, so tmply comment out
*/

Pixmap Xwindow::makeColorPixmap(const unsigned char *rgba, int width, int height) {
  // create a Pixmap (my understanding - sub canvas) to store
  Pixmap pix = XCreatePixmap(d, w, width, height, DefaultDepth(d,DefaultScreen(d)));

  // init a default GC to draw pixels to pix
  GC gcTmp = XCreateGC(d, pix, 0, nullptr);

  // init a empty XImage, then fill corresponding fields
  XImage img = {};
  img.width = width;
  img.height = height;
  img.depth = DefaultDepth(d,DefaultScreen(d)); // init depth to default
  img.bits_per_pixel = 32; // format of provided png
  img.format = ZPixmap; // format of bmp
  img.byte_order = LSBFirst; // decode format, right (2^0) is smallest digit
  img.bitmap_unit = 32; // since png is 32bits per pixel
  img.bitmap_bit_order = LSBFirst; // keep the same as img.byte_order
  img.bitmap_pad = 32; // keep same with bitmap_unit

  img.bytes_per_line = width * 4;   // no padding

  // tell pure color (r,g,b)'s Hex code for decode
  img.red_mask = 0x00ff0000;
  img.green_mask = 0x0000ff00;
  img.blue_mask = 0x000000ff;
  
  img.data = reinterpret_cast<char*>( // because returned char* by std_image is unsigned, where data need signed
    const_cast<unsigned char*>( // because returned char* by stb_image is const, we do not change it, but X11 need it to be non-const
      rgba
    )
  );

  // init image with filled info
  XInitImage(&img);

  // store img with gcTmp in pix (in my word, a sub-canvas), which in (my word, canvas) d
  XPutImage(d, pix, gcTmp, &img, 0, 0, 0, 0, width, height);

  // free tmp GC
  XFreeGC(d, gcTmp);

  // return the Pixmap
  return pix;
}

Pixmap Xwindow::makeMaskPixmap(const unsigned char *rgba, int width, int height) {
  // we need one bit to store transparancy, and we have 32 pixel, so need four bytes
  size_t rowBytes = (width + 7) / 8; // take celling of width / 8

  // uint8_t - unsigned int with 8 bits, so can store bits in bytes
  vector<uint8_t> mask(rowBytes * height, 0); // init with (rowBytes * height) 0, that all are transparant

  // iterate all pixels, for pixels that is not transparant (since source is png), set its corresponding
  // value in mask to be 1, for later drawing
  for (int i = 0; i < height; ++i) {
    const unsigned char *row = rgba + i * width * 4; // which is one line of rgba
    for (int j = 0; j < width; ++j) {
      unsigned char alpha = row[j * 4 + 3]; // so now we get alpha from one single pixel
      if (alpha) {
        mask[i * rowBytes // i is row index, * rowBytes to locate initial position of target row
        + (j / 8)] // this pixel is jth pixel, / 8 to get in jth byte
        |= // bitwise or, use to replace target position 0 to 1
        (1 << (j % 8)); // use j % 8 to get index of bit in a byte, 1 << (j % 8) shift 1 to wanted position to bitwise or
        // shift left because LSBFirst, most right position is smallest, i.e. first bit in a byte
      }
    }
  }

  // turn mask into Pixmap
  // casting -> same reason as in makeColorPixmap, git rid of unsigned
  Pixmap pixmask = XCreateBitmapFromData(d, w, reinterpret_cast<char*>(mask.data()), width, height);

  // return resulting bitmap pixmap
  return pixmask;
}


void Xwindow::loadPixmap(std::string key, std::string path) {
  // if key already exist, return, avoid repetitively loading
  if (cache.find(key) != cache.end()) return;

  ////////////// decode png ///////////////
  int width, height, channels;
  unsigned char *rgba = stbi_load( // function from stb_image.h to handle png
    path.c_str(),  // since X11 use char* instead of std::string
    &width, &height, // function write width and height into them
    &channels, // function write number of channels in original picture
    4 // force number of channels to be 4
  );

  if (!rgba) { // if failed, function return nullptr
    cerr << "failed to decode png." << endl;
    return;
  }
  //////////////////////////////////////////
  

  // create final PixmapPair
  PixmapPair pp;
  pp.color = makeColorPixmap(rgba, width, height);
  pp.mask = makeMaskPixmap(rgba, width, height);
  pp.w = width;
  pp.h = height;

  // put in to cache
  cache.emplace(key,pp);

  // free image decoded
  stbi_image_free(rgba);
}

void Xwindow::drawPic(string key, int x, int y) {
  // find target pic, return if did not load
  auto pair = cache.find(key);
  if (pair == cache.end()) return;

  const PixmapPair pp = pair->second;
  // set clip area, clip all pixel with alpha = 0 (calculated in makeMastPixmap)
  XSetClipOrigin(d, gc, x, y);
  XSetClipMask(d, gc, pp.mask);

  XCopyArea(
    d, // canvas that the pic will be drawn
    pp.color, // image it self, stored in char*, format for X11
    w, // target window
    gc, // (in my word - the pen used to draw)
    0, 0, // origin of canvas
    pp.w, pp.h, // size of picture
    x, y); // wanted top left coordinate to draw

  XSetClipMask(d, gc, None); // reset clip info, prevent affect further drawing
}
