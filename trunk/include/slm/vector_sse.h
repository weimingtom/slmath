template <class T> class vector_sse
{
    T *buf;
    char *actualbuf;
    size_t alen, ulen;

    void reallocate(size_t n)
    {
        assert(n>ulen);
        T *obuf = buf;
        char *oactual = actualbuf;
        actualbuf = (char *)malloc(sizeof(T)*(alen = n) + 8);
        assert(actualbuf);
        // this is hardcoded to the assumption that the base allocator returns 8 byte aligned memory, and makes it 16 byte aligned
        // this is way more efficient than the generic aligned allocators which have to store additional information in the block etc
        buf = (T *)((size_t)actualbuf & 8 ? actualbuf + 8 : actualbuf);
        if(ulen) memcpy(buf, obuf, ulen*sizeof(T));
        if(oactual) free(oactual);
    }

    public:

    vector_sse() : ulen(0), buf(NULL), actualbuf(NULL) { reallocate(8); }
    ~vector_sse() { setsize(0); free(actualbuf); }

    T &operator[](size_t i) { assert(i<ulen); return buf[i]; }

    void push_back(T &e)
    {
        if(ulen==alen) reallocate(alen*2);
        buf[ulen++] = e;
    }

    T &back()     { assert(ulen); return (*this)[ulen-1]; }
    void drop()   { assert(ulen); --ulen; }
    bool empty()  { return ulen==0; }
    size_t size()    { return ulen; }
    T *getbuf()   { return buf; }
    void setsize  (size_t i) { while(ulen>i) drop(); }
};
