#include <iostream>
#include <memory>
#include <string>

template <typename T>
class Container {
 public:
  explicit Container(const T& value) : value_(std::make_unique<Model<T>>(value)) {}

  template <typename U>
  explicit Container(const Container<U>& other)
      : value_(std::make_unique<Model<U>>(*other.value_)) {}

  T& get() { return value_->get(); }
  const T& get() const { return value_->get(); }

 private:
  struct Concept {
    virtual ~Concept() = default;
    virtual T& get() = 0;
    virtual const T& get() const = 0;
  };

  template <typename U>
  struct Model : Concept {
    Model(const U& value) : value_(value) {}
    T& get() override { return value_; }
    const T& get() const override { return value_; }
    U value_;
  };

  std::unique_ptr<Concept> value_;
};

int main() {
  Container<int> c1(42);
  Container<std::string> c2("hello");

  std::cout << c1.get() << std::endl;
  std::cout << c2.get() << std::endl;

  return 0;
}
