#ifndef STL_DUMPER_INCLUDED_H
#define STL_DUMPER_INCLUDED_H


#include <iostream>
#include <iterator>
#include <type_traits>

template <typename T, size_t N, typename = typename std::enable_if<!std::is_same<typename std::remove_cv<T>::type, char>::value, T>::type>
std::ostream& operator << (std::ostream& os, T(&t)[N])
{
   os.put('[');
   for(auto it = std::begin(t); it != std::end(t); it++){
      if(it != std::begin(t)){
         os << ", " << *it;
      }
      else{
         os << *it;
      }
   }

   os.put(']');
   return os;
}



template <typename U, typename V>
std::ostream& operator << (std::ostream& os, std::pair<U, V> const& pr)
{
   os << "{ " << pr.first << " : " << pr.second << " }";
   return os;
}

template <typename T, typename = typename std::enable_if<!std::is_same<T, std::string>::value, typename T::iterator>::type>
std::ostream& operator << (std::ostream& os, T const& t)
{
   os.put('[');
   for(auto it = std::begin(t); it != std::end(t); it++){
      if(it != std::begin(t)){
         os << ", "<< *it;
      }
      else{
         os << *it;
      }
   }
   os.put(']');
   return os;
}

#endif /* STL_DUMPER_INCLUDED_H */