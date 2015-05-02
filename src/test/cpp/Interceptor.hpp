#ifndef INTERCEPTOR_HPP
#define INTERCEPTOR_HPP

template <class O, class R>
class Interceptor {
private:
    bool deleteReplacementInDestructor;
    O* original;
    R* replacement;
    O*& intercepted;

protected:
    Interceptor(O*& intercepted) : intercepted(intercepted) {
        original = intercepted;
    }

    void replaceWith(R* replacement, bool deleteInDestructor = true) {
        this->deleteReplacementInDestructor = deleteInDestructor;
        this->replacement = replacement;
        intercepted = replacement;
    }

public:
    Interceptor(O*& intercepted, R* replacement,
            bool deleteReplacementInDestructor = true)
            : Interceptor(intercepted) {
        replaceWith(replacement, deleteReplacementInDestructor);
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
