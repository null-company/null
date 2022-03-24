#pragma once

#include <scene.pb.h>

class Serializable {
    // Suppose in every method make downcast to the specific message type

    virtual void serialize(google::protobuf::Message *message) = 0;
    virtual void deserialize(google::protobuf::Message *message) = 0;
};