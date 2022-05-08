#pragma once

#include <google/protobuf/message.h>

#include "serialized/serverConfig.pb.h"

class Serializable {
    // Suppose in every method make downcast to the specific message type
    virtual void serialize(google::protobuf::Message *message) = 0;
};
