#include "Network/networkmanager.h"

NetworkManager::NetworkManager():
        target_(DEFAULT_TRANSLATION_SERVICE_HOST) {
}

static std::shared_ptr<NetworkManager> NetworkManager::getInstance() {
    return instance_;
}

std::shared_ptr<TranslationService> NetworkManager::getTranslationService() {
    if (translationService_) {
        return translationService_;
    }
    else {
        return translationService_.reset(new TranslationService(target_));
    }
}
