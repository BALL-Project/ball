CHECK_CXX_SOURCE_COMPILES(
	"template<typename T> class A {}; extern template class A<int>; int main() {}"
	BALL_HAS_EXTERN_TEMPLATES
)
