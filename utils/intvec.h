
#pragma once

 // An STL-ish vector without templates
  class FL_EXPORT CLongVector
  {
    long *arr;
    unsigned int _size;
    void init()
    {
      arr = NULL;
      _size = 0;
    }
    
    void copy(long *newarr, unsigned int newsize)
    {
      size(newsize);
      memcpy(arr, newarr, newsize * sizeof(long));
    }
    
  public:
    CLongVector() { init(); }					// CTOR
    ~CLongVector() { if ( arr ) free(arr); arr = NULL; }		// DTOR
    CLongVector(CLongVector &o) { init(); copy(o.arr, o._size); }	// COPY CTOR
    CLongVector& operator=(CLongVector&o)
   {				// ASSIGN
      init();
      copy(o.arr, o._size);
      return(*this);
    }
    long operator[](int x) const { return(arr[x]); }
    long& operator[](int x) { return(arr[x]); }
    unsigned int size() { return(_size); }
    void size(unsigned int count) {
      if ( count != _size ) {
        arr = (long*)realloc(arr, count * sizeof(long));
        _size = count;
      }
    }
    long erase(int x)
    {
        if(x<0 || x >= (int)_size) return 0;
        long tmp = arr[x];
        arr[x]=0;
        if(x < (int)_size-1)
            memcpy(arr + x , arr + (x+1), (_size-1 - x) * sizeof(long));
        
        size(_size-1);
        return tmp;
    }
    void swap(int a, int b)
    {
        long l = arr[a];
        arr[a] = arr[b];
        arr[b] = l;
    }
    void insert(long val, int before)
    {
        if(before >= (int)_size)
        {
            push_back(val);
            return;
        }
        if(before<0) before=0;
        size(_size+1);
        
        memcpy(arr + before +1 , arr + before, (_size-1 - before) * sizeof(long));
        arr[before] = val;        
    }
    long pop_back() { long tmp = arr[_size-1]; _size--; return(tmp); }
    void push_back(long val) { unsigned int x = _size; size(_size+1); arr[x] = val; }
    long back() { return(arr[_size-1]); }
  };
