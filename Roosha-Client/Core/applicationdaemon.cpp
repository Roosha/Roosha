#include "Core/applicationdaemon.h"
#include "Core/centralcontroller.h"

ApplicationDaemon::ApplicationDaemon(int argc, char **argv) : QtService<QApplication>(argc, argv, "Roosha") {
    setServiceDescription("Roosha - spaces repetition flashcard program");
    setServiceFlags(QtServiceBase::Default);
    //    setStartupType(type); TODO: this line need to run daemon during system startup.
}

ApplicationDaemon::~ApplicationDaemon(){
}



void ApplicationDaemon::start() { // aka main

    CentralController centralController;
    centralController.start();

}

void ApplicationDaemon::stop() {

}

void ApplicationDaemon::pause() {

}

void ApplicationDaemon::resume() {

}

void ApplicationDaemon::processCommand(int code) {

}
