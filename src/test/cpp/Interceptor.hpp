#ifndef INTERCEPTOR_HPP
#define INTERCEPTOR_HPP

template <class O, class R>
class Interceptor {
private:
    bool deleteReplacementInDestructor;
    O* original;
    R* replacement;
    O*& intercepted;

public:
    Interceptor(O*& intercepted, R* replacement,
            bool deleteReplacementInDestructor = true);
    ~Interceptor();

    O* getOriginal();
    R* getReplacement();
};

template <class O, class R>
Interceptor<O, R>::Interceptor(O*& intercepted, R* replacement,
        bool deleteReplacementInDestructor) : intercepted(intercepted) {
    this->deleteReplacementInDestructor = deleteReplacementInDestructor;
    this->replacement = replacement;
    original = intercepted;
    intercepted = replacement;
}

template <class O, class R>
Interceptor<O, R>::~Interceptor() {
    intercepted = original;

    if (deleteReplacementInDestructor)
        delete replacement;
}

template <class O, class R>
O* Interceptor<O, R>::getOriginal() {
    return original;
}

template <class O, class R>
R* Interceptor<O, R>::getReplacement() {
    return replacement;
}

#endif
