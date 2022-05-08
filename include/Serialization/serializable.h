#pragma once

#include <scene.pb.h>
#include <Script.hpp>
#include <google/protobuf/message.h>

#include "serialized/serverConfig.pb.h"

//template <class T>
//concept SerializableConcept = requires(google::protobuf::Message* m, T t) {
//    t.serialize(m);
//    T::deserialize(m);
//    dynamic_cast<T>(T::deserialize(m));
//    T();
//};

// OK this is useless
// we'll just have the functions in the specific script classes
// TODO
// DELETE IN COMMIT
template <typename T>
class Serializable {
    // Suppose in every method make downcast to the specific message type

    virtual void serialize(google::protobuf::Message *message);
    static std::unique_ptr<T> deserialize(google::protobuf::Message *message);
};
