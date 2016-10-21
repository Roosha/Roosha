#ifndef APPLICATIONDAEMON_H
#define APPLICATIONDAEMON_H
#include <QApplication>
#include <QtService>

class ApplicationDaemon : public QtService<QApplication> {
public:
    ApplicationDaemon(int argc, char ** argv);
    ~ApplicationDaemon();

protected:
    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);
};

#endif // APPLICATIONDAEMON_H
