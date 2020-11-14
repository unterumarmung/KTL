#pragma once
#include <ntddk.h>
#include <runtime/basic_runtime.h>

namespace winapi::kernel {
struct NtSuccess {
  constexpr bool operator()(NTSTATUS status) const noexcept {
    return NT_SUCCESS(status);
  }
};

template <NTSTATUS expected>
struct NtStatusEqual {
  constexpr bool operator()(NTSTATUS status) const noexcept {
    return status == expected;
  }
};
}