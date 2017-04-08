//
// Created by gylo on 19.01.17.
//

#ifndef ROOSHACLIENT_ERROR_STATUS_H
#define ROOSHACLIENT_ERROR_STATUS_H

enum RPCErrorStatus {
    /// RPC exceeded specified timeout. There is no guarantee neither that RPC finished successfully nor it was aborted.
            DEADLINE_EXCEEDED,
    /// RPC failed due to missing or bad credentials
            NOT_AUTHENTICATED,
    /// This error status emerges if and only if RPC is either 'authorize' or 'registrate' and it's preceded by another
    /// one which has not completed yet.
            ALREADY_IN_AUTHNTICATION_PROCESS,
    /// Failed to connect to server.
            NO_CONNECTION,
    /// Server history changed during synchronization, need to retry
            CONCURRENT_HISTORY_MODIFICATION,
    /// Any other error.
            UNKNOWN,
};

Q_DECLARE_METATYPE(RPCErrorStatus);

#endif //ROOSHACLIENT_ERROR_STATUS_H
