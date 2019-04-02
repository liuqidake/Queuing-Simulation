//
//  event.cpp
//  queuingSimulation
//
//  Created by Erik Poole on 3/31/19.
//  Copyright © 2019 ErikPoole. All rights reserved.
//

#include "event.hpp"

event::event(long inputEventTime, long inputServiceTime, long inputTimeTaken, long inputLine) {
    eventTime = inputEventTime;
    serviceTime = inputServiceTime;
    timeTaken = inputTimeTaken;
    lineNumber = inputLine;
}


void event::handleEvent(long &globalTime, std::vector<line> &lines, std::priority_queue<event*, std::vector<event*>, compareEvents> &eventQueue) {
    assert(false);
}


void newPersonEvent::handleEvent(long &globalTime, std::vector<line> &lines, std::priority_queue<event*, std::vector<event*>, compareEvents> &eventQueue) {
//        std::cout << "New Person\n";

    for (line l : lines) {
        l.syncLine(eventTime-globalTime);
    }
    globalTime = eventTime;
    
    lineNumber = determineLine(lines);
    
    //PROBLEM - inputTimeTaken is aggregate of ALL people in line and assumes just one register
//    tellerFreedEvent* eventPtr = new tellerFreedEvent(globalTime+serviceTime+currentLine.totalLineTime, serviceTime, serviceTime+(currentLine.totalLineTime/6));
    
    tellerFreedEvent* eventPtr = new tellerFreedEvent(globalTime+serviceTime+lines[lineNumber].totalLineTime, serviceTime, serviceTime+lines[lineNumber].totalLineTime, lineNumber);
    eventQueue.push(eventPtr);
    if (lines[lineNumber].registers > 0) {
        lines[lineNumber].registers--;
    } else {
        lines[lineNumber].addCustomer(serviceTime);
    }
}


void tellerFreedEvent::handleEvent(long &globalTime, std::vector<line> &lines, std::priority_queue<event*, std::vector<event*>, compareEvents> &eventQueue) {
//    std::cout << "Teller Freed\n";
    
    for (line l : lines) {
        l.syncLine(eventTime-globalTime);
    }
    globalTime = eventTime;
    
//    std::cout << lines[lineNumber].totalLineTime << "\n";
    if (lines[lineNumber].totalLineTime <= 0) {
        lines[lineNumber].registers++;
    }
}


bool compareEvents::operator()(event* rhs, event* lhs) {
    return rhs->eventTime > lhs->eventTime;
}



int determineLine(std::vector<line> lines) {
    long lowTime = lines[0].totalLineTime;
    int index = 0;
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].registers > 0) {
            return i;
        }
    }
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].totalLineTime < lowTime) {
            lowTime = lines[i].totalLineTime;
            index = i;
        }
    }
    return index;
}


void printPercentiles(std::vector<long> inputVector) {
    
    
    double valuesPerPercentile = (double) inputVector.size() / 100;

    int tenthPercentileIndex = (int) 10*valuesPerPercentile;
    int fiftiethPercentileIndex = (int) 50*valuesPerPercentile;
    int nintiethPercentileIndex = (int) 90*valuesPerPercentile;
    
    double tenthPercentile = (double) inputVector[tenthPercentileIndex]/60;
    double fiftiethPercentile = (double) inputVector[fiftiethPercentileIndex]/60;
    double nintiethPercentile = (double) inputVector[nintiethPercentileIndex]/60;

//    std::cout << inputVector.size() << "\n";
//    std::cout << valuesPerPercentile << "\n";
//    std::cout << tenthPercentile << " " << tenthPercentileIndex << "\n";
//    std::cout << fiftiethPercentile << " " << fiftiethPercentileIndex << "\n";
//    std::cout << nintiethPercentile << " " << nintiethPercentileIndex << "\n";
    
    std::cout << "10th percenile " << tenthPercentile << "\n50th percentile " << fiftiethPercentile << "\n90th percentile " << nintiethPercentile << "\n\n";
}
