namespace LIB {
    template <typename rt, typename... args> using fn = rt(*)(args...);
    template <typename nT = int> struct num {
        nT elem;
        operator nT& () { return elem; }
        num(nT from = 0) : elem(from){}
    };
    typedef unsigned __int64 ui64;
    typedef unsigned __int32 ui32;
    typedef unsigned __int16 ui16;
    typedef unsigned __int8   ui8;
    typedef signed __int64    i64;
    typedef signed __int32    i32;
    typedef signed __int16    i16;
    typedef signed __int8      i8;
    typedef           double    d;
    typedef    long   double   ld;
    typedef           float     f;
}
using namespace LIB;