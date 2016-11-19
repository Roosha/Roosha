#include "translations_test_slot_holder.h"

TranslationsTestSlotHolder::TranslationsTestSlotHolder() {
}

void TranslationsTestSlotHolder::translationSucceededSlot(quint32 id, Translations translations) {
    std::cout << "Received translation!" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "translations.source:" << translations[0]->getSource().toStdString() << std::endl;
    for (auto translation: translations) {
        for (auto target: translation->getTarget()) {
            std::cout << "target: " << target.toStdString() << std::endl;
        }
        for (auto example: translation->getExamples()) {
            std::cout << "example: " << example.toStdString() << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------------" << std::endl;
}

void TranslationsTestSlotHolder::translationFailedSlot(quint32 id, RPCErrorStatus status) {
    std::cout << "Received error" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "error: " << status << std::endl;
//    std::cout << "error message: " << status.error_message() << std::endl;
    std::cout << std::endl << "-----------------------------------------" << std::endl;
}
