#pragma once
#ifndef TYPETRAITS_H_
#define TYPETRAITS_H_

#include <type_traits>

namespace Croissant
{
	namespace Tpl
	{
		template<typename T>
		struct TypeTraits
		{
		private:
			template<class U>
			struct tmpType
			{
				typedef U Result;
			};

            template<class U>
            struct tmpType<U&>
            {
                typedef U Result;
            };

        public:
            typedef typename tmpType<T>::Result ParamType;
            //typedef T ParamType;

        };
	}
}

#endif
