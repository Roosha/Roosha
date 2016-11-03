#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "roosha_service.h"

#define DEFAULT_ROOSHA_SERVICE_HOST "localhost:1543"

class NetworkManager {
public:
    NetworkManager();

    std::shared_ptr<RooshaRpcService> getTranslationService();
private:

    static std::shared_ptr<NetworkManager> instance_(NetworkManager());

    grpc::string target_;
    std::shared_ptr<RooshaRpcService> translationService_;
};

#endif // NETWORKMANAGER_H
