#define TEST_CASE(fnName) bool fnName()
#define CHECK() bool

class TestGroup {

    struct UnitTest {
        bool (* unitFunction) ();
        const char * name;
        const char * description;
    };

public:
    virtual void Init() = 0;
    virtual void Destroy() = 0;

    void AddUnitTest(UnitTest & newUnitTest);
    UnitTest* GetUnitTests(){ return m_UnitTests; }

private:
    UnitTest * m_UnitTests;
};