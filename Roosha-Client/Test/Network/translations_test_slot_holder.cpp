#include "translations_test_slot_holder.h"

TranslationsTestSlotHolder::TranslationsTestSlotHolder() {
}

void TranslationsTestSlotHolder::translationSucceededSlot(quint32 id, Translations translations) {
    std::cout << "Received translation!" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "translations.source:" << translations.source() << std::endl;
    for (int i = 0; i < translations.translation_size(); i++) {
        auto tr = translations.translation(i);
        for (int j = 0; j < tr.target_size(); j++) {
            std::cout << "target: " << tr.target(j) << std::endl;
        }
        for (int j = 0; j < tr.example_size(); j++) {
            std::cout << "example: " << tr.example(j) << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------------" << std::endl;
}

void TranslationsTestSlotHolder::translationFailedSlot(quint32 id, grpc::Status status) {
    std::cout << "Received error" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "error code: " << status.error_code() << std::endl;
    std::cout << "error message: " << status.error_message() << std::endl;
    std::cout << std::endl << "-----------------------------------------" << std::endl;
}
