#ifndef __LOGSTREAM_H__
#define __LOGSTREAM_H__


#include <assert.h>
#include <string.h> // memcpy
#include <string>



const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000*1000;

template<int SIZE>
class FixedBuffer
{
 public:
  FixedBuffer()
    : cur_(data_)
  {
   
  }

  ~FixedBuffer()
  {
    
  }

  void append(const char* /*restrict*/ buf, size_t len)
  {
    // FIXME: append partially
	//只能执行向上转换
    if (static_cast<size_t>(avail()) > len)
    {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char* data() const { return data_; }
  int length() const { return static_cast<int>(cur_ - data_); }

  // write to data_ directly
  char* current() { return cur_; }
  int avail() const { return static_cast<int>(end() - cur_); }
  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }
//  void bzero() { bzero(data_, sizeof(data_)); }

  // for used by GDB
  const char* debugString();
  
  // for used by unit test
  std::string toString() const { return std::string(data_, length()); }
  std::string toStringPiece() const { return std::string(data_, length()); }

 private:
  const char* end() const { return data_ + sizeof data_; }
  // Must be outline function for cookies.


  char data_[SIZE];
  char* cur_;
};



class LogStream 
{
  typedef LogStream self;
 public:
  typedef FixedBuffer<kSmallBuffer> Buffer;

  self& operator<<(bool v)
  {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
  }

  self& operator<<(short);
  self& operator<<(unsigned short);
  self& operator<<(int);
  self& operator<<(unsigned int);
  self& operator<<(long);
  self& operator<<(unsigned long);
  self& operator<<(long long);
  self& operator<<(unsigned long long);

  self& operator<<(const void*);

  self& operator<<(float v)
  {
    *this << static_cast<double>(v);
    return *this;
  }
  self& operator<<(double);
  // self& operator<<(long double);

  self& operator<<(char v)
  {
    buffer_.append(&v, 1);
    return *this;
  }

  // self& operator<<(signed char);
  // self& operator<<(unsigned char);

  self& operator<<(const char* str)
  {
    if (str)
    {
      buffer_.append(str, strlen(str));
    }
    else
    {
      buffer_.append("(null)", 6);
    }
    return *this;
  }

  self& operator<<(const unsigned char* str)
  {
    return operator<<(reinterpret_cast<const char*>(str));
  }

  /*self& operator<<(const string& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }*/

  self& operator<<(const std::string& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }



  self& operator<<(const Buffer& v)
  {
    *this << v.toStringPiece();
    return *this;
  }

  void append(const char* data, int len) { buffer_.append(data, len); }
  const Buffer& buffer() const { return buffer_; }
  void resetBuffer() { buffer_.reset(); }

 private:
  

  template<typename T>
  void formatInteger(T);

  Buffer buffer_;

  static const int kMaxNumericSize = 32;
};




#endif  // MUDUO_BASE_LOGSTREAM_H
