#ifndef MOCK_INTERCEPTOR_HPP
#define MOCK_INTERCEPTOR_HPP

#include <fakeit.hpp>

#include "Interceptor.hpp"

template <class C>
class MockInterceptor : public Interceptor<C, C> {
private:
    fakeit::Mock<C> replacement;

public:
    MockInterceptor(C*& intercepted) : Interceptor<C, C>(intercepted) {
        this->replaceWith(&replacement.get(), false);
    }

    fakeit::Mock<C>& getMock() {
        return replacement;
    }
};

#endif
