#pragma once

template<class T>
struct TwObjectDetect
{
    template<typename A> static char check(decltype(A::classInfo)  )
    {
        std::cout<<"TwObjectDetect::exist classInfo"<<std::endl;
        return 'y';
    }
	template<typename B> static float check(...)
    {
        std::cout<<"TwObjectDetect::..."<<std::endl;
        return 0.0f;
    }

	static const bool isTwObjectType = (sizeof(check<T>(nullptr)) == sizeof(check<TwObject>(nullptr)));
};

template<class T, bool i>
struct TwHolderDetect
{

};

template<class T>
struct TwHolderDetect<T, true>
{
	typedef TwWeakPtr<T> Holder;

};

template<class T>
struct TwHolderDetect<T, false>
{
	typedef T* Holder;
};


template<class T>
struct TwHolderTraits
{
    typedef typename TwHolderDetect<T, TwObjectDetect<T>::isTwObjectType>::Holder TwHolder;
};


template<class T>
inline T* getObj(T* o) { return o;}
template<class T>
inline T* getObj(const TwWeakPtr<T>& o) { return o.data();} 
