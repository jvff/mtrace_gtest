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
            bool deleteReplacementInDestructor = true)
            : intercepted(intercepted) {
        this->deleteReplacementInDestructor = deleteReplacementInDestructor;
        this->replacement = replacement;
        original = intercepted;
        intercepted = replacement;
    }

    ~Interceptor() {
        intercepted = original;

        if (deleteReplacementInDestructor)
            delete replacement;
    }

    O* getOriginal() {
        return original;
    }

    R* getReplacement() {
        return replacement;
    }
};

#endif
