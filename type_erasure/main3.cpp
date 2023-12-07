// from https://youtu.be/tbUCHifyT24, see also https://godbolt.org/z/Geh4KbE44
#include <cassert>
#include <memory>
#include <utility>
using std::unique_ptr;
using std::make_unique;

struct TEBase {
    virtual unique_ptr<TEBase> clone() const = 0;
    virtual int negate() const = 0;
    virtual bool not_() const = 0;
    virtual ~TEBase() = default;
};
template<class Number>
struct TED : public TEBase {
    Number num_;
    explicit TED(Number n) : num_(std::move(n)) {}
    unique_ptr<TEBase> clone() const override {
        return std::make_unique<TED>(num_);
    }
    int negate() const override { return -num_; }
    bool not_() const override { return !num_; }
};

struct TE {
    unique_ptr<TEBase> p_ = nullptr;
    template<class Number>
    TE(Number n) : p_(
        make_unique<TED<Number>>(std::move(n))
    ) {}
    TE(const TE& rhs) : p_(rhs.p_->clone()) {}
    TE(TE&&) noexcept = default;
    TE& operator=(TE rhs) {
        std::swap(p_, rhs.p_); return *this;
    }
    ~TE() = default;
    int operator-() const {
        return p_->negate();
    }
    bool operator!() const {
        return p_->not_();
    }
};



int main()
{
    TE one(1);
    TE two(2.0);
    assert( -one == -1 );
    assert( !two == false );
}