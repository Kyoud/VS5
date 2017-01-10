/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef lamport_TYPES_H
#define lamport_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>




typedef struct _TimeStamp__isset {
  _TimeStamp__isset() : time(false), id(false) {}
  bool time;
  bool id;
} _TimeStamp__isset;

class TimeStamp {
 public:

  static const char* ascii_fingerprint; // = "F33135321253DAEB67B0E79E416CA831";
  static const uint8_t binary_fingerprint[16]; // = {0xF3,0x31,0x35,0x32,0x12,0x53,0xDA,0xEB,0x67,0xB0,0xE7,0x9E,0x41,0x6C,0xA8,0x31};

  TimeStamp() : time(0), id(0) {
  }

  virtual ~TimeStamp() throw() {}

  int64_t time;
  int64_t id;

  _TimeStamp__isset __isset;

  void __set_time(const int64_t val) {
    time = val;
  }

  void __set_id(const int64_t val) {
    id = val;
  }

  bool operator == (const TimeStamp & rhs) const
  {
    if (!(time == rhs.time))
      return false;
    if (!(id == rhs.id))
      return false;
    return true;
  }
  bool operator != (const TimeStamp &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TimeStamp & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(TimeStamp &a, TimeStamp &b);



#endif