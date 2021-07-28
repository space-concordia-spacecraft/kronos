#define TEST_F(fnName) bool fnName()

class TestGroup {

public:
    virtual void Init() = 0;
    virtual void Destroy() = 0;

};