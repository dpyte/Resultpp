#ifndef RESULTPP_RESULTPP_HXX
#define RESULTPP_RESULTPP_HXX

#include "ResultImpl.hxx"

namespace resultpp {
    template<typename T>
    using Result = internal::ResultImpl<T>;
}

#endif//RESULTPP_RESULTPP_HXX