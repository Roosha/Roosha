#include "Network/networkmanager.h"

NetworkManager::NetworkManager():
        target_(DEFAULT_TRANSLATION_SERVICE_HOST) {
}

std::shared_ptr<TranslationService> NetworkManager::getTranslationService() {
    if (!translationService_) {
        translationService_.reset(new TranslationService(target_));
    }
    return translationService_;
}
