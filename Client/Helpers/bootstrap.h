#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

class CentralController;

class Bootstrap {
 public:
    Bootstrap(CentralController *cc_);

    void run();

 private:
    CentralController *cc_;
};

#endif // BOOTSTRAP_H
