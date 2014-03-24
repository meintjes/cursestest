#ifndef __UNIQUE_H__
#define __UNIQUE_H__

//contains a serilization function for unique pointers. credit dodol from SO:
//http://stackoverflow.com/questions/12915267/boost-serialization-stdunique-ptr-support

#include <boost/serialization/split_free.hpp>
#include <memory>
namespace boost { 
namespace serialization {

template<class Archive, class T>
inline void save(
    Archive & ar,
    const std::unique_ptr< T > &t,
    const unsigned int file_version
){
    // only the raw pointer has to be saved
    const T * const tx = t.get();
    ar << tx;
}
template<class Archive, class T>
inline void load(
    Archive & ar,
    std::unique_ptr< T > &t,
    const unsigned int file_version
){
    T *pTarget;
    ar >> pTarget;

    #if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
        t.release();
        t = std::unique_ptr< T >(pTarget);
    #else
        t.reset(pTarget);
    #endif
}
template<class Archive, class T>
inline void serialize(
    Archive & ar,
    std::unique_ptr< T > &t,
    const unsigned int file_version
){
    boost::serialization::split_free(ar, t, file_version);
}
} // namespace serialization
} // namespace boost

#endif
