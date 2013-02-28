int a = 0;

class Foo {
    public:
    Foo() {
        a = 1;
    }

    ~Foo() {
        a = 2;
    }
};

static Foo foo;
