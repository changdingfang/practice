#ifndef _server_controller_src_common_data_h
#define _server_controller_src_common_data_h

#include <stdint.h>
#include <string.h>

class Data
{
public:
  Data();

  Data(uint8_t* buf, uint64_t buf_len);

  ~Data();

  Data(const Data& val);

  Data(uint64_t capacity);

  Data& operator=(const Data& val);

  bool operator!=(const Data& val);

  bool operator==(const Data& val);

  void Copy(const uint8_t* buf, uint64_t buf_len);

  void Assign(uint8_t* buf, uint64_t buf_len);

  void Append(const uint8_t* buf, uint64_t buf_len);

  void Append(const Data& data);

  void Subdata(uint64_t pos, void* data, uint64_t len = 0) const;

  Data Subdata(uint64_t pos, uint64_t len = 0) const;

  uint64_t Find(const uint8_t* data, uint64_t len) const;

  uint64_t Find(const Data& data) const;

  uint64_t Compare(const uint8_t* buf, uint64_t len) const;

  uint64_t Compare(const Data& data) const;

  inline const uint8_t* Buffer() const
  {
    return buf_;
  }

  inline uint64_t Size() const
  {
    return len_;
  }

  inline bool Empty() const
  {
    return (buf_ == 0);
  }

  inline void Clear()
  {
    delete[] buf_;
    buf_ = 0;
    len_ = 0;
  }

  inline uint64_t Erase(uint64_t pos, uint64_t size)
  {
    if (pos < 0 || size < -1) {
      return -1;
    }

    uint64_t new_buf_len = 0;
    uint8_t* new_buf = 0;

    if (pos == 0) {
      if (size < len_) {
        new_buf_len = len_ - size;
        new_buf = new uint8_t[new_buf_len];
        memcpy(new_buf, buf_ + size, new_buf_len);
      }
    }
    else {
      if (pos + size < len_) {
        new_buf_len = len_ - size;
        new_buf = new uint8_t[new_buf_len];
        memcpy(new_buf, buf_, pos + 1);
        memcpy(new_buf + pos + 1, buf_ + pos + size, size);
      }
      else {
        new_buf_len = pos;
        new_buf = new uint8_t[new_buf_len];
        memcpy(new_buf, buf_, pos);
      }
    }
    delete[] buf_;
    buf_ = new_buf;
    len_ = new_buf_len;
    return len_;
  }

private:
  uint8_t* buf_;

  uint64_t len_;

  uint64_t capacity_;
};

#endif //_server_controller_src_common_data_h
