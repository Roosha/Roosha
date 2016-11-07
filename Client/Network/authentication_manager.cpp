#include "authentication_manager.h"
#include "roosha_service_connector.h"
#include "network_manager.h"

AuthenticationManager::AuthenticationManager(NetworkManager *n) :
    connector(new RooshaServiceConnector(this)),
    netManager(n) {
}
