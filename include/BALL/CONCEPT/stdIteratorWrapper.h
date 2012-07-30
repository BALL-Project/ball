#ifndef BALL_CONCEPT_STDITERATORWRAPPER_H
#define BALL_CONCEPT_STDITERATORWRAPPER_H

#define IteratorWrapper(type)\
	template<typename T>\
	class type##IteratorWrapper\
	{\
		public:\
			type##IteratorWrapper(T* c) : container_(c) {}\
			type##Iterator begin() { return container_->begin##type(); }\
			type##Iterator end() { return container_->end##type(); }\
\
		private:\
			T* container_;\
	};

#define ConstIteratorWrapper(type)\
	template<typename T>\
	class type##ConstIteratorWrapper\
	{\
		public:\
			type##ConstIteratorWrapper(const T* c) : container_(c) {}\
			type##ConstIterator begin() { return container_->begin##type(); }\
			type##ConstIterator end() { return container_->end##type(); }\
\
		private:\
			const T* container_;\
	};

#define BALL_DECLARE_STD_ITERATOR_WRAPPER(container, type, method_name)\
	type##IteratorWrapper< container > method_name () {\
		return type##IteratorWrapper< container >(this);\
	}\
	type##ConstIteratorWrapper< container > method_name () const {\
		return type##ConstIteratorWrapper< container >(this);\
	}

#endif //BALL_CONCEPT_STDITERATORWRAPPER_H
