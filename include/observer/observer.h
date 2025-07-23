#ifndef OBSERVER_H
#define OBSERVER_H
// make sure only include once

class Square; // forward declaration

class Observer {
public:
    virtual void notify(const Square&) = 0;
    virtual ~Observer() = default;
};

#endif
