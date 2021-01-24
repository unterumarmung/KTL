#pragma once
#include <basic_types.h>

namespace ktl {
using exc_char_t = wchar_t;

struct constexpr_message_tag {};

namespace crt {
struct exception_data {
  const exc_char_t* str;
  size_t ref_counter;
};

class exception_base {
 private:
  static constexpr size_t SHARED_DATA_MASK{1};

 public:
  exception_base(const exc_char_t* msg);
  exception_base(const exc_char_t* msg, size_t length);

  template <size_t N>
  constexpr exception_base(const exc_char_t (&msg)[N])
      : exception_base(msg, N) {}

  constexpr exception_base(const exc_char_t* msg,
                           constexpr_message_tag) noexcept
      : m_data{msg} {}

  exception_base(const exception_base& other);
  exception_base& operator=(const exception_base& other);
  virtual ~exception_base();

 protected:
  const exc_char_t* exception_base::get_message() const noexcept;

 private:
  bool has_shared_data() const noexcept;
  exception_data* as_shared_data() const noexcept;

  static void* create_masked_shared_data(const exc_char_t* msg,
                                         size_t msg_length) noexcept;
  static exception_data* create_shared_data(
      const exc_char_t* msg,
      size_t msg_length) noexcept;  // terminate if fails
  static void destroy_shared_data(exception_data* target) noexcept;

 private:
  const void* m_data;  //!< exception_data* or exc_char_t*
};
}  // namespace crt

class exception : public crt::exception_base {
 public:
  using MyBase = crt::exception_base;

 public:
  using MyBase::MyBase;
  virtual const exc_char_t* what() const noexcept { return get_message(); }
};

class bad_alloc : public exception {
 public:
  using MyBase = exception;

 public:
  bad_alloc() noexcept;

 protected:
  using MyBase::MyBase;
};

class bad_array_length : public bad_alloc {
 public:
  bad_array_length() noexcept;
};

}  // namespace ktl