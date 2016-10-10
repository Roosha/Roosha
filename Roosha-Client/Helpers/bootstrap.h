#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

class CentralController;

class Bootstrap {
 public:
    Bootstrap(CentralController * cc);

    void run();

 private:
    CentralController * cc;
};

#endif // BOOTSTRAP_H
