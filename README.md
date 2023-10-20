# Resultpp Library

Resultpp is a simple C++ library that provides a convenient way to encapsulate the result or outcome of operations. 

## Getting Started

### Prerequisites

- C++17 or later.

### Installation

Resultpp is a header-only library, so you only need to include the necessary header files in your project to use it. 
You can copy the `ResultImpl.hxx` and `resultpp.hxx` files to your project or use a package manager to install it.

### Usage

To use Resultpp, you can include the necessary header in your C++ code:

```c++
#include "resultpp.hxx"

resultpp::internal::ResultImpl<int> result(42); // Ok variant
resultpp::internal::ResultImpl<int> errorResult("Something went wrong"); // Err variant

if (result.IsOk()) {
    int value = result.Data(); // Extract value
}

if (errorResult.IsErr()) {
    std::string errorMsg = errorResult.Message(); // Extract error message
} 
```

### License
This library is open-source and released under the MIT License. You can find the complete license information in the LICENSE file.