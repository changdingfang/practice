#include "data.h"

Data::Data()
{
  len_ = 0;
  capacity_ = 1024;
  buf_ = new uint8_t[capacity_]();
}

Data::Data(uint8_t* buf, uint64_t buf_len)
{
  len_ = buf_len;
  capacity_ = 2 * buf_len;
  buf_ = new uint8_t[capacity_]();
  memcpy(buf_, buf, buf_len);
}

Data::Data(uint64_t capacity)
{
  capacity_ = capacity;
  len_ = 0;
  buf_ = new uint8_t[capacity_]();
}

Data::~Data()
{
  delete[] buf_;
  buf_ = 0;
}

Data::Data(const Data& val)
{
  if (val.Empty()) {
    len_ = 0;
    buf_ = 0;
    capacity_ = 0;
    return;
  }
  len_ = val.len_;
  capacity_ = val.capacity_;
  buf_ = new uint8_t[capacity_]();
  memcpy(buf_, val.buf_, len_);
}

Data& Data::operator=(const Data& val)
{
  if (&val == this) {
    return *this;
  }
  if (val.Empty()) {
    return *this;
  }

  len_ = val.len_;
  capacity_ = val.capacity_;
  delete[] buf_;
  buf_ = new uint8_t[capacity_]();
  memcpy(buf_, val.buf_, len_);
  return *this;
}

bool Data::operator!=(const Data& val)
{
  if (len_ != val.len_) {
    return true;
  }

  if (len_ == 0 && val.buf_ == 0 && buf_ == 0) {
    return true;
  }

  if (memcmp(buf_, val.buf_, len_) != 0) {
    return true;
  }

  return false;
}

bool Data::operator==(const Data& val)
{
  return !(*this != val);
}

void Data::Copy(const uint8_t* buf, uint64_t buf_len)
{
  if (buf == 0 || buf_len <= 0) {
    return;
  }

  len_ = buf_len;
  if (capacity_ < buf_len) {
    delete[] buf_;
    capacity_ = 2 * buf_len;
    buf_ = new uint8_t[capacity_]();
  }
  memcpy(buf_, buf, len_);
}

void Data::Assign(uint8_t* buf, uint64_t buf_len)
{
  delete buf_;
  buf_ = buf;
  len_ = buf_len;
  capacity_ = buf_len;
}

void Data::Append(const uint8_t* buf, uint64_t buf_len)
{
  if (buf == 0 || buf_len <= 0) {
    return;
  }
  if (capacity_ < len_ + buf_len) {
    capacity_ = 2 * (len_ + buf_len);
    uint8_t* tempBuf = new uint8_t[capacity_]();
    memcpy(tempBuf, buf_, len_);
    delete[] buf_;
    buf_ = tempBuf;
  }
  memcpy(buf_ + len_, buf, buf_len);
  len_ = len_ + buf_len;
}

void Data::Append(const Data &data)
{
  if (data.Empty()) {
    return;
  }
  Append(data.buf_, data.len_);
}

Data Data::Subdata(uint64_t pos, uint64_t len) const
{
  if (pos < 0 || len < 0 || (len_ - pos < len) || Empty()) {
    Data data;
    return data;
  }

  if (len == 0) {
    Data data(buf_ + pos, len_ - pos);
    return data;
  }

  Data data(buf_ + pos, len);
  return data;
}

void Data::Subdata(uint64_t pos, void *buf, uint64_t len) const
{
  //TRACE(10, "Data::Subdata "
  //      << CCommon::BinMsg((uint8_t*)buf_, len_));
  if (pos < 0 || len < 0 || (len_ - pos < len) || Empty()) {
    return;
  }

  if (len == 0) {
    memcpy(buf, buf_ + pos, len_ - pos);
    return;
  }

  memcpy(buf, buf_ + pos, len);
  //TRACE(10, "Data::Subdata sub str "
  //      << "pos[" << pos << "] "
  //      << CCommon::BinMsg((uint8_t*)buf, len_));
}

uint64_t Data::Find(const uint8_t* buf, uint64_t len) const
{
//  TRACE(10, "Data::Find base str "
//        << CCommon::BinToString(buf_, len_));
//  TRACE(10, "Data::Find sub str "
//        << CCommon::BinToString(buf, len));

  uint64_t pos = 0;
  uint64_t endPos = len_ - len;
  while (pos <= endPos) {
    if (memcmp(buf_ + pos, buf, len) == 0) {
      return pos;
    }
    ++pos;
  }
  return -1;
}

uint64_t Data::Find(const Data &data) const
{
  uint64_t pos = 0;
  uint64_t endPos = len_ - data.Size();
  while (pos <= endPos) {
    if (memcmp(buf_ + pos, data.buf_, data.Size()) == 0) {
      return pos;
    }
    ++pos;
  }
  return -1;
}

uint64_t Data::Compare(const uint8_t* buf, uint64_t len) const
{
  if (buf == 0 || len != len_) {
    return -1;
  }
  return memcmp(buf_, buf, len_);
}

uint64_t Data::Compare(const Data& data) const
{
  if (data.Size() != len_) {
    return -1;
  }
  return memcmp(buf_, data.buf_, len_);
}
