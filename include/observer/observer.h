#ifndef OBSERVER_H
#define OBSERVER_H
// make sure only include once

class Square; // forward declaration

class Observer {
    virtual void notify(Square&) = 0;
};

#endif
