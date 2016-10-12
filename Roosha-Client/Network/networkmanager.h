#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "translation_service.h"

#define DEFAULT_TRANSLATION_SERVICE_HOST "localhost:1543"

class NetworkManager {
 public:
    static std::shared_ptr<NetworkManager> getInstance();

    std::shared_ptr<TranslationService> getTranslationService();
 private:
    NetworkManager();

    static std::shared_ptr<NetworkManager> instance_(NetworkManager());

    grpc::string target_;
    std::shared_ptr<TranslationService> translationService_;
};

#endif // NETWORKMANAGER_H