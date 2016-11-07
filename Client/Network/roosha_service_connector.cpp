#include "roosha_service_connector.h"

RooshaServiceConnector::RooshaServiceConnector(AuthenticationManager *m) :
    listener(new AsyncRpcResponseListener(this)),
    authManager(m) {}

AsyncRpcResponseListener::AsyncRpcResponseListener(RooshaServiceConnector *r) :
    connector(r) {}
